#include "../../../build/ui_helpwindow.h"
#include "../modernredux/style.h"
#include "helpwindow.h"
#include <QFile>
#include <QTextCursor>

HelpWindow::HelpWindow(QWidget* parent) : QDialog(parent), ui(new Ui::HelpWindow) {
    /* Constructor */
    ui->setupUi(this);
    setModernReduxStyleSheet(this);
    ui->pageGeneral->insertHtml(loadResource(":/resources/help/help.html"));
    ui->pageMonoDithering->insertHtml(loadResource(":/resources/help/mono_dithering.html"));
    ui->pageColorDithering->insertHtml(loadResource(":/resources/help/color_dithering.html"));
    ui->pagePaletteSettings->insertHtml(loadResource(":/resources/help/palette.html"));
    ui->pageChangeLog->insertHtml(loadResource(":/resources/changelog.html"));
}

void HelpWindow::showEvent(QShowEvent*) {
    // move all text cursors to the top
    ui->pageGeneral->moveCursor (QTextCursor::Start);
    ui->pageMonoDithering->moveCursor (QTextCursor::Start);
    ui->pageColorDithering->moveCursor (QTextCursor::Start);
    ui->pagePaletteSettings->moveCursor (QTextCursor::Start);
    ui->pageChangeLog->moveCursor (QTextCursor::Start);
}

QString HelpWindow::loadResource(const QString& name){
    /* Loads a text based resource from the QRC file and returns it as string */
    QFile file(name);
    file.open(QFile::ReadOnly);
    QString html = file.readAll();
    file.close();
    return html;
}

void HelpWindow::showChangelog() {
    /* Loads and displays the change log */
    resize(920, 720);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1); // last tab
    show();
}

void HelpWindow::showHelp() {
    /* Loads and displays the help */
    resize(920, 720);
    ui->tabWidget->setCurrentIndex(0);
    show();
}

HelpWindow::~HelpWindow() {
    /* Destructor */
    delete ui;
}
