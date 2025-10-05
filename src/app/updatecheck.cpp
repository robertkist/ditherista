#include "updatecheck.h"
#include <QDesktopServices>
#include <QShowEvent>
#include <QSettings>

#define PAGE_UPDATE_AVAILABLE 0
#define PAGE_UP_TO_DATE 1
#define PAGE_CHECKING_FOR_UPDATE 2
#define PAGE_NETWORK_ERROR 3

UpdateCheck::UpdateCheck(QWidget* parent) : QDialog(parent), ui(new Ui::UpdateCheck) {
    /* Constructor */
    this->ui->setupUi(this);
    const QSettings settings(":/resources/about.ini", QSettings::IniFormat);
    currentVersion = settings.value("about/version").toString();
    ui->currentVersionLabel->setText(currentVersion);
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->closeButton2, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->downloadButton, SIGNAL(clicked()), this, SLOT(downloadButtonClickedSlot()));
}

UpdateCheck::~UpdateCheck() {
    /* Destructor */
    delete ui;
}

void UpdateCheck::setUpdateAvailable(QString url, const QString& newVerison) {
    /* Shows that an update to Ditherista is available */
    this->url = std::move(url);
    ui->newVersionLabel->setText(currentVersion + " → " + newVerison);
    ui->stackedWidget->setCurrentIndex(PAGE_UPDATE_AVAILABLE);
}

void UpdateCheck::downloadButtonClickedSlot() {
    /* User pressed the download button. This opens the download webpage in the user's default browser */
    QDesktopServices::openUrl(QUrl(url));
    close();
}

void UpdateCheck::showEvent(QShowEvent* event) {
    /* Qt's show event - ensures we're showing the right start page */
    ui->stackedWidget->setCurrentIndex(PAGE_CHECKING_FOR_UPDATE);
    event->accept();
}

void UpdateCheck::setUpdateToDate() {
    ui->stackedWidget->setCurrentIndex(PAGE_UP_TO_DATE);
}

void UpdateCheck::setNetworkError() {
    ui->stackedWidget->setCurrentIndex(PAGE_NETWORK_ERROR);
}

QString UpdateCheck::getCurrentVersion() {
    return currentVersion;
}
