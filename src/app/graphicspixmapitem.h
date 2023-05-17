#pragma once
#ifndef GRAPHICSPIXMAPITEM_H
#define GRAPHICSPIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

class GraphicsPixmapItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    GraphicsPixmapItem(const QPixmap& p);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent*);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
signals:
    void tempFileCreated(QString fileName);
};

#endif // GRAPHICSPIXMAPITEM_H
