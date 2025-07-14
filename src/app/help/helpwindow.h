#pragma once
#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include "../../../build/ui_helpwindow.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class HelpWindow; }
QT_END_NAMESPACE

class HelpWindow final : public QDialog {
    Q_OBJECT
public:
    /* methods */
    explicit HelpWindow(QWidget* parent = nullptr);
    ~HelpWindow() override;
public slots:
    void showHelp();
    void showChangelog();
protected:
    void showEvent(QShowEvent*) override;
private:
    /* attributes */
    Ui::HelpWindow* ui{};
    /* methods */
    QString loadResource(const QString &name);
};

#endif // HELPWINDOW_H
