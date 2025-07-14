#pragma once
#ifndef BATCHDITHERDIALOG_H
#define BATCHDITHERDIALOG_H

#include "ui_batchditherdialog.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class BatchDitherDialog; }
QT_END_NAMESPACE

class BatchDitherDialog final : public QDialog {
    Q_OBJECT
public:
    /* attributes */
    bool cancelled = true;
    QString sourcePath;
    QString targetPath;
    QString postfix;
    QString prefix;
    /* methods */
	explicit BatchDitherDialog(QWidget* parent = nullptr);
    int exec() override;
    void setDitherer(QString ditherer);
    QString showDirectoryDialog(const QString& path);
    ~BatchDitherDialog() override;
private:
    /* attributes */
    Ui::BatchDitherDialog* ui{};
private slots:
    void prefixCheckStateChangedSlot(Qt::CheckState state);
    void postfixCheckStateChangedSlot(Qt::CheckState state);
    void sourceBrowseButtonPressedSlot();
	void targetBrowseButtonPressedSlot();
	void startButtonPressedSlot();
};

#endif // BATCHDITHERDIALOG_H