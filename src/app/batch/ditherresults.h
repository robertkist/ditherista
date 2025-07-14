#pragma once
#ifndef DITHERRESULTS_H
#define DITHERRESULTS_H

#include "../enums.h"
#include "ui_ditherresults.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class DitherResults; }
QT_END_NAMESPACE

class DitherResults final : public QDialog {
    Q_OBJECT
public:
    /* methods */
    explicit DitherResults(QWidget* parent = nullptr);
    void showEvent(QShowEvent* event) override;
    void addResult(const QString& file, BatchDitherResult result, ReturnStatus status);
    ~DitherResults() override;
private:
    /* attributes */
    Ui::DitherResults* ui{};
    int errorCount = 0;
};

#endif // DITHERRESULTS_H