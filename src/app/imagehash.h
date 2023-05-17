#pragma once
#ifndef IMAGEHASH_H
#define IMAGEHASH_H

#include "libdither.h"
#include <QObject>
#include <QImage>
#include <QHash>

class ImageHash : public QObject{
    Q_OBJECT
public:
    void clearAllDitheredImages();
    void clearDitheredImage(int i);
    void setSourceImage(const QImage* img);
    bool hasDitheredImage(int i) { return out_img.contains(i); };
    void setImageFromDither(int i, const uint8_t *out_buf);
    void adjustSource(double gamma, double brightness, double contrast);
    QImage* getDitheredImage(int i) const;
    QImage* getSourceQImage() { return &src_img; };
    DitherImage* getSourceDitherImage() { return src_buf; }
    ~ImageHash();
private:
    void reset();
    DitherImage* orig_buf = nullptr; // unmodified source (no brightness, contrast, etc.)
    DitherImage* src_buf = nullptr;  // input image buffer (0.0 - 1.0 range)
    QHash<int, QImage*> out_img;
    QImage src_img;
};

#endif  // IMAGEHASH_H
