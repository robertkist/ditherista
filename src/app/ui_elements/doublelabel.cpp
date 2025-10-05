#include "doublelabel.h"
#include <QMouseEvent>

DoubleLabel::DoubleLabel(QWidget* parent) : QLabel(parent) {
    menu = new ColorCopyMenu(this);
    connect(menu, SIGNAL(copyColor()), this, SLOT(copyColorSlot()));
    connect(menu, SIGNAL(colorEdited(QColor)), this, SLOT(colorEditedSlot(QColor)));
}

DoubleLabel::~DoubleLabel() {
    delete menu;
}

void DoubleLabel::mouseDoubleClickEvent(QMouseEvent*){
    menu->editColor(QColor(text()));
}

void DoubleLabel::setColor(QColor color) {
    setText(color.name(QColor::HexArgb).toUpper());
}

void DoubleLabel::copyColorSlot() {
    /* copies the right clicked item's color value to the clipboard */
    menu->sendToClipboard(text());
}

void DoubleLabel::colorEditedSlot(QColor color) {
    emit colorChanged(color);
}

void DoubleLabel::mousePressEvent(QMouseEvent* event) {
    /* handles mouse click events */
    if (event->button() == Qt::RightButton) {
        menu->drawMenu(mapToGlobal(event->pos()), QColor(text()));
        event->accept();
    } else if (event->button() == Qt::LeftButton) {
        QLabel::mousePressEvent(event);
        event->accept();
    }
}