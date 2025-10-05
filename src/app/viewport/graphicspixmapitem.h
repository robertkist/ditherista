#pragma once
#ifndef GRAPHICSPIXMAPITEM_H
#define GRAPHICSPIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

class GraphicsPixmapItem final : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
signals:
    void tempFileCreated(QString fileName);
public:
    /* methods */
    explicit GraphicsPixmapItem(const QPixmap& p);
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
private:
    bool dragging = false;
    /* attributes */
    bool drag = false;
};

#endif // GRAPHICSPIXMAPITEM_H
