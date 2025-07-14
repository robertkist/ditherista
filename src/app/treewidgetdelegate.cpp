#include "treewidgetdelegate.h"
#include <QPainter>

// https://stackoverflow.com/questions/7175333/how-to-create-delegate-for-qtreewidget

constexpr int ICON_DIMENSION_PX = 16;

TreeWidgetDelegate::TreeWidgetDelegate(QObject* parent) : QStyledItemDelegate(parent) {
    ready = QPixmap(":/resources/img_done.svg");
    notReady = QPixmap(":/resources/img_notdone.svg");
};

void TreeWidgetDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    /* note: we have to use the 'selected_row' helper variable instead of using
     * option.state & QStyle::State_Selected to determine if a row should be drawn as
     * selected, because in rare cases the delegate gets confused about what is actually selected
     * due to the auto-expanding stacked widget that is jostling things around (the treewidget
     * itself isn't confused; it always knows what really has been clicked - it's just the delegate that's
     * the problem... */
    painter->setRenderHint(QPainter::Antialiasing, true);
    // draw blue background for currently selected row
    if(index.row() == selected_row && option.state & QStyle::State_Enabled) {  // this was using option.state & QStyle::State_Selected
        painter->setPen(Qt::transparent);
        painter->setBrush(option.palette.highlight());
        painter->drawRoundedRect(option.rect, 6, 6);
    }
    // draw row contents
    QStyleOptionViewItem itemOption(option);
    if(option.state & QStyle::State_Enabled) {
        itemOption.state &= QStyle::State_Enabled;
    } else {
        itemOption.state = QStyle::State_None;
    }
    QStyledItemDelegate::paint(painter, itemOption, index);
    // draw icon when image has been dithered / not dithered
    int dim = ICON_DIMENSION_PX; // image dimensions
    int ypos = option.rect.y() + static_cast<int>((option.rect.height() - dim) * 0.5f);
    int xpos = option.rect.width() - dim - 4;
    if(index.data(Qt::UserRole).toBool()) {
        painter->drawPixmap(QRect(xpos, ypos, dim, dim), ready);
    } else {
        painter->drawPixmap(QRect(xpos, ypos, dim, dim), notReady);
    }
}
