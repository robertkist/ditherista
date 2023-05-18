#pragma once
#ifndef TREEWIDGETDELEGATE_H
#define TREEWIDGETDELEGATE_H

#include <QStyledItemDelegate>

class TreeWidgetDelegate : public QStyledItemDelegate {
public:
    TreeWidgetDelegate(QObject* parent);
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    int selected_row = 0;  // row number of the currently selected item
private:
    QPixmap ready, notready;
};

#endif  // TREEWIDGETDELEGATE_H
