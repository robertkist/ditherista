#include "batchditherdialog.h"
#include "../ui_elements/messagebox.h"
#include <QFileDialog>
#include <QMessageBox>

BatchDitherDialog::BatchDitherDialog(QWidget* parent) : QDialog(parent), ui(new Ui::BatchDitherDialog) {
    /* Constructor */
    this->ui->setupUi(this);
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startButtonPressedSlot()));
    connect(ui->sourceBrowseButton, SIGNAL(clicked()), this, SLOT(sourceBrowseButtonPressedSlot()));
    connect(ui->targetBrowseButton, SIGNAL(clicked()), this, SLOT(targetBrowseButtonPressedSlot()));
    connect(ui->prefixCheck, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(prefixCheckStateChangedSlot(Qt::CheckState)));
    connect(ui->postfixCheck, SIGNAL(checkStateChanged(Qt::CheckState)), this, SLOT(postfixCheckStateChangedSlot(Qt::CheckState)));
    setResult(QDialog::Rejected);
}

BatchDitherDialog::~BatchDitherDialog() {
    /* Destructor - free the UI */
    delete ui;
}

int BatchDitherDialog::exec() {
    /* Displays the dialog */
    cancelled = true;
    ui->sourceEdit->setText(sourcePath);
    ui->targetEdit->setText(targetPath);
    ui->prefixEdit->setText(prefix);
    ui->prefixCheck->setChecked(!prefix.isEmpty());
    ui->postfixEdit->setText(postfix);
    ui->postfixCheck->setChecked(!postfix.isEmpty());
    return QDialog::exec();
}

QString BatchDitherDialog::showDirectoryDialog(const QString& path) {
    /* Shows a directory selection dialog */
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.setDirectory(path);
    if (dialog.exec()) {
        return dialog.selectedFiles().at(0);
    }
    return {};
}

void BatchDitherDialog::sourceBrowseButtonPressedSlot() {
    /* User pressed button to browse for a source directory */
    if (QString result = showDirectoryDialog(sourcePath); !result.isEmpty()) {
        ui->sourceEdit->setText(result);
    }
}

void BatchDitherDialog::targetBrowseButtonPressedSlot() {
    /* User pressed button to browse for a target directory */
    if (QString result = showDirectoryDialog(targetPath); !result.isEmpty()) {
        ui->targetEdit->setText(result);
    }
}

void BatchDitherDialog::startButtonPressedSlot() {
    /* User presses the button to start the batch dithering process */
    const QString spath = ui->sourceEdit->text().trimmed();
    const QString tpath = ui->targetEdit->text().trimmed();
    if (spath.isEmpty()) {
        showErrorBox(this, "Source directory is empty.");
        return;
    }
    if (!QDir(spath).exists()) {
        showErrorBox(this, "Source directory does not exist.");
        return;
    }
    if (tpath.isEmpty()) {
        const QString pref = ui->prefixEdit->text();
        const QString post = ui->postfixEdit->text();
        if (pref.isEmpty() && post.isEmpty()) {
            showErrorBox(this, "A prefix or suffix must be set\nwhen dithering in the same directory");
            return;
        }
    } else {
        if (!QDir(tpath).exists()) {
            showErrorBox(this, "Target directory does not exist.");
            return;
        }
    }
    sourcePath = QDir(QDir::toNativeSeparators(spath)).canonicalPath();
    targetPath = QDir(QDir::toNativeSeparators(tpath)).canonicalPath();
    if (ui->prefixCheck->isChecked()) {
    	prefix = ui->prefixEdit->text();
    }
    if (ui->postfixCheck->isChecked()) {
    	postfix = ui->postfixEdit->text();
    }
    cancelled = false;
    close();
}

void BatchDitherDialog::setDitherer(QString ditherer) {
    /* Displays the name of the currently active ditherer */
    ui->dithererNameLabel->setText(ditherer);
}

void BatchDitherDialog::prefixCheckStateChangedSlot(const Qt::CheckState state) {
    /* User toggles the use of a prefix */
    ui->prefixEdit->setEnabled(state == Qt::Checked);
}

void BatchDitherDialog::postfixCheckStateChangedSlot(const Qt::CheckState state) {
    /* User toggles the use of a postfix */
    ui->postfixEdit->setEnabled(state == Qt::Checked);
}