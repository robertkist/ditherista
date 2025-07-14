#pragma once
#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include "graphicspixmapitem.h"
#include <QGraphicsView>
#include <QDragEnterEvent>

class GraphicsView final : public QGraphicsView {
    Q_OBJECT
signals:
    void loadImageSignal(QString fileName);
public:
    /* methods */
    explicit GraphicsView(QWidget* parent = nullptr);
    void dropEvent(QDropEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void resetScene(int width, int height);

    void showSourceMono(bool show) const;
    void showSourceColor(const bool show) const;

    void setDitherImageMono(const QImage* img, const QString& partialFileName);
    void setDitherImageColor(const QImage* img, const QString& partialFileName);

    void setSourceImageMono(const QImage* img);
    void setSourceImageColor(const QImage* img);

    ~GraphicsView() override;
private:
    /* attributes */
    int zoomLevel = 0;
    QGraphicsScene scene;
    GraphicsPixmapItem* out_pix_item_mono = nullptr;   // dithered mono image
    GraphicsPixmapItem* out_pix_item_color = nullptr;  // dithered color image
    QGraphicsPixmapItem* src_pix_item_mono = nullptr;  // source mono image
    QGraphicsPixmapItem* src_pix_item_color = nullptr; // source color image
    QStringList tempFiles;
    /* methods */
    void deleteTempFiles();
private slots:
    void tempFileCreatedSlot(const QString& fileName);
};

#endif // GRAPHICSVIEW_H
