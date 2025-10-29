/********************************************************************************
** Form generated from reading UI file 'helpwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELPWINDOW_H
#define UI_HELPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HelpWindow
{
public:
    QGridLayout *gridLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout;
    QTextEdit *pageGeneral;
    QWidget *tab_2;
    QGridLayout *gridLayout_5;
    QTextEdit *pageMonoDithering;
    QWidget *tab_3;
    QGridLayout *gridLayout_4;
    QTextEdit *pageColorDithering;
    QWidget *tab_4;
    QGridLayout *gridLayout_3;
    QTextEdit *pagePaletteSettings;
    QWidget *tab_5;
    QGridLayout *gridLayout_6;
    QTextEdit *pageChangeLog;

    void setupUi(QDialog *HelpWindow)
    {
        if (HelpWindow->objectName().isEmpty())
            HelpWindow->setObjectName("HelpWindow");
        HelpWindow->resize(690, 560);
        gridLayout_2 = new QGridLayout(HelpWindow);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(HelpWindow);
        tabWidget->setObjectName("tabWidget");
        tab = new QWidget();
        tab->setObjectName("tab");
        gridLayout = new QGridLayout(tab);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pageGeneral = new QTextEdit(tab);
        pageGeneral->setObjectName("pageGeneral");
        pageGeneral->setMouseTracking(false);
        pageGeneral->setAcceptDrops(false);
        pageGeneral->setFrameShadow(QFrame::Shadow::Plain);
        pageGeneral->setLineWidth(0);
        pageGeneral->setDocumentTitle(QString::fromUtf8(""));
        pageGeneral->setUndoRedoEnabled(false);
        pageGeneral->setReadOnly(true);

        gridLayout->addWidget(pageGeneral, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        gridLayout_5 = new QGridLayout(tab_2);
        gridLayout_5->setSpacing(0);
        gridLayout_5->setObjectName("gridLayout_5");
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        pageMonoDithering = new QTextEdit(tab_2);
        pageMonoDithering->setObjectName("pageMonoDithering");
        pageMonoDithering->setMouseTracking(false);
        pageMonoDithering->setAcceptDrops(false);
        pageMonoDithering->setFrameShadow(QFrame::Shadow::Plain);
        pageMonoDithering->setLineWidth(0);
        pageMonoDithering->setUndoRedoEnabled(false);
        pageMonoDithering->setReadOnly(true);

        gridLayout_5->addWidget(pageMonoDithering, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        gridLayout_4 = new QGridLayout(tab_3);
        gridLayout_4->setSpacing(0);
        gridLayout_4->setObjectName("gridLayout_4");
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        pageColorDithering = new QTextEdit(tab_3);
        pageColorDithering->setObjectName("pageColorDithering");
        pageColorDithering->setMouseTracking(false);
        pageColorDithering->setAcceptDrops(false);
        pageColorDithering->setFrameShadow(QFrame::Shadow::Plain);
        pageColorDithering->setLineWidth(0);
        pageColorDithering->setUndoRedoEnabled(false);
        pageColorDithering->setReadOnly(true);

        gridLayout_4->addWidget(pageColorDithering, 0, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        gridLayout_3 = new QGridLayout(tab_4);
        gridLayout_3->setSpacing(0);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        pagePaletteSettings = new QTextEdit(tab_4);
        pagePaletteSettings->setObjectName("pagePaletteSettings");
        pagePaletteSettings->setMouseTracking(false);
        pagePaletteSettings->setAcceptDrops(false);
        pagePaletteSettings->setFrameShadow(QFrame::Shadow::Plain);
        pagePaletteSettings->setLineWidth(0);
        pagePaletteSettings->setUndoRedoEnabled(false);
        pagePaletteSettings->setReadOnly(true);

        gridLayout_3->addWidget(pagePaletteSettings, 0, 0, 1, 1);

        tabWidget->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName("tab_5");
        gridLayout_6 = new QGridLayout(tab_5);
        gridLayout_6->setSpacing(0);
        gridLayout_6->setObjectName("gridLayout_6");
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        pageChangeLog = new QTextEdit(tab_5);
        pageChangeLog->setObjectName("pageChangeLog");
        pageChangeLog->setMouseTracking(false);
        pageChangeLog->setAcceptDrops(false);
        pageChangeLog->setFrameShadow(QFrame::Shadow::Plain);
        pageChangeLog->setLineWidth(0);
        pageChangeLog->setUndoRedoEnabled(false);
        pageChangeLog->setReadOnly(true);

        gridLayout_6->addWidget(pageChangeLog, 0, 0, 1, 1);

        tabWidget->addTab(tab_5, QString());

        gridLayout_2->addWidget(tabWidget, 0, 0, 1, 1);


        retranslateUi(HelpWindow);

        tabWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(HelpWindow);
    } // setupUi

    void retranslateUi(QDialog *HelpWindow)
    {
        HelpWindow->setWindowTitle(QCoreApplication::translate("HelpWindow", "Ditherista Help", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("HelpWindow", "General", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("HelpWindow", "Mono Dithering", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("HelpWindow", "Color Dithering", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("HelpWindow", "Palette Settings", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QCoreApplication::translate("HelpWindow", "Change Log", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HelpWindow: public Ui_HelpWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELPWINDOW_H
