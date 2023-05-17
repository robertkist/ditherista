#pragma once
#ifndef NOTIFICATIONLABEL_H
#define NOTIFICATIONLABEL_H

#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTextDocument>

class NotificationLabel : public QLabel {
    Q_OBJECT
public:
    NotificationLabel(QWidget* parent);
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent*);
    void showText(QString text, unsigned long t);
    void cancel() { cancel_flag = true; };
private:
    QStringList textLines;
    QGraphicsOpacityEffect* fadeEffect;
    QPropertyAnimation* animation;
    QTextDocument doc;
    unsigned long startTime = 0;
    unsigned long timeout = 0.0;
    bool visible = false;
    bool cancel_flag = false;
    int textWidth = 0;

    void fadeIn();
    void fadeOut();
    void adjustPosition();
};

#endif // NOTIFICATIONLABEL_H
