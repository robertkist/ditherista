#include <QDebug>
#include "imagehashcolor.h"

void ImageHashColor::setSourceImage(const QImage* inputImage) {
    /* sets the source images */
    ImageHash::setSourceImage(inputImage);
    sourceImage = ColorImage_new(origQImage.width(), origQImage.height());
    for(int y = 0; y < origQImage.height(); y++) {
        for(int x = 0; x < origQImage.width(); x++) {
            const QRgb pixel = origQImage.pixel(QPoint(x, y));
            ColorImage_set_rgb(sourceImage, y * origQImage.width() + x, qRed(pixel), qGreen(pixel), qBlue(pixel), qAlpha(pixel));
        }
    }
    brightness = DEFAULT_COLOR_BRIGHTNESS_ADJUST;
    contrast = DEFAULT_COLOR_CONTRAST_ADJUST;
    gamma = DEFAULT_COLOR_GAMMA_ADJUST;
    saturation = DEFAULT_COLOR_SATURATION_ADJUST;
    adjustSource();
}

void ImageHashColor::adjustSource() {
    double dBrightness = (double)brightness / 100.0;
    double dContrast = (double)(contrast / 100.0) + 1.0;
    double dGamma = 1.0 / ((double)(gamma / 100.0) + 1.0);
    double dSaturation = (double)(saturation / 100.0) + 1.0;
    double remove_gamma_exp = 1.0 / 2.2;
    for (int y = 0; y < sourceImage->height; y++) {
        for (int x = 0; x < sourceImage->width; x++) {
            const QRgb pixel = origQImage.pixel(x, y);
            // convert from sRGB to linear
            double fr = (double)qRed(pixel) / 255.0;
            double fg = (double)qGreen(pixel) / 255.0;
            double fb = (double)qBlue(pixel) / 255.0;
            // RGB to linear
            fr = pow(fr, remove_gamma_exp);
            fg = pow(fg, remove_gamma_exp);
            fb = pow(fb, remove_gamma_exp);
            // apply gamma
            fr = pow(fr, dGamma);
            fg = pow(fg, dGamma);
            fb = pow(fb, dGamma);
            // apply contrast
            fr = (fr - 0.5) * dContrast + 0.5;
            fg = (fg - 0.5) * dContrast + 0.5;
            fb = (fb - 0.5) * dContrast + 0.5;
            // apply brightness
            fr = fr + dBrightness;
            fg = fg + dBrightness;
            fb = fb + dBrightness;
            // clamp
            fr = fr < 0.0 ? 0.0 : (fr > 1.0 ? 1.0 : fr);
            fg = fg < 0.0 ? 0.0 : (fg > 1.0 ? 1.0 : fg);
            fb = fb < 0.0 ? 0.0 : (fb > 1.0 ? 1.0 : fb);
            // apply saturation
            double grey = fr * 0.299 + fg * 0.586 + fb * 0.114; // perception based conversion to grey
            fr = grey * (1.0 - dSaturation) + fr * dSaturation;
            fg = grey * (1.0 - dSaturation) + fg * dSaturation;
            fb = grey * (1.0 - dSaturation) + fb * dSaturation;
            // clamp
            fr = fr < 0.0 ? 0.0 : (fr > 1.0 ? 1.0 : fr);
            fg = fg < 0.0 ? 0.0 : (fg > 1.0 ? 1.0 : fg);
            fb = fb < 0.0 ? 0.0 : (fb > 1.0 ? 1.0 : fb);
            // convert from linear back to sRGB
            fr = pow(fr, 2.2);
            fg = pow(fg, 2.2);
            fb = pow(fb, 2.2);
            uint8_t br = (uint8_t)(fr * 255.0);
            uint8_t bg = (uint8_t)(fg * 255.0);
            uint8_t bb = (uint8_t)(fb * 255.0);
            sourceQImage.setPixel(x, y, qRgba(br, bg, bb, qAlpha(pixel)));
            ColorImage_set_rgb(sourceImage, y * sourceImage->width + x, br, bg, bb, qAlpha(pixel));
        }
    }
}

void ImageHashColor::reset() {
    /* clears all dithered images and source / original images */
    ImageHash::reset();
    if(sourceImage != nullptr) {
        ColorImage_free(sourceImage);
    }
}

ColorImage* ImageHashColor::getSourceImage() const {
    /* returns the source image (with adjustments) */
    return sourceImage;
}

void ImageHashColor::setImageFromDither(int i, const BytePalette* pal, const int* out_buf) {
    /* sets the dithered image from the ditherer's output buffer */
    if(outImage.contains(i)) {
        clearDitheredImage(i);
    }
    outImage[i] = new QImage(sourceImage->width, sourceImage->height, QImage::Format_ARGB32);
    outImage[i]->fill(0);
    for (int y = 0; y < sourceImage->height; y++) {
        for (int x = 0; x < sourceImage->width; x++) {
            size_t j = y * sourceImage->width + x;
            if (out_buf[j] == -1) {
                outImage[i]->setPixel(x, y, qRgba(0, 0, 0, 0));
            } else {
                ByteColor *c = BytePalette_get(pal, (size_t) out_buf[j]);
                outImage[i]->setPixel(x, y, qRgba(c->r, c->g, c->b, c->a));
            }
        }
    }
}

ColorImage* ImageHashColor::getSourceImage() {
    return sourceImage;
};
