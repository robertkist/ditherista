#pragma once
#ifndef MOUSEEVENTFILTER_H
#define MOUSEEVENTFILTER_H

#include <QObject>

class MouseEventFilter : public QObject {
    Q_OBJECT
protected:
    bool eventFilter(QObject* obj, QEvent* event);
};

#endif  // MOUSEEVENTFILTER_H

