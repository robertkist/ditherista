#pragma once
#ifndef NOTIFICATIONLABEL_H
#define NOTIFICATIONLABEL_H

#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

/* a label which fades in and out; used to display user notifications overlay over the graphicsview */

class NotificationLabel final : public QLabel {
    Q_OBJECT
public:
    /* methods */
    explicit NotificationLabel(QWidget* parent);
    void paintEvent(QPaintEvent*) override;
    void timerEvent(QTimerEvent*) override;
    void showText(QString text, unsigned long t);
    void cancel();
    [[nodiscard]] bool isVisible() const;
    void adjustPosition();
private:
    /* attributes */
    QStringList textLines;
    QGraphicsOpacityEffect* fadeEffect;
    QPropertyAnimation* animation;
    QTextDocument doc;
    unsigned long startTime = 0;
    unsigned long timeout = 0.0;
    bool visible = false;
    bool cancel_flag = false;
    int textWidth = 0;
    /* methods */
    void fadeIn();
    void fadeOut();
};

#endif // NOTIFICATIONLABEL_H
