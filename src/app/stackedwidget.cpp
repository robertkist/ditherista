#include "stackedwidget.h"

StackedWidget::StackedWidget(QWidget* parent) : QWidget(parent), curr_index(0) {
    boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0, 0, 0, 0);
}

int StackedWidget::count() {
    return widgets.count();
}

void StackedWidget::addWidget(QWidget* w) {
    widgets.append(w);
    boxLayout->addWidget(w);
    showCurrentWidget();
}

QWidget* StackedWidget::currentWidget() {
    return widgets.at(curr_index);
}

void StackedWidget::setCurrentIndex(int i) {
    curr_index = i;
    showCurrentWidget();
}

void StackedWidget::showCurrentWidget() {
    if (widgets.count() > 0) {
        foreach (QWidget* widget, widgets)
            widget->hide();
        widgets.at(curr_index)->show();
        updateGeometry();
    }
}

QSize StackedWidget::sizeHint() {
    return currentWidget()->minimumSize();
}
