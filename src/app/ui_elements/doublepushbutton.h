#pragma once
#ifndef DOUBLEPUSHBUTTON_H
#define DOUBLEPUSHBUTTON_H

/* a QPushButton which can be double-clicked */

#include "colorcopymenu.h"
#include <QPushButton>

class DoublePushButton final : public QPushButton {
    Q_OBJECT
signals:
    void colorChanged(QColor color);
public:
    explicit DoublePushButton(QWidget* parent);
    ~DoublePushButton();
    void setColor(QColor color);
    QColor getColor();
protected:
    void mouseDoubleClickEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent* event);
private:
    QColor currentColor;
    ColorCopyMenu* menu;
private slots:
    void copyColorSlot();
    void colorEditedSlot(QColor color);
};

#endif // DOUBLEPUSHBUTTON_H
