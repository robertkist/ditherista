#include "imagehashmono.h"

void ImageHashMono::setSourceImage(const QImage* inputImage) {
    /* sets the source images */
    ImageHash::setSourceImage(inputImage);
    sourceImage = DitherImage_new(origQImage.width(), origQImage.height());
    origLinear = DitherImage_new(origQImage.width(), origQImage.height());
    for(int y = 0; y < sourceImage->height; y++) {
        for(int x = 0; x < sourceImage->width; x++) {
            const QRgb pixel = origQImage.pixel(QPoint(x, y));
            DitherImage_set_pixel_rgba(sourceImage, x, y, qRed(pixel), qGreen(pixel), qBlue(pixel), qAlpha(pixel), true);
            DitherImage_set_pixel_rgba(origLinear, x, y, qRed(pixel), qGreen(pixel), qBlue(pixel), qAlpha(pixel), true);
        }
    }
    brightness = DEFAULT_MONO_BRIGHTNESS_ADJUST;
    contrast = DEFAULT_MONO_CONTRAST_ADJUST;
    gamma = DEFAULT_MONO_GAMMA_ADJUST;
    adjustSource();
}

void ImageHashMono::adjustSource() {
    /* applies image adjustments for brightness, contrast, gamma, etc. */
    double dBrightness = (double)brightness / 100.0;
    double dContrast = (double)(contrast / 100.0) + 1.0;
    double dGamma = 1.0 / ((double)(gamma / 100.0) + 1.0);
    for (int y = 0; y < sourceImage->height; y++) {
        for (int x = 0; x < sourceImage->width; x++) {
            const size_t i = y * sourceImage->width + x;
            double c = origLinear->buffer[i];
            c = pow(c, dGamma);               // apply gamma
            c = (c - 0.5) * dContrast + 0.5;  // apply contrast
            c = c + dBrightness;              // apply brightness
            c = c < 0.0 ? 0.0 : (c > 1.0 ? 1.0 : c); // clamp to 0.0 - 1.0 range
            sourceImage->buffer[i] = c;
            int p = static_cast<int>(gamma_encode(c) * 255.0);
            sourceQImage.setPixel(x, y, qRgba(p, p, p, sourceImage->transparency[i]));
        }
    }
}

void ImageHashMono::reset() {
    /* clears all dithered images and source / original images */
    ImageHash::reset();
    if(sourceImage != nullptr) {
        DitherImage_free(sourceImage);
        DitherImage_free(origLinear);
    }
}

DitherImage* ImageHashMono::getSourceImage() const {
    /* returns the source image (with adjustments) */
    return sourceImage;
}

void ImageHashMono::setImageFromDither(int i, const uint8_t* out_buf) {
    /* sets the dithered image from the ditherer's output buffer */
    if(outImage.contains(i)) {
        clearDitheredImage(i);
    }
    outImage[i] = new QImage(sourceImage->width, sourceImage->height, QImage::Format_ARGB32);
    outImage[i]->fill(qRgb(0, 0, 0));
    for (int y = 0; y < sourceImage->height; y++) {
        for (int x = 0; x < sourceImage->width; x++) {
            size_t addr = (size_t)(y * sourceImage->width + x);
            if (out_buf[addr] == 255) {
                outImage[i]->setPixel(x, y, qRgba(255, 255, 255, 255));
            } else if (out_buf[addr] == 128) {
                outImage[i]->setPixel(x, y, qRgba(0, 0, 0, 0));
            }
        }
    }
}