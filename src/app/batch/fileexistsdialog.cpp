#include "fileexistsdialog.h"

FileExistsDialog::FileExistsDialog(QString fileName, QWidget* parent) : QDialog(parent), ui(new Ui::FileExistsDialog) {
    /* Constructor */
    this->ui->setupUi(this);
    ui->fileExistsLabel->setText(tr("File exists: ") + fileName);
    ui->fileExistsLabel->setToolTip(fileName);
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->skipButton, SIGNAL(clicked()), this, SLOT(skipSlot()));
    connect(ui->skipAllButton, SIGNAL(clicked()), this, SLOT(skipAllSlot()));
    connect(ui->overwriteButton, SIGNAL(clicked()), this, SLOT(overwriteSlot()));
    connect(ui->overwriteAllButton, SIGNAL(clicked()), this, SLOT(overwriteAllSlot()));
}

FileExistsDialog::~FileExistsDialog() {
    /* Destructor */
    delete ui;
}

void FileExistsDialog::skipSlot() {
    /* User pressed skip button - skips the current file */
    status = RS_SKIP;
    close();
}

void FileExistsDialog::skipAllSlot() {
    /* User pressed skip all button - skips all remaining already existing files */
    status = RS_SKIP_ALL;
    close();
}

void FileExistsDialog::overwriteSlot() {
    /* User pressed overwrite button - overwrites the currently existing target file */
    status = RS_OVERWRITE;
    close();
}

void FileExistsDialog::overwriteAllSlot() {
    /* User pressed overwrite all button - overwrites all existing target files */
    status = RS_OVERWRITE_ALL;
    close();
}