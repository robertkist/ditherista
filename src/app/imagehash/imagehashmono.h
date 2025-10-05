#pragma once
#ifndef IMAGEHASHMONO_H
#define IMAGEHASHMONO_H

#include "../../../libdither/src/libdither/libdither.h"
#include "imagehash.h"
#include <QObject>
#include <QImage>
#include <QHash>

// values are from 1 - 100
#define DEFAULT_MONO_BRIGHTNESS_ADJUST 0
#define DEFAULT_MONO_CONTRAST_ADJUST 0
#define DEFAULT_MONO_GAMMA_ADJUST 0

class ImageHashMono final : public ImageHash {
public:
    /* methods */
    void setSourceImage(const QImage* img);
    void setImageFromDither(int i, const uint8_t* out_buf);
    [[nodiscard]] DitherImage* getSourceImage() const;
    void adjustSource();

    /* attributes */
    int brightness = DEFAULT_MONO_BRIGHTNESS_ADJUST;
    int contrast = DEFAULT_MONO_CONTRAST_ADJUST;
    int gamma = DEFAULT_MONO_GAMMA_ADJUST;

private:
    /* attributes */
    DitherImage* sourceImage = nullptr;  // source with adjustments as DitherImage
    DitherImage* origLinear = nullptr;
    /* methods */
    void reset();
};

#endif  // IMAGEHASHMONO_H
