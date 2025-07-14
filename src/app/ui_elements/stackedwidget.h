#pragma once
#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

/* a stacked widget that resizes to the sizes of its individual pages */

#include <QWidget>
#include <QVBoxLayout>

class StackedWidget final : public QWidget {
    Q_OBJECT
public:
    /* methods */
    explicit StackedWidget(QWidget* parent);
    void addWidget(QWidget* widget);
    int count() const;
    QWidget* currentWidget() const;
    void setAutoResize(bool yes);
    using QWidget::sizeHint;
    QSize sizeHint() const override;
    int currentIndex() { return curr_index; };
protected:
    /* methods */
    void showCurrentWidget();
private:
    /* attributes */
    bool auto_resize;
    int curr_index;
    QList<QWidget*> widgets;
    QVBoxLayout* boxLayout;
public slots:
    void setCurrentIndex(int);
};

#endif // STACKEDWIDGET_H
