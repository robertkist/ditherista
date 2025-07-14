#include "stackedwidget.h"

StackedWidget::StackedWidget(QWidget* parent) : QWidget(parent), curr_index(0) {
    boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0, 0, 0, 0);
}

int StackedWidget::count() const {
    return static_cast<int>(widgets.count());
}

void StackedWidget::addWidget(QWidget* widget) {
    widgets.append(widget);
    boxLayout->addWidget(widget);
    showCurrentWidget();
}

QWidget* StackedWidget::currentWidget() const {
    return widgets.at(curr_index);
}

void StackedWidget::setCurrentIndex(const int i) {
    curr_index = i;
    showCurrentWidget();
}

void StackedWidget::showCurrentWidget() {
    if (widgets.count() > 0) {
        foreach (QWidget* widget, widgets) {
            widget->hide();
        }
        widgets.at(curr_index)->show();
        updateGeometry();
    }
}

QSize StackedWidget::sizeHint() const {
    return currentWidget()->minimumSize();
}

void StackedWidget::setAutoResize(const bool yes) {
    auto_resize = yes;
}