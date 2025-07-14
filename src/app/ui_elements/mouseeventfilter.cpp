#include "mouseeventfilter.h"
#include <QEvent>
#include <QDebug>

bool MouseEventFilter::eventFilter(QObject* obj, QEvent* event) {
    /* https://stackoverflow.com/questions/6580226/qt-change-cursor-to-hourglass-and-disable-cursor */
    if (uiDisabled) { // UI is disabled -> ignore all events
        switch (event->type()) {
            case QEvent::KeyPress:
            case QEvent::KeyRelease:
            case QEvent::MouseButtonRelease:
            case QEvent::MouseButtonPress:
            case QEvent::MouseButtonDblClick:
            case QEvent::Gesture:
            case QEvent::DragEnter:
            case QEvent::DragMove:
            case QEvent::DragLeave:
            case QEvent::Drop:
                return true;
            default:
                break;
        }
    } else { // UI is enabled -> catch cursor up/down key presses
        switch (event->type()) {
            case QEvent::KeyPress: {
                QKeyEvent *keyEvent = (QKeyEvent *) event;
                if (keyEvent->key() == Qt::Key_Down || keyEvent->key() == Qt::Key_Up) {
                    emit keyEventSignal(keyEvent);
                    return true;
                }
            }
            default:
                break;
        }
    }
    return QObject::eventFilter(obj, event);
}