#include "notificationlabel.h"
#include <time.h>
#include <QPainter>
#include <QTextOption>

const int FADE_IN_DURATION_MS = 150;
const int FADE_OUT_DURATION_MS = 350;
const int BOTTOM_MARGIN = 50;
const int VERT_PADDING = 20;
const int HOR_PADDING = 40;

NotificationLabel::NotificationLabel(QWidget* parent) : QLabel(parent) {
    setStyleSheet("background: #202020;"
                  "border-radius: 8px;"
                  "font-family: Roboto;"
                  "font-size: 18px;");
    setGeometry(0, 0, 205, 34);
    setAlignment(Qt::AlignCenter);
    fadeEffect = new QGraphicsOpacityEffect(this);
    fadeEffect->setOpacity(0.0);
    setGraphicsEffect(fadeEffect);
    animation = new QPropertyAnimation(fadeEffect, "opacity");
    setAttribute(Qt::WA_TransparentForMouseEvents);
    startTimer(100);
}

void NotificationLabel::fadeOut() {
    if(visible) {
        visible = false;
        fadeEffect->setOpacity(1.0);
        animation->setStartValue(1.0);
        animation->setEndValue(0.0);
        animation->setDuration(FADE_OUT_DURATION_MS);
        animation->start();
    } else
        fadeEffect->setOpacity(0.0);
}

void NotificationLabel::fadeIn() {
    if(!visible) {
        visible = true;
        fadeEffect->setOpacity(0.0);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->setDuration(FADE_IN_DURATION_MS);
        animation->start();
    } else
        fadeEffect->setOpacity(1.0);
}

void NotificationLabel::adjustPosition() {
    int pwh = (int)((float)((QWidget*)parent())->geometry().width() / 2.0);
    int ph = ((QWidget*)parent())->geometry().height();
    move(pwh - (int)((float)width() / 2.0), ph - height() - BOTTOM_MARGIN);
}

void NotificationLabel::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    QTextOption o;
    o.setAlignment(Qt::AlignHCenter);
    o.setWrapMode(QTextOption::NoWrap);
    QTextDocument td;
    td.setDefaultTextOption(o);
    td.setPageSize(QSize(textWidth, fontMetrics().lineSpacing()));
    td.setDefaultFont(font());
    td.setDocumentMargin(0);
    int ofs = VERT_PADDING - 1;
    painter.translate(QPointF(HOR_PADDING, ofs));
    for(int i = 0; i < textLines.count(); i++) {
        td.setHtml(textLines[i]);
        td.drawContents(&painter);
        painter.translate(QPointF(0, fontMetrics().lineSpacing()));
    }
}

void NotificationLabel::showText(QString text, unsigned long t) {
    cancel_flag = false;
    setText(text);
    startTime = (unsigned long)time(NULL);
    timeout = (unsigned long)((float)t / 1000.0);
    fadeIn();

    textLines = this->text().split("\n");
    textWidth = 0;
    for(int i = 0; i < textLines.count(); i++) {
        QTextDocument td;
        td.setHtml(textLines[i]);
        int w = fontMetrics().boundingRect(td.toPlainText()).width();
        if (w > textWidth)
            textWidth = w;
    }
    int height = (textLines.count() - 1) * fontMetrics().lineSpacing() + fontMetrics().ascent();
    setGeometry(pos().x(), pos().y(), textWidth + (HOR_PADDING * 2), height + (VERT_PADDING * 2));
    adjustPosition();
}

void NotificationLabel::timerEvent(QTimerEvent*) {
    if(visible) {
        unsigned long t = (unsigned long)time(NULL);
        if(t - startTime > timeout || cancel_flag)
            fadeOut();
    }
}
