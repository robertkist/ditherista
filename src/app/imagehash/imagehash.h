#pragma once
#ifndef IMAGEHASH_H
#define IMAGEHASH_H

#include "../../../libdither/src/libdither/libdither.h"
#include <QObject>
#include <QImage>
#include <QHash>

class ImageHash : public QObject {
    Q_OBJECT
public:
    /* methods */
    void clearAllDitheredImages();
    void clearDitheredImage(int i);
    bool hasDitheredImage(int i) const;
    void setSourceImage(const QImage* img);
    [[nodiscard]] QImage* getDitheredImage(int i) const;
    QImage* getSourceQImage();
    ~ImageHash() override;
protected:
    /* attributes */
    QHash<int, QImage*> outImage; // dithered images
    QImage sourceQImage;          // source with adjustments as QImage
    QImage origQImage;            // original image as QImage
    /* methods */
    void reset();

};

#endif  // IMAGEHASH_H
