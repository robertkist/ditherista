/********************************************************************************
** Form generated from reading UI file 'ditherresults.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DITHERRESULTS_H
#define UI_DITHERRESULTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DitherResults
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *resultText;
    QSpacerItem *horizontalSpacer;
    QTreeWidget *treeWidget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *closeButton;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QDialog *DitherResults)
    {
        if (DitherResults->objectName().isEmpty())
            DitherResults->setObjectName("DitherResults");
        DitherResults->resize(522, 341);
        gridLayout = new QGridLayout(DitherResults);
        gridLayout->setObjectName("gridLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(DitherResults);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        resultText = new QLabel(DitherResults);
        resultText->setObjectName("resultText");

        horizontalLayout->addWidget(resultText);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        treeWidget = new QTreeWidget(DitherResults);
        treeWidget->setObjectName("treeWidget");
        treeWidget->setIndentation(10);

        verticalLayout->addWidget(treeWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        closeButton = new QPushButton(DitherResults);
        closeButton->setObjectName("closeButton");

        horizontalLayout_2->addWidget(closeButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(DitherResults);

        QMetaObject::connectSlotsByName(DitherResults);
    } // setupUi

    void retranslateUi(QDialog *DitherResults)
    {
        DitherResults->setWindowTitle(QCoreApplication::translate("DitherResults", "Batch Dither Results", nullptr));
        label->setText(QCoreApplication::translate("DitherResults", "Dithering Results:", nullptr));
        resultText->setText(QCoreApplication::translate("DitherResults", "No errors", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(2, QCoreApplication::translate("DitherResults", "Comment", nullptr));
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("DitherResults", "File", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("DitherResults", "Result", nullptr));
        closeButton->setText(QCoreApplication::translate("DitherResults", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DitherResults: public Ui_DitherResults {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DITHERRESULTS_H
