#include <QMouseEvent>
#include "doublepushbutton.h"
#include "svg.h"

DoublePushButton::DoublePushButton(QWidget* parent) : QPushButton(parent) {
    setFlat(true);
    setStyleSheet("QPushButton { background-color: transparent; border: 0px }");
    currentColor = QColor(0, 0, 0, 255);
    menu = new ColorCopyMenu(this);
    connect(menu, SIGNAL(copyColor()), this, SLOT(copyColorSlot()));
    connect(menu, SIGNAL(colorEdited(QColor)), this, SLOT(colorEditedSlot(QColor)));
}

DoublePushButton::~DoublePushButton() {
    delete menu;
}

void DoublePushButton::mousePressEvent(QMouseEvent* event) {
    /* handles mouse click events */
    if (event->button() == Qt::RightButton) {
        menu->drawMenu(mapToGlobal(event->pos()), currentColor);
        event->accept();
    } else if (event->button() == Qt::LeftButton) {
        QPushButton::mousePressEvent(event);
        event->accept();
    }
}

void DoublePushButton::setColor(QColor color) {
    /* sets the color swatch */
    currentColor = color;
    QPixmap* pixmap = getColorDotSvg(rgbToHexString(color.red(), color.green(), color.blue()), this);
    QIcon icon;
    icon.addPixmap(*pixmap, QIcon::Normal);
    setIcon(icon);
    delete pixmap;
}

void DoublePushButton::mouseDoubleClickEvent(QMouseEvent*){
    menu->editColor(currentColor);
}

void DoublePushButton::copyColorSlot() {
    /* copies the right clicked item's color value to the clipboard */
    menu->sendToClipboard(currentColor.name().toLower());
}

void DoublePushButton::colorEditedSlot(QColor color) {
    emit colorChanged(color);
}

QColor DoublePushButton::getColor() {
    /* returns the swatch's color */
    return currentColor;
}
