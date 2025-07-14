#pragma once
#ifndef FILEEXISTSDIALOG_H
#define FILEEXISTSDIALOG_H

#include "../enums.h"
#include "ui_fileexistsdialog.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class FileExistsDialog; }
QT_END_NAMESPACE

class FileExistsDialog final : public QDialog {
    Q_OBJECT
private:
    /* attributes */
    Ui::FileExistsDialog* ui{};
public:
    ReturnStatus status = RS_CANCEL; // TODO not needed? we might be able to get this from Qt directly
    /* methods */
    explicit FileExistsDialog(QString fileName, QWidget* parent = nullptr);
    ~FileExistsDialog() override;
private slots:
    void skipSlot();
    void skipAllSlot();
    void overwriteSlot();
    void overwriteAllSlot();
};

#endif // FILEEXISTSDIALOG_H