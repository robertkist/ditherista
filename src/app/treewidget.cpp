#include "treewidget.h"
#include "treewidgetdelegate.h"
#include <QKeyEvent>

// https://stackoverflow.com/questions/7175333/how-to-create-delegate-for-qtreewidget

/*
 * TreeWidget manages all info about the currently selected ditherer.
 * It provides the global number of the currently selected ditherer,
 * the selected ditherer's short identifier (e.g. ERR) and it long identifier (e.g. ERR_FFS)
 * and the suggested default file name part that includes the ditherer's name and type.
 */

TreeWidget::TreeWidget(QWidget* parent) : QTreeWidget(parent) {
    TreeWidgetDelegate* delegate = new TreeWidgetDelegate(this);
    setItemDelegate(delegate);
    connect(this, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(treeWidgetItemChangedSlot(QTreeWidgetItem*, int)));
}

QString TreeWidget::getCurrentDitherFileName() {
    QString fileName = ditherTypeName[currentDitherType];
    if(subDitherTypeName[currentSubDitherType] != "")
        fileName += QString("_") + subDitherTypeName[currentSubDitherType];
    fileName += "_dither";
    return fileName;
}

void TreeWidget::treeWidgetItemChangedSlot(QTreeWidgetItem* item, int) {
    TreeWidgetDelegate* id = (TreeWidgetDelegate*)itemDelegate();
    id->selected_row = item->data(ITEM_DATA_COUNT, Qt::UserRole).toInt();
    emit itemChangedSignal(item);
}

void TreeWidget::setItemActive(int index) {
    QTreeWidgetItem* item = topLevelItem(index);
    item->setSelected(true);
    setCurrentItem(item);
    currentDitherType = (enum DitherType)item->data(ITEM_DATA_DTYPE, Qt::UserRole).toInt();
    currentSubDitherType = (enum SubDitherType)item->data(ITEM_DATA_DSUBTYPE, Qt::UserRole).toInt();
    currentDitherNumber = item->data(ITEM_DATA_COUNT, Qt::UserRole).toInt();
}

QTreeWidgetItem* TreeWidget::addTreeItem(enum DitherType dt, enum SubDitherType num, QString title) {
    /* Adds ditherers to the TreeWidget GUI item */
    QIcon icon;
    switch(dt) {
        case ERR: icon = QIcon(":/resources/ico_error.svg"); break;
        case GRD: icon = QIcon(":/resources/ico_grid.svg"); break;
        case DOT: icon = QIcon(":/resources/ico_dot.svg"); break;
        case ORD: icon = QIcon(":/resources/ico_ordered.svg"); break;
        case LIP: icon = QIcon(":/resources/ico_dotlippens.svg"); break;
        case DBS: icon = QIcon(":/resources/ico_dbs.svg"); break;
        case ALL: icon = QIcon(":/resources/ico_allebach.svg"); break;
        case THR: icon = QIcon(":/resources/ico_threshold.svg"); break;
        case PAT: icon = QIcon(":/resources/ico_pattern.svg"); break;
        case RIM: icon = QIcon(":/resources/ico_riemersma.svg"); break;
        case VAR: icon = QIcon(":/resources/ico_variable.svg"); break;
    }
    static int item_count = 0;
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, title);
    item->setIcon(0, icon);
    item->setData(ITEM_DATA_DONE, Qt::UserRole, false);
    item->setData(ITEM_DATA_COUNT, Qt::UserRole, item_count);  // overall number of the ditherer
    item->setData(ITEM_DATA_DTYPE, Qt::UserRole, dt);   // dither type: e.g. error diffusion
    item->setData(ITEM_DATA_DSUBTYPE, Qt::UserRole, num);  // sub-type: e.g. floyd-steinberg
    addTopLevelItem(item);
    item_count++;
    return item;
}

void TreeWidget::clearAllDitherFlags() {
    for(int i = 0; i < topLevelItemCount(); i++) {
        topLevelItem(i)->setData(ITEM_DATA_DONE, Qt::UserRole, false);
    }
}

void TreeWidget::keyPressEvent(QKeyEvent* event) {
    if(event->key() == Qt::Key_Down){
        QTreeWidgetItem* item = itemBelow(currentItem());
        if(item != nullptr) {
            setCurrentItem(item);
            item->setSelected(true);
            emit itemPressed(item, 0);
        }
    } else if (event->key() == Qt::Key_Up) {
        QTreeWidgetItem* item = itemAbove(currentItem());
        if(item != nullptr) {
            setCurrentItem(item);
            item->setSelected(true);
            emit itemPressed(item, 0);
        }
    } else
        QTreeView::keyPressEvent(event);
    event->accept();
}
