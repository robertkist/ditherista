#pragma once
#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include "../../../build/ui_aboutwindow.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class AboutWindow; }
QT_END_NAMESPACE

class AboutWindow final : public QDialog {
    Q_OBJECT
public:
    /* attributes */
    QString appTitle;
    /* methods */
    explicit AboutWindow(QWidget* parent = nullptr);
    ~AboutWindow() override;
private:
    /* attributes */
    QPixmap p;
    Ui::AboutWindow* ui{};
};

#endif // ABOUTWINDOW_H
