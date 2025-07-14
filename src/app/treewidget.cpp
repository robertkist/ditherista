#include "treewidget.h"
#include "treewidgetdelegate.h"
#include <QKeyEvent>

/*
 * TreeWidget manages all info about the currently selected ditherer.
 * It provides the global number of the currently selected ditherer,
 * the selected ditherer's short identifier (e.g. ERR) and it long identifier (e.g. ERR_FFS)
 * and the suggested default file name part that includes the ditherer's name and type.
 */

TreeWidget::TreeWidget(QWidget* parent) : QTreeWidget(parent) {
    /* Constructor */
    TreeWidgetDelegate* delegate = new TreeWidgetDelegate(this);
    setItemDelegate(delegate);
    connect(this, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(treeWidgetItemChangedSlot(QTreeWidgetItem*, int)));

    // TODO temporary disabled (batch dithering)
//    // context menu setup
//    menu = new QMenu(this);
//    QAction* action1 = new QAction(tr("Batch dither..."), this);
//    connect(action1, SIGNAL(triggered()), this, SLOT(batchDitherRequestedSlot()));
//    menu->addAction(action1);
}

void TreeWidget::mousePressEvent(QMouseEvent* event) {
    /* Determines the clicking behavior:
       left click to select items
       right click on currently selected item brings up a context menu
    */
    if (event->button() == Qt::RightButton) {
        // TODO temporary disabled (batch dithering)
        // showContextMenuSlot(event->pos());
    } else if (event->button() == Qt::LeftButton) {
        QTreeWidget::mousePressEvent(event);
    }
    event->accept();
}

void TreeWidget::showContextMenuSlot(const QPoint& pos) const {
    /* User right clicked on a tileset -> show context menu */
    if (QTreeWidgetItem* rightClickItem = itemAt(pos); rightClickItem == currentItem()) {  // only trigger context menu at currently selected item
        menu->popup(mapToGlobal(pos));
    }
}

// TODO temporary disabled (batch dithering)
//void TreeWidget::batchDitherRequestedSlot() {
//    /* Gets the currently selected ditherer and initiates a batch render */
//    if (currentItem() != nullptr) {
//        emit batchDitherSignal();
//    }
//}

QString TreeWidget::getCurrentDitherFileName() const {
    /* Returns a human readable name of the currently selected ditherer */
    QString fileName = ditherTypeName[currentDitherType];
    if(!subDitherTypeName[currentSubDitherType].isEmpty()) {
        fileName += QString("_") + subDitherTypeName[currentSubDitherType];
    }
    fileName += "_dither";
    return fileName;
}

void TreeWidget::treeWidgetItemChangedSlot(QTreeWidgetItem* item, int) {
    /* User selected a different ditherer from the tree */
    changeDitherer(item);
    emit itemChangedSignal(item);
}

void TreeWidget::setItemActive(const int index) {
    /* Sets the specified ditherer as active and selected ditherer */
    QTreeWidgetItem* item = topLevelItem(index);
    item->setSelected(true);
    changeDitherer(item);
    setCurrentItem(item);
}

void TreeWidget::changeDitherer(QTreeWidgetItem* item) {
    /* shared functionality called from manually setting the active ditherer(setItemActive())
     * or via Slot(treeWidgetItemChangedSlot()) */
    TreeWidgetDelegate* id = static_cast<TreeWidgetDelegate*>(itemDelegate());
    id->selected_row = item->data(ITEM_DATA_COUNT, Qt::UserRole).toInt();
    currentDitherType = static_cast<DitherType>(item->data(ITEM_DATA_DTYPE, Qt::UserRole).toInt());
    currentSubDitherType = static_cast<SubDitherType>(item->data(ITEM_DATA_DSUBTYPE, Qt::UserRole).toInt());
    currentDitherNumber = item->data(ITEM_DATA_COUNT, Qt::UserRole).toInt();
}

QTreeWidgetItem* TreeWidget::addTreeItem(const DitherType dt, const SubDitherType num, const QString& title) {
    /* Adds ditherers to the TreeWidget GUI item */
    QIcon icon;
    switch(dt) {
        case ERR_C:
        case ERR: icon = QIcon(":/resources/ico_error.svg"); break;
        case GRD_C:
        case GRD: icon = QIcon(":/resources/ico_grid.svg"); break;
        case DOT_C:
        case DOT: icon = QIcon(":/resources/ico_dot.svg"); break;
        case ORD_C:
        case ORD: icon = QIcon(":/resources/ico_ordered.svg"); break;
        case LIP_C:
        case LIP: icon = QIcon(":/resources/ico_dotlippens.svg"); break;
        case DBS_C:
        case DBS: icon = QIcon(":/resources/ico_dbs.svg"); break;
        case ALL_C:
        case ALL: icon = QIcon(":/resources/ico_allebach.svg"); break;
        case THR_C:
        case THR: icon = QIcon(":/resources/ico_threshold.svg"); break;
        case PAT_C:
        case PAT: icon = QIcon(":/resources/ico_pattern.svg"); break;
        case RIM_C:
        case RIM: icon = QIcon(":/resources/ico_riemersma.svg"); break;
        case VAR_C:
        case VAR: icon = QIcon(":/resources/ico_variable.svg"); break;
    }
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, title);
    item->setIcon(0, icon);
    item->setData(ITEM_DATA_DONE, Qt::UserRole, false);
    item->setData(ITEM_DATA_COUNT, Qt::UserRole, item_count);  // overall number of the ditherer
    item->setData(ITEM_DATA_DTYPE, Qt::UserRole, dt);          // dither type: e.g. error diffusion
    item->setData(ITEM_DATA_DSUBTYPE, Qt::UserRole, num);      // sub-type: e.g. floyd-steinberg
    addTopLevelItem(item);
    item_count++;
    return item;
}

void TreeWidget::clearAllDitherFlags() const {
    /* Marks all dither slots as not having dithered anything yet. i.e. re-visiting the slot after the dither
       flag has been cleared will trigger a re-dither.
     */
    for(int i = 0; i < topLevelItemCount(); i++) {
        topLevelItem(i)->setData(ITEM_DATA_DONE, Qt::UserRole, false);
    }
}

void TreeWidget::handleKeyPressEvent(QKeyEvent* event) {
    /* Qt's key press event. Allows the user to use cursor keys to go to the next/previous ditherer in the list */
    if(event->key() == Qt::Key_Down){
        if(QTreeWidgetItem* item = itemBelow(currentItem()); item != nullptr) {
            setCurrentItem(item);
            item->setSelected(true);
            emit itemPressed(item, 0);
        }
    } else if (event->key() == Qt::Key_Up) {
        if(QTreeWidgetItem* item = itemAbove(currentItem()); item != nullptr) {
            setCurrentItem(item);
            item->setSelected(true);
            emit itemPressed(item, 0);
        }
    } else {
        QTreeView::keyPressEvent(event);
    }
    event->accept();
}

void TreeWidget::setCurrentItemDitherFlag(const bool value) const {
    currentItem()->setData(ITEM_DATA_DONE, Qt::UserRole, value);
}

void TreeWidget::setValue(const SubDitherType key1, const SettingKey key2, const QVariant& value) {
    settings[key1][key2] = value;
}

QVariant TreeWidget::getValue(const SubDitherType key1, const SettingKey key2) {
    return settings[key1][key2];
}
