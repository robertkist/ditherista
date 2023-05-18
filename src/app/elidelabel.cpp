#include "elidelabel.h"
#include <QPainter>

ElideLabel::ElideLabel(QWidget* parent) : QLabel(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void ElideLabel::setText(QString text) {
    content = text;
}

void ElideLabel::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QFontMetrics fontMetrics = painter.fontMetrics();
    QString elidedText = fontMetrics.elidedText(content, Qt::ElideRight, width());
    painter.drawText(QPoint(0, fontMetrics.ascent()), elidedText);
}
