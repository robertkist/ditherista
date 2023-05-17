#include "modernredux/style.h"
#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include "libdither.h"
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QtGlobal>

AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent), ui(new Ui::AboutWindow) {
    ui->setupUi(this);
    setModernReduxStyleSheet(this);
    // set application icon
    const QImage i = QImage(":/resources/appicon.png");
    ui->iconLabel->setPixmap(QPixmap::fromImage(i.scaled(QSize(64, 64))));
    // set application version and homepage link
    QSettings settings(":/resources/about.ini", QSettings::IniFormat);
    ui->versionLabel->setText(settings.value("about/version").toString());
    QString homepage = settings.value("about/homepage").toString();
    ui->linkLabel->setText("<a href=\"" + homepage + "\">" + homepage + "</a>");
    appTitle = settings.value("about/title").toString();
    ui->appTitle->setText(appTitle);
    // load licenses file
    QFile file(":/resources/license.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString license = in.readAll();
    file.close();
    ui->licenseLabel->setText(license);
    // set Qt and libdither version
    ui->libditherVersion->setText(libdither_version());
    ui->qtVersion->setText(qVersion());
}

AboutWindow::~AboutWindow() {
    delete ui;
}
