#include "mouseeventfilter.h"
#include <QEvent>

bool MouseEventFilter::eventFilter(QObject* obj, QEvent* event) {
    /* https://stackoverflow.com/questions/6580226/qt-change-cursor-to-hourglass-and-disable-cursor */
    switch(event->type()) {
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
        default: break;
    }
    return QObject::eventFilter(obj, event);
}