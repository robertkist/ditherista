#include "modernredux/style.h"
#include "helpwindow.h"
#include "ui_helpwindow.h"
#include <QFile>

HelpWindow::HelpWindow(QWidget *parent) : QDialog(parent), ui(new Ui::HelpWindow) {
    ui->setupUi(this);
    setModernReduxStyleSheet(this);
}

QString HelpWindow::loadResource(QString name){
    QFile file(name);
    file.open(QFile::ReadOnly);
    QString html = file.readAll();
    file.close();
    return html;
}

void HelpWindow::showChangelog() {
    resize(500, 500);
    ui->content->clear();
    ui->content->insertHtml(loadResource(":/resources/changelog.html"));
    ui->content->moveCursor (QTextCursor::Start);
    setWindowTitle(tr("Changelog"));
    show();
}

void HelpWindow::showHelp() {
    resize(920, 720);
    ui->content->clear();
    ui->content->insertHtml(loadResource(":/resources/help/help.html"));
    ui->content->moveCursor (QTextCursor::Start);
    setWindowTitle(tr("Help"));
    show();
}

HelpWindow::~HelpWindow() {
    delete ui;
}
