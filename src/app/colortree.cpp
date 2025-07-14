#include "colortree.h"
#include "colortreedelegate.h"
#include "ui_elements/svg.h"
#include <QMouseEvent>

ColorTree::ColorTree(QWidget* parent) : QTreeWidget(parent) {
    /* Constructor */
    ColorTreeDelegate* delegate = new ColorTreeDelegate(this);
    setItemDelegate(delegate);
    menu = new ColorCopyMenu(this);
    connect(menu, SIGNAL(copyColor()), this, SLOT(copyColorSlot()));
    connect(menu, SIGNAL(colorEdited(QColor)), this, SLOT(colorEditedSlot(QColor)));
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(doubleClickedSlot(QTreeWidgetItem*, int)));
}

ColorTree::~ColorTree() {
    /* Destructor */
    delete menu;
}

void ColorTree::colorEditedSlot(QColor color) {
    int index = clickedItem->text(0).toInt();
    emit colorChanged(index - 1, color);
}

void ColorTree::copyColorSlot() {
    /* copies the right clicked item's color value to the clipboard */
    menu->sendToClipboard(clickedItem->text(1).toLower());
}

void ColorTree::mousePressEvent(QMouseEvent* event) {
    /* handles mouse click events */
    if (event->button() == Qt::RightButton) {
        if (QTreeWidgetItem* rightClickItem = itemAt(event->pos())) {  // only trigger context menu at currently selected item
            clickedItem = rightClickItem;
            menu->drawMenu(mapToGlobal(event->pos()), QColor(clickedItem->text(1)));
        }
        event->accept();
    } else if (event->button() == Qt::LeftButton) {
        QTreeWidget::mousePressEvent(event);
        event->accept();
    }
}

void ColorTree::doubleClickedSlot(QTreeWidgetItem* item, int) {
    /* user double clicked on an entry in the tree */
    clickedItem = item;
    menu->editColor(QColor(item->text(1)));
}

void ColorTree::addColorEntry(int index, ByteColor* color) {
    /* adds a new color entry to the tree - used to populate the tree */
    QString hexColor = rgbToHexString(color->r, color->g, color->b);
    QPixmap* pixmap = getColorDotSvg(hexColor, this);
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setIcon(0, QIcon(*pixmap));
    item->setText(0, QString("%1").arg(index + 1));
    item->setText(1, QString("#%1%2").arg(color->a, 2, 16, QChar('0')).arg(hexColor).toUpper());
    addTopLevelItem(item);
    delete pixmap;
}