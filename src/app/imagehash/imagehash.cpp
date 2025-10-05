#include "imagehash.h"
#include <QDebug>

ImageHash::~ImageHash() {
    /* destructor */
    reset(); // free memory
}

bool ImageHash::hasDitheredImage(const int i) const {
    /* returns true if a dithered image for the given ditherer exists */
    return outImage.contains(i);
}

void ImageHash::clearAllDitheredImages() {
    /* clears all dithered images */
    QHashIterator <int, QImage*> i(outImage);
    while (i.hasNext()) {
        i.next();
        delete outImage[i.key()];
    }
    outImage.clear();
}

void ImageHash::clearDitheredImage(const int i) {
    /* clears an individual dithered image */
    delete outImage[i];
    outImage.remove(i);
}

void ImageHash::reset() {
    /* clears all dithered images and source / original images */
    clearAllDitheredImages();
}

QImage* ImageHash::getDitheredImage(const int i) const {
    /* returns a dithered image */
    return outImage[i];
}

QImage* ImageHash::getSourceQImage() {
    /* returns the source image (with adjustments) */
    return &sourceQImage;
}

void ImageHash::setSourceImage(const QImage* inputImage) {
    /* sets the source images */
    origQImage = inputImage->convertToFormat(QImage::Format_ARGB32);    // 'backup' of the original image
    sourceQImage = inputImage->convertToFormat(QImage::Format_ARGB32);  // image which will receive adjustments (e.g. brightness)
    clearAllDitheredImages();
}
