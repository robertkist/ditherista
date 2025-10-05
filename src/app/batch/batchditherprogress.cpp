#include "batchditherprogress.h"

// TODO add a delay - are the X of X numbers displayed properly?

BatchDitherProgress::BatchDitherProgress(QWidget* parent) : QDialog(parent), ui(new Ui::BatchDitherProgress) {
    /* Constructor */
    this->ui->setupUi(this);
    ui->progressBar->setValue(0);
    ui->progressBar->setMinimum(0);
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelSlot()));
}

BatchDitherProgress::~BatchDitherProgress() {
    /* Destructor */
    delete ui;
}

void BatchDitherProgress::setText(const QString& text) {
    /* Updates the dithering status: current file number (out of total) and current file name */
    const QString text1 = tr("converting")
        + QString("%1").arg(ui->progressBar->value()) + tr("of")
        + QString("%1").arg(ui->progressBar->maximum());
    const QFontMetrics metrics(ui->progressLabel->font());
    const int targetWidth = ui->progressLabel->width() - metrics.boundingRect(text1).width();
    const QString text2 = metrics.elidedText(text, Qt::ElideMiddle, targetWidth);
    ui->progressLabel->setText(text1 + text2);
}

void BatchDitherProgress::setMaximum(int maximum) {
    /* sets the total number of files to be dithered */
    ui->progressBar->setMaximum(maximum);
}

void BatchDitherProgress::setValue(int value) {
    /* sets the number of the current file being dithered */
    ui->progressBar->setValue(value);
}

void BatchDitherProgress::cancelSlot() {
    /* User clicked the cancel button */
    cancelled = true;
}