#pragma once
#ifndef IMAGEHASHCOLOR_H
#define IMAGEHASHCOLOR_H

#include "../../../libdither/src/libdither/libdither.h"
#include "imagehash.h"
#include <QObject>
#include <QImage>
#include <QHash>

// values are from 1 - 100
#define DEFAULT_COLOR_BRIGHTNESS_ADJUST 0
#define DEFAULT_COLOR_CONTRAST_ADJUST 0
#define DEFAULT_COLOR_GAMMA_ADJUST 0
#define DEFAULT_COLOR_SATURATION_ADJUST 0

class ImageHashColor final : public ImageHash {
public:
    /* methods */
    void setSourceImage(const QImage* img);
    void setImageFromDither(int i, const BytePalette* pal, const int* out_buf);
    [[nodiscard]] ColorImage* getSourceImage() const;
    void adjustSource();
    ColorImage* getSourceImage();

    /* attributes */
    int brightness = DEFAULT_COLOR_BRIGHTNESS_ADJUST;
    int contrast = DEFAULT_COLOR_CONTRAST_ADJUST;
    int gamma = DEFAULT_COLOR_GAMMA_ADJUST;
    int saturation = DEFAULT_COLOR_SATURATION_ADJUST;
private:
    /* attributes */
    ColorImage* sourceImage = nullptr;  // source with adjustments as DitherImage
    /* methods */
    void reset();
};

#endif  // IMAGEHASHCOLOR_H
