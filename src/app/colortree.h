#pragma once
#ifndef COLORTREE_H
#define COLORTREE_H

#include <QTreeWidget>
#include <QAction>
#include "libdither.h"
#include "ui_elements/colorcopymenu.h"

class ColorTree final : public QTreeWidget {
    Q_OBJECT
signals:
    void colorChanged(int index, QColor color);
public:
    /* methods */
    explicit ColorTree(QWidget*);
    ~ColorTree();
    void mousePressEvent(QMouseEvent* event);
    void addColorEntry(int index, ByteColor* color);
private:
    /* attributes */
    ColorCopyMenu* menu;
    QTreeWidgetItem* clickedItem = nullptr;
    QColor pastedColor;
private slots:
    void copyColorSlot();
    void colorEditedSlot(QColor color);
    void doubleClickedSlot(QTreeWidgetItem* item, int);
};

#endif  // COLORTREE_H
