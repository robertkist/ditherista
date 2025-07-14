#include "../modernredux/style.h"
#include "aboutwindow.h"
#include "../../../build/ui_aboutwindow.h"
#include "../../../libdither/src/libdither/libdither.h"
#include <QFile>
#include <QSettings>

AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent), ui(new Ui::AboutWindow) {
    /* Constructor - prepares the about dialog and loads license from resources */
    ui->setupUi(this);
    setModernReduxStyleSheet(this);

    // set application icon
    const QImage i = QImage(":/resources/appicon.png");
    ui->iconLabel->setPixmap(QPixmap::fromImage(i.scaled(QSize(64, 64))));

    // set application version and homepage link
    const QSettings settings(":/resources/about.ini", QSettings::IniFormat);
    ui->versionLabel->setText(settings.value("about/version").toString());
    const QString homepage = settings.value("about/homepage").toString();
    ui->linkLabel->setText("<a href=\"" + homepage + "\">" + homepage + "</a>");
    appTitle = settings.value("about/title").toString();
    ui->appTitle->setText(appTitle);

    // load licenses file
    QFile file(":/resources/license.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString license = in.readAll();
    file.close();
    license.replace("{{APP_YEAR}}", settings.value("about/year").toString());
    ui->licenseLabel->setText(license);

    // set Qt and libdither version
    ui->libditherVersion->setText(libdither_version());
    ui->qtVersion->setText(qVersion());
}

AboutWindow::~AboutWindow() {
    /* Destructor - free the UI */
    delete ui;
}
