#include "sectioncombobox.h"
#include <QPainter>

SectionComboBox::SectionComboBox(QWidget* parent) : QComboBox(parent) {}

void SectionComboBox::paintEvent(QPaintEvent* event) {
    event->accept();
    QPainter painter(this);
    QStyleOptionComboBox opt;
    initStyleOption(&opt); // This populates the style option with the current state
    // Draw the standard combo box elements first
    style()->drawComplexControl(QStyle::CC_ComboBox, &opt, &painter, this);
    // Draw the selected text
    QRect textRect = style()->subControlRect(QStyle::CC_ComboBox, &opt, QStyle::SC_ComboBoxEditField, this);
    QFontMetrics fm = fontMetrics();
    QString elidedText = fm.elidedText(itemText(currentIndex()), Qt::ElideRight, textRect.width());
    painter.drawText(textRect, Qt::AlignVCenter, elidedText);
}