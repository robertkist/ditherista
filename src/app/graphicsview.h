#pragma once
#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include "graphicspixmapitem.h"
#include <QGraphicsView>
#include <QDragEnterEvent>

class GraphicsView : public QGraphicsView {
    Q_OBJECT
signals:
    void loadImageSignal(QString fileName);
public:
    GraphicsView(QWidget* parent = nullptr);
    void dropEvent(QDropEvent* event);
    void dragEnterEvent(QDragEnterEvent* event) { event->acceptProposedAction(); };
    void dragMoveEvent(QDragMoveEvent* event) { event->acceptProposedAction(); };
    void dragLeaveEvent(QDragLeaveEvent* event) { event->accept(); };
    void showSource(bool show);
    void resetScene(int width, int height);
    void setDitherImage(const QImage* img, QString partialFileName);
    void setSourceImage(const QImage* img);
    ~GraphicsView() { deleteTempFiles(); };
private:
    QGraphicsScene scene;
    GraphicsPixmapItem* out_pix_item = nullptr;
    QGraphicsPixmapItem* src_pix_item = nullptr;
    QStringList tempFiles;
    void deleteTempFiles();
private slots:
    void tempFileCreatedSlot(QString fileName) { tempFiles<<fileName; };
};

#endif // GRAPHICSVIEW_H
