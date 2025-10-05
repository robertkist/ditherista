#pragma once
#ifndef TREEWIDGETDELEGATE_H
#define TREEWIDGETDELEGATE_H

#include <QStyledItemDelegate>

class TreeWidgetDelegate final : public QStyledItemDelegate {
public:
    /* attributes */
    int selected_row = 0; // row number of the currently selected item
    /* methods */
    explicit TreeWidgetDelegate(QObject* parent);
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
private:
    /* attributes */
    QPixmap ready;
    QPixmap notReady;
};

#endif  // TREEWIDGETDELEGATE_H
