#pragma once
#ifndef MOUSEEVENTFILTER_H
#define MOUSEEVENTFILTER_H

/* an event-filter to ignore mouse events while the program is busy */

#include <QObject>
#include <QKeyEvent>

class MouseEventFilter final : public QObject {
    Q_OBJECT
signals:
    void keyEventSignal(QKeyEvent* event);
private:
    bool uiDisabled = false;
protected:
    /* methods */
    bool eventFilter(QObject* obj, QEvent* event) override;
public:
    void enableUi() { uiDisabled = false; };
    void disableUi() { uiDisabled = true; };
};

#endif  // MOUSEEVENTFILTER_H
