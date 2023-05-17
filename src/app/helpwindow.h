#pragma once
#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>
#include "ui_helpwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HelpWindow; }
QT_END_NAMESPACE

class HelpWindow : public QDialog {
    Q_OBJECT
public:
    HelpWindow(QWidget* parent = nullptr);
    ~HelpWindow();
public slots:
    void showHelp();
    void showChangelog();
private:
    Ui::HelpWindow* ui;
    QString loadResource(QString name);
};

#endif // HELPWINDOW_H
