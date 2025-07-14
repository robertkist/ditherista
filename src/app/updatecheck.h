#pragma once
#ifndef UPDATECHECK_H
#define UPDATECHECK_H

#include "ui_updatecheck.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class UpdateCheck; }
QT_END_NAMESPACE

class UpdateCheck final : public QDialog {
    Q_OBJECT
public:
    /* methods */
    explicit UpdateCheck(QWidget* parent = nullptr);
    void showEvent(QShowEvent* event) override;
    void setUpdateAvailable(QString url, const QString &newVerison);
    void setUpdateToDate();
    void setNetworkError();
    QString getCurrentVersion();
    ~UpdateCheck() override;
private:
    /* attributes */
    QString url;
    QString currentVersion;
    Ui::UpdateCheck* ui{};
private slots:
    void downloadButtonClickedSlot();
};

#endif // UPDATECHECK_H