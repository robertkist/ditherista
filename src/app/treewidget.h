#pragma once
#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include "enums.h"
#include <QTreeWidget>
#include <QString>
#include <QHash>

// dither type mapping for file-name generation for saving
const QHash<int, QString> ditherTypeName = {
        {ALL, "allebach"}, {DBS, "dbs"}, {DOT, "dotdiff"}, {ERR, "errordiff"},
        {LIP, "dotlippens"}, {ORD, "ordered"}, {PAT, "pattern"}, {RIM, "riemersma"},
        {THR, "threshold"}, {VAR, "variable"}};

const QHash<int, QString> subDitherTypeName = {
        {ALL_ALL, ""}, {DBS_DBS, ""}, {DOT_KNU, "knuth"}, {DOT_MKN, "mini-knuth"},
        {DOT_OKN, "optimized-knuth"}, {DOT_M08, "mese8x8"}, {DOT_M16, "mese16x16"},
        {DOT_G08, "guoliu8x8"}, {DOT_G16, "guoliu16x16"}, {ERR_XOT, "xot"}, {ERR_DIA, "diagonal"},
        {ERR_FFS, "floyd-steinberg"}, {ERR_SF3, "shiaufan3"}, {ERR_SF2, "shiaufan2"},
        {ERR_SF1, "shiaufan1"}, {ERR_STK, "stucki"}, {ERR_D1D, "1d"}, {ERR_D2D, "2d"},
        {ERR_FFS, "fake-floyd-steinberg"}, {ERR_JJN, "jarvis-judice-ninke"}, {ERR_ATK, "atkinson"},
        {ERR_BRK, "burkes"}, {ERR_SI3, "sierra3"}, {ERR_SI2, "sierra2row"}, {ERR_SIL, "sierra-lite"},
        {ERR_SPG, "steve-pigeon"}, {ERR_RKI, "robert-kist"}, {ERR_SSA, "stevenson-arce"},
        {LIP_LI1, "lippens1"}, {LIP_LI2, "lippens2"}, {LIP_LI3, "lippens3"}, {LIP_GUO, "guoliu16x16"},
        {LIP_MSE, "mese16x16"}, {LIP_KNU, "knuth"}, {ORD_BLU, "blue-noise"}, {ORD_BA2, "bayer2x2"},
        {ORD_BA3, "bayer3x3"}, {ORD_BA4, "bayer4x4"}, {ORD_BA8, "bayer8x8"}, {ORD_B16, "bayer16x16"},
        {ORD_B32, "bayer32x32"}, {ORD_DD1, "dispersed-dots1"}, {ORD_DD2, "dispersed-dots2"},
        {ORD_VDD, "void-dispersed-dots"}, {ORD_NR1, "non-Rectangular1"}, {ORD_NR2, "non-Rectangular2"},
        {ORD_NR3, "non-Rectangular3"}, {ORD_NR4, "non-Rectangular4"}, {ORD_ULY, "ulichney"},
        {ORD_UB5, "ulichney-bayer5x5"}, {ORD_CD1, "clustered-dot1"}, {ORD_CD2, "clustered-dot2"},
        {ORD_CD3, "clustered-dot3"}, {ORD_CD4, "clustered-dot4"}, {ORD_CD5, "clustered-dot5"},
        {ORD_CD6, "clustered-dot6"}, {ORD_CD7, "clustered-dot7"}, {ORD_CD8, "clustered-dot8"},
        {ORD_CD9, "clustered-dot9"}, {ORD_CDA, "clustered-dot10"}, {ORD_CDB, "clustered-dot11"},
        {ORD_CWP, "central-white-point"}, {ORD_BWP, "balanced-white-point"}, {ORD_DIO, "diagonal-ordered"},
        {ORD_UCD, "ulichney-cluster-dot"}, {ORD_M5C, "magic-5x5circle"}, {ORD_M6C, "magic-6x6circle"},
        {ORD_M7C, "magic-7x7circle"}, {ORD_M44, "magic-4x4-45deg"}, {ORD_M64, "magic-6x6-45deg"},
        {ORD_M84, "magic-8x8-45deg"}, {ORD_MA4, "magic-4x4"}, {ORD_MA6, "magic-6x6"}, {ORD_MA8, "magic-8x8"},
        {ORD_VA2, "variable2x2"}, {ORD_VA4, "variable4x4"}, {ORD_IGR, "interleaved-gradient"},
        {PAT_22A, "2x2-pattern"}, {PAT_33A, "3x3-pattern1"}, {PAT_33B, "3x3-pattern2"},
        {PAT_33C, "3x3-pattern3"}, {PAT_44A, "4x4-pattern"}, {PAT_52A, "5x2-pattern"},
        {RIM_HIL, "hilbert"}, {RIM_HIM, "hilbert-mod"}, {RIM_PEA, "peano"}, {RIM_FS0, "fass0"},
        {RIM_FS1, "fass1"}, {RIM_FS2, "fass2"}, {RIM_GOS, "gosper"}, {RIM_FSS, "fass-spiral"},
        {THR_THR, ""}, {VAR_OST, "ostromoukhov"}, {VAR_ZHF, "zhoufang"}};

const int ITEM_DATA_DONE = 0;
const int ITEM_DATA_COUNT = 1;
const int ITEM_DATA_DTYPE = 2;
const int ITEM_DATA_DSUBTYPE = 3;

class TreeWidget : public QTreeWidget {
    Q_OBJECT
signals:
    void itemChangedSignal(QTreeWidgetItem* item);
public:
    TreeWidget(QWidget*);
    void keyPressEvent(QKeyEvent* e);
    void clearAllDitherFlags();
    void setItemActive(int index);
    QTreeWidgetItem* addTreeItem(enum DitherType dt, enum SubDitherType num, QString title);
    QString getCurrentDitherFileName();
    void setCurrentItemDitherFlag(bool value) { currentItem()->setData(ITEM_DATA_DONE, Qt::UserRole, value); };
    void setValue(SubDitherType key1, SettingKey key2, QVariant value) { settings[key1][key2] = value; };
    QVariant getValue(SubDitherType key1, SettingKey key2) { return settings[key1][key2]; };

private:
    QHash<SubDitherType, QHash<SettingKey, QVariant>> settings;
    int currentDitherNumber;
    enum DitherType currentDitherType;
    enum SubDitherType currentSubDitherType;
private slots:
    void treeWidgetItemChangedSlot(QTreeWidgetItem* item, int);
};

#endif  // TREEWIDGET_H
