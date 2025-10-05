#pragma once
#ifndef COLORTREEDELEGATE_H
#define COLORTREEDELEGATE_H

#include <QStyledItemDelegate>

class ColorTreeDelegate final : public QStyledItemDelegate {
public:
    /* methods */
    explicit ColorTreeDelegate(QObject* parent);
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif  // COLORTREEDELEGATE_H
