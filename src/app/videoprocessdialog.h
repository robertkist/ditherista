#pragma once
#ifndef VIDEOPROCESSDIALOG_H
#define VIDEOPROCESSDIALOG_H

#include <QDialog>

namespace Ui {
class VideoProcessDialog;
}

class VideoProcessDialog : public QDialog {
    Q_OBJECT

public:
    explicit VideoProcessDialog(QWidget *parent = nullptr);
    ~VideoProcessDialog() override;

    void setTotalFrames(int total);
    void setProgress(int current, int total);
    void setPreviewFrame(const QImage& frame);
    void setStatus(const QString& status);
    void showError(const QString& error);
    void processingComplete(bool success, const QString& message);

signals:
    void cancelRequested();

private slots:
    void onCancelClicked();

private:
    Ui::VideoProcessDialog *ui;
    bool processing = false;
};

#endif // VIDEOPROCESSDIALOG_H
