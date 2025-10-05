#pragma once
#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include "enums.h"
#include <QTreeWidget>
#include <QMenu>

constexpr int ITEM_DATA_DONE = 0;
constexpr int ITEM_DATA_COUNT = 1;
constexpr int ITEM_DATA_DTYPE = 2;
constexpr int ITEM_DATA_DSUBTYPE = 3;

class TreeWidget final : public QTreeWidget {
    Q_OBJECT
signals:
    void itemChangedSignal(QTreeWidgetItem* item);
    void batchDitherSignal();
public:
    /* methods */
    explicit TreeWidget(QWidget*);
    void handleKeyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event) override;
    void clearAllDitherFlags() const;
    void setItemActive(int index);
    void changeDitherer(QTreeWidgetItem* item);
    QTreeWidgetItem* addTreeItem(DitherType dt, SubDitherType num, const QString &title);
    QString getCurrentDitherFileName() const;
    void setCurrentItemDitherFlag(bool value) const;
    void setValue(SubDitherType key1, SettingKey key2, const QVariant& value);
    QVariant getValue(SubDitherType key1, SettingKey key2);
private:
    /* attributes */
    QMenu* menu;
    QHash<SubDitherType, QHash<SettingKey, QVariant>> settings;
    int currentDitherNumber{};
    DitherType currentDitherType;
    SubDitherType currentSubDitherType;
    int item_count = 0;
    /* methods */
    void showContextMenuSlot(const QPoint& pos) const;
public slots:
    void treeWidgetItemChangedSlot(QTreeWidgetItem* item, int);
private slots:
//    void batchDitherRequestedSlot(); // TODO temporary disabled
};

#endif  // TREEWIDGET_H
