#pragma once
#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class StackedWidget : public QWidget {
    Q_OBJECT
public:
    StackedWidget(QWidget*);
    void addWidget(QWidget*);
    int count();
    QWidget* currentWidget();
    void setAutoResize(bool yes) { auto_resize = yes; }
    using QWidget::sizeHint;
    QSize sizeHint();
protected:
    void showCurrentWidget();
private:
    bool auto_resize;
    int curr_index;
    QList<QWidget*> widgets;
    QVBoxLayout* boxLayout;
public slots:
    void setCurrentIndex(int);
};

#endif // STACKEDWIDGET_H
