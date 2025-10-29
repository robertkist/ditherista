#include "videoprocessdialog.h"
#include "ui_videoprocessdialog.h"
#include <QPushButton>

VideoProcessDialog::VideoProcessDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoProcessDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Processing Video"));
    setModal(true);
    
    connect(ui->cancelButton, &QPushButton::clicked, this, &VideoProcessDialog::onCancelClicked);
    
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
}

VideoProcessDialog::~VideoProcessDialog()
{
    delete ui;
}

void VideoProcessDialog::setTotalFrames(int total) {
    ui->progressBar->setMaximum(total);
    ui->totalFramesLabel->setText(tr("Total frames: %1").arg(total));
}

void VideoProcessDialog::setProgress(int current, int total) {
    ui->progressBar->setValue(current);
    ui->currentFrameLabel->setText(tr("Processing frame %1 of %2").arg(current).arg(total));
    
    // Calculate percentage
    if (total > 0) {
        int percentage = (current * 100) / total;
        ui->percentageLabel->setText(QString("%1%").arg(percentage));
    }
}

void VideoProcessDialog::setPreviewFrame(const QImage& frame) {
    if (!frame.isNull()) {
        QPixmap pixmap = QPixmap::fromImage(frame);
        // Scale to fit preview area while maintaining aspect ratio
        pixmap = pixmap.scaled(ui->previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->previewLabel->setPixmap(pixmap);
    }
}

void VideoProcessDialog::setStatus(const QString& status) {
    ui->statusLabel->setText(status);
}

void VideoProcessDialog::showError(const QString& error) {
    ui->statusLabel->setText("<font color='red'>" + tr("Error: ") + error + "</font>");
}

void VideoProcessDialog::processingComplete(bool success, const QString& message) {
    processing = false;
    ui->cancelButton->setText(tr("Close"));
    
    if (success) {
        ui->statusLabel->setText("<font color='green'>" + message + "</font>");
        ui->progressBar->setValue(ui->progressBar->maximum());
    } else {
        ui->statusLabel->setText("<font color='red'>" + message + "</font>");
    }
}

void VideoProcessDialog::onCancelClicked() {
    if (processing) {
        emit cancelRequested();
        ui->cancelButton->setEnabled(false);
        ui->statusLabel->setText(tr("Cancelling..."));
    } else {
        reject();
    }
}
