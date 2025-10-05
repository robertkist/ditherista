#pragma once
#ifndef BATCHDITHERPROGRESS_H
#define BATCHDITHERPROGRESS_H

#include "ui_batchditherprogress.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class BatchDitherProgress; }
QT_END_NAMESPACE

class BatchDitherProgress final : public QDialog {
    Q_OBJECT
public:
    /* attributes */
    bool cancelled = false; // TODO not needed. we can get this info from Qt directly
    /* methods */
    explicit BatchDitherProgress(QWidget* parent = nullptr);
    void setMaximum(int maximum);
    void setValue(int value);
    void setText(const QString& text);
    ~BatchDitherProgress() override;
private:
    /* attributes */
    Ui::BatchDitherProgress* ui{};
private slots:
    void cancelSlot();
};

#endif // BATCHDITHERPROGRESS_H