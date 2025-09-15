#include "editcombobox.h"
#include <QAbstractItemView>
#include <QEvent>
#include <QLineEdit>
#include <QApplication>

void EditComboBox::showPopup() {
    /* moves the popup right above the combobox widget */
    int l = lineEdit()->geometry().height();
    QComboBox::showPopup();
    QWidget* popup = this->findChild<QFrame*>();
    popup->move(popup->x(), popup->y() - l - popup->height());
}

void EditComboBox::hidePopup() {
    /* work-around: LineEdit() sometimes steals focus from the ComboBox */
    QComboBox::hidePopup();
    emit currentComboIndexChanged(indexHelper);
}

void EditComboBox::highlightedSlot(int index) {
    /* work-around: LineEdit() sometimes steals focus from the ComboBox */
    indexHelper = index;
}

EditComboBox::EditComboBox(QWidget* parent) : QComboBox(parent) {
    /* Constructor */
    connect(this, SIGNAL(highlighted(int)), this, SLOT(highlightedSlot(int)));
}