#include "imagehash.h"
#include "math.h"

void ImageHash::clearAllDitheredImages() {
    /* clears all dithered images */
    QHashIterator <int, QImage*> i(out_img);
    while (i.hasNext()) {
        i.next();
        delete out_img[i.key()];
    }
    out_img.clear();
}

void ImageHash::reset() {
    /* clears all dithered images and source / original images */
    clearAllDitheredImages();
    if(src_buf != nullptr) DitherImage_free(src_buf);
    if(orig_buf != nullptr) DitherImage_free(orig_buf);
}

void ImageHash::clearDitheredImage(int i) {
    /* clears an individual dithered image */
    delete out_img[i];
    out_img.remove(i);
}

void ImageHash::setImageFromDither(int i, const uint8_t* out_buf) {
    if(out_img.contains(i))
        clearDitheredImage(i);
    out_img[i] = new QImage(src_buf->width, src_buf->height, QImage::Format_Mono);
    out_img[i]->fill(0);
    for (int y = 0; y < src_buf->height; y++)
        for (int x = 0; x < src_buf->width; x++)
            if (out_buf[y * src_buf->width + x] == 0xff)
                out_img[i]->setPixel(x, y, 1);
}

QImage* ImageHash::getDitheredImage(int i) const {
    return out_img[i];
}

void ImageHash::setSourceImage(const QImage* img) {
    reset();
    src_img = img->convertToFormat(QImage::Format_RGB32);
    src_buf = DitherImage_new(src_img.width(), src_img.height());
    orig_buf = DitherImage_new(src_img.width(), src_img.height());
    for(int y = 0; y < src_buf->height; y++) {
        for(int x = 0; x < src_buf->width; x++) {
            QRgb pixel = src_img.pixel(QPoint(x, y));
            DitherImage_set_pixel(src_buf, x, y, qRed(pixel), qGreen(pixel), qBlue(pixel), true);
        }
    }
    memcpy(orig_buf->buffer, src_buf->buffer, src_buf->width * src_buf->height * sizeof(double));
    adjustSource(0.0, 0.0, 0.0);
}

ImageHash::~ImageHash() {
    reset();
}

void ImageHash::adjustSource(double gamma, double brightness, double contrast) {
    for(int y = 0; y < src_buf->height; y++) {
        for(int x = 0; x < src_buf->width; x++) {
            size_t i = y * src_buf->width + x;
            double c = orig_buf->buffer[i];
            c = pow(c, 1.0 / (gamma + 1.0));         // apply gamma
            c = (c - 0.5) * (contrast + 1.0) + 0.5;  // apply contrast
            c = c + brightness;                      // apply brightness
            c = c < 0.0 ? 0.0 : (c > 1.0 ? 1.0 : c); // clamp to 0.0 - 1.0 range
            src_buf->buffer[i] = c;
            int p = (int)(gamma_encode(c) * 255.0);
            src_img.setPixel(x, y, qRgb(p, p, p));
        }
    }
}
