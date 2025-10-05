#include "notificationlabel.h"
#include <QPainter>

constexpr int FADE_IN_DURATION_MS = 150;
constexpr int FADE_OUT_DURATION_MS = 350;
constexpr int BOTTOM_MARGIN = 50;
constexpr int VERT_PADDING = 20;
constexpr int HOR_PADDING = 40;

NotificationLabel::NotificationLabel(QWidget* parent) : QLabel(parent) {
    setStyleSheet("background: #202020;"
                  "border-radius: 8px;"
                  "font-family: Roboto;"
                  "font-size: 18px;");
    setGeometry(0, 0, 205, 34);
    setAlignment(Qt::AlignCenter);
    fadeEffect = new QGraphicsOpacityEffect(this);
    fadeEffect->setOpacity(0.0f);
    setGraphicsEffect(fadeEffect);
    animation = new QPropertyAnimation(fadeEffect, "opacity");
    setAttribute(Qt::WA_TransparentForMouseEvents);
    startTimer(100);
}

void NotificationLabel::fadeOut() {
    if(visible) {
        visible = false;
        fadeEffect->setOpacity(1.0f);
        animation->setStartValue(1.0f);
        animation->setEndValue(0.0f);
        animation->setDuration(FADE_OUT_DURATION_MS);
        animation->start();
    } else
        fadeEffect->setOpacity(0.0f);
}

void NotificationLabel::fadeIn() {
    if(!visible) {
        visible = true;
        fadeEffect->setOpacity(0.0f);
        animation->setStartValue(0.0f);
        animation->setEndValue(1.0f);
        animation->setDuration(FADE_IN_DURATION_MS);
        animation->start();
    } else
        fadeEffect->setOpacity(1.0f);
}

void NotificationLabel::adjustPosition() {
    const int pwh = static_cast<int>(static_cast<float>(static_cast<QWidget*>(parent())->geometry().width()) / 2.0f);
    const int ph = static_cast<QWidget*>(parent())->geometry().height();
    move(pwh - static_cast<int>(static_cast<float>(width()) / 2.0f), ph - height() - BOTTOM_MARGIN);
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
    const int ofs = VERT_PADDING - 1;
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
    startTime = static_cast<unsigned long>(time(nullptr));
    timeout = static_cast<unsigned long>(static_cast<float>(t) / 1000.0f);
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
    const int height = (textLines.count() - 1) * fontMetrics().lineSpacing() + fontMetrics().ascent();
    setGeometry(pos().x(), pos().y(), textWidth + HOR_PADDING * 2, height + VERT_PADDING * 2);
    adjustPosition();
}

void NotificationLabel::timerEvent(QTimerEvent*) {
    if(visible) {
        unsigned long t = static_cast<unsigned long>(time(nullptr));
        if(t - startTime > timeout || cancel_flag) {
            fadeOut();
        }
    }
}

bool NotificationLabel::isVisible() const {
    return visible;
}

void NotificationLabel::cancel() {
    cancel_flag = true;
}