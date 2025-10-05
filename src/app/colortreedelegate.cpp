#include "colortreedelegate.h"
#include <QPainter>

ColorTreeDelegate::ColorTreeDelegate(QObject* parent) : QStyledItemDelegate(parent) {

};

void ColorTreeDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    /* note: we have to use the 'selected_row' helper variable instead of using
     * option.state & QStyle::State_Selected to determine if a row should be drawn as
     * selected, because in rare cases the delegate gets confused about what is actually selected
     * due to the auto-expanding stacked widget that is jostling things around (the treewidget
     * itself isn't confused; it always knows what really has been clicked - it's just the delegate that's
     * the problem... */
    painter->setRenderHint(QPainter::Antialiasing, true);
    // draw blue background for currently selected row
    if (option.state & QStyle::State_Selected && option.state & QStyle::State_Enabled) {
        painter->setPen(Qt::transparent);
        painter->setBrush(option.palette.highlight());
        painter->drawRoundedRect(option.rect, 6, 6);

        QRect r = QRect(option.rect.topLeft(), option.rect.size());
        if (index.column() == 0) {
            r.setLeft(r.left() + 6);
            painter->drawRect(r);
        } else {
            r.setRight(r.right() - 6);
            painter->drawRect(r);
        }
    }
    // draw row contents
    QStyleOptionViewItem itemOption(option);
    if(option.state & QStyle::State_Enabled) {
        itemOption.state &= QStyle::State_Enabled;
    } else {
        itemOption.state = QStyle::State_None;
    }
    QStyledItemDelegate::paint(painter, itemOption, index);
}
