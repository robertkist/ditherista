#pragma once
#ifndef DOUBLELABEL_H
#define DOUBLELABEL_H

/* A QLabel which sends a signal when double-clicked */

#include "colorcopymenu.h"
#include <QLabel>

class DoubleLabel final : public QLabel {
    Q_OBJECT
signals:
    void colorChanged(QColor color);
private:
    ColorCopyMenu* menu;
public:
    explicit DoubleLabel(QWidget* parent);
    ~DoubleLabel();
    void setColor(QColor color);
protected:
    void mouseDoubleClickEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent* event);
private slots:
    void copyColorSlot();
    void colorEditedSlot(QColor color);
};

#endif // DOUBLELABEL_H
