#pragma once
#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>
#include "ui_aboutwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AboutWindow; }
QT_END_NAMESPACE

class AboutWindow : public QDialog {
    Q_OBJECT
public:
    QString appTitle;
    AboutWindow(QWidget* parent = nullptr);
    ~AboutWindow();
private:
    QPixmap p;
    Ui::AboutWindow* ui;
};

#endif // ABOUTWINDOW_H
