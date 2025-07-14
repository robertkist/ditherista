#include "ditherresults.h"
#include <QLabel>
#include <QTreeWidget>
#include <QShowEvent>

DitherResults::DitherResults(QWidget* parent) : QDialog(parent), ui(new Ui::DitherResults) {
    /* Constructor */
    this->ui->setupUi(this);
    ui->treeWidget->setSortingEnabled(false);
    ui->treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->treeWidget->setFocusPolicy(Qt::NoFocus);
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

DitherResults::~DitherResults() {
    /* Destructor */
    delete ui;
}

void DitherResults::addResult(const QString& file, BatchDitherResult result, ReturnStatus status) {
    /* Adds a dither result (success, error, skipped, etc.) to the results list which will be displayed in the dialog
     * We're using column 0 for a cheap sort where ok items are on top, errors are shown next, and skipped files last */
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(1, file);
    if (result == DR_OK) {
        switch (status) {
            case RS_SKIP:
            case RS_SKIP_ALL:
                item->setText(0, "Z");
                item->setIcon(0, QIcon(QPixmap(":/resources/img_notdone.svg")));
                item->setText(2, tr("skipped"));
                break;
            case RS_OVERWRITE:
            case RS_OVERWRITE_ALL:
                item->setText(0, "A");
                item->setIcon(0, QIcon(QPixmap(":/resources/img_status_ok.svg")));
                item->setText(2, tr("overwritten"));
                break;
            default:
                item->setText(0, "A");
                item->setIcon(0, QIcon(QPixmap(":/resources/img_status_ok.svg")));
                break;
        }
    } else {
        item->setText(0, "B");
        item->setIcon(0, QIcon(QPixmap(":/resources/img_status_error.svg")));
        item->setText(0, tr("error"));
        switch (result) {
            case DR_IMAGE_READ_ERROR: item->setText(2, tr("error: could not read input file (wrong format?)")); break;
            case DR_IMAGE_WRITE_ERROR: item->setText(2, tr("error: could not write output file")); break;
            case DR_RESOLUTION_ERROR: item->setText(2, tr("error: image too big (<4k)")); break;
            default: item->setText(2, tr("error: unknown error")); break;; // we should never trigger this...
        }
        errorCount++;
    }
    item->setData(0, Qt::ForegroundRole, QColor(Qt::transparent));
    ui->treeWidget->addTopLevelItem(item);
}

void DitherResults::showEvent(QShowEvent* event) {
    /* Displays the dialog and summarizes the results from the addResult() calls */
    switch (errorCount) {
        case 0: ui->resultText->setText(tr("no errors")); break;
        case 1: ui->resultText->setText(tr("1 error")); break;
        default: ui->resultText->setText(QString("%1 ").arg(errorCount) + tr("errors")); break;
    }
    ui->treeWidget->sortItems(0, Qt::AscendingOrder);
    ui->treeWidget->setColumnWidth(0, 56);
    ui->treeWidget->resizeColumnToContents(1);
    ui->treeWidget->resizeColumnToContents(2);
    ui->treeWidget->setSortingEnabled(true);
    event->accept();
}
