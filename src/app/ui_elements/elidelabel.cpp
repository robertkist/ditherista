#include "elidelabel.h"
#include <QPainter>
#include <QPaintEvent>

ElideLabel::ElideLabel(QWidget* parent) : QLabel(parent) {
    /* Constructor */
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void ElideLabel::setText(const QString& text) {
    /* Sets the label's display text */
    content = text;
}

void ElideLabel::paintEvent(QPaintEvent* event) {
    /* Qt's paint event - draws the label */
    event->accept();
    QPainter painter(this);
    const QFontMetrics fontMetrics = painter.fontMetrics();
    const QString elidedText = fontMetrics.elidedText(content, Qt::ElideRight, width());
    int ofs = (height() - fontMetrics.height()) / 2;
    painter.drawText(QPoint(0, ofs + fontMetrics.ascent()), elidedText);
}
