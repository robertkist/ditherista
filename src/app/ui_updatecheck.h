/********************************************************************************
** Form generated from reading UI file 'updatecheck.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPDATECHECK_H
#define UI_UPDATECHECK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UpdateCheck
{
public:
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *newVersionLabel;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *downloadButton;
    QSpacerItem *horizontalSpacer_2;
    QWidget *page_2;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QLabel *currentVersionLabel;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *closeButton;
    QSpacerItem *horizontalSpacer_4;
    QWidget *page_3;
    QGridLayout *gridLayout_4;
    QLabel *label_3;
    QWidget *page_4;
    QGridLayout *gridLayout_5;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *closeButton2;
    QSpacerItem *horizontalSpacer_6;

    void setupUi(QDialog *UpdateCheck)
    {
        if (UpdateCheck->objectName().isEmpty())
            UpdateCheck->setObjectName("UpdateCheck");
        UpdateCheck->resize(320, 114);
        UpdateCheck->setMinimumSize(QSize(320, 114));
        UpdateCheck->setMaximumSize(QSize(320, 114));
        UpdateCheck->setModal(true);
        gridLayout = new QGridLayout(UpdateCheck);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(UpdateCheck);
        stackedWidget->setObjectName("stackedWidget");
        page = new QWidget();
        page->setObjectName("page");
        gridLayout_2 = new QGridLayout(page);
        gridLayout_2->setObjectName("gridLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(page);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(label);

        newVersionLabel = new QLabel(page);
        newVersionLabel->setObjectName("newVersionLabel");
        newVersionLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(newVersionLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancelButton = new QPushButton(page);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setMinimumSize(QSize(88, 0));
        cancelButton->setMaximumSize(QSize(88, 16777215));

        horizontalLayout->addWidget(cancelButton);

        downloadButton = new QPushButton(page);
        downloadButton->setObjectName("downloadButton");
        downloadButton->setMinimumSize(QSize(88, 0));
        downloadButton->setMaximumSize(QSize(88, 16777215));

        horizontalLayout->addWidget(downloadButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        gridLayout_3 = new QGridLayout(page_2);
        gridLayout_3->setObjectName("gridLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_2 = new QLabel(page_2);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(label_2);

        currentVersionLabel = new QLabel(page_2);
        currentVersionLabel->setObjectName("currentVersionLabel");
        currentVersionLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(currentVersionLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        closeButton = new QPushButton(page_2);
        closeButton->setObjectName("closeButton");

        horizontalLayout_2->addWidget(closeButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout_2->addLayout(horizontalLayout_2);


        gridLayout_3->addLayout(verticalLayout_2, 0, 0, 1, 1);

        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        gridLayout_4 = new QGridLayout(page_3);
        gridLayout_4->setObjectName("gridLayout_4");
        label_3 = new QLabel(page_3);
        label_3->setObjectName("label_3");
        label_3->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_4->addWidget(label_3, 0, 0, 1, 1);

        stackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName("page_4");
        gridLayout_5 = new QGridLayout(page_4);
        gridLayout_5->setObjectName("gridLayout_5");
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_4 = new QLabel(page_4);
        label_4->setObjectName("label_4");
        label_4->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_3->addWidget(label_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        closeButton2 = new QPushButton(page_4);
        closeButton2->setObjectName("closeButton2");

        horizontalLayout_3->addWidget(closeButton2);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);


        verticalLayout_3->addLayout(horizontalLayout_3);


        gridLayout_5->addLayout(verticalLayout_3, 0, 0, 1, 1);

        stackedWidget->addWidget(page_4);

        gridLayout->addWidget(stackedWidget, 0, 0, 1, 1);


        retranslateUi(UpdateCheck);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(UpdateCheck);
    } // setupUi

    void retranslateUi(QDialog *UpdateCheck)
    {
        UpdateCheck->setWindowTitle(QCoreApplication::translate("UpdateCheck", "Update Check", nullptr));
        label->setText(QCoreApplication::translate("UpdateCheck", "A newer version is available!", nullptr));
        newVersionLabel->setText(QCoreApplication::translate("UpdateCheck", "TextLabel", nullptr));
        cancelButton->setText(QCoreApplication::translate("UpdateCheck", "Cancel", nullptr));
        downloadButton->setText(QCoreApplication::translate("UpdateCheck", "Download", nullptr));
        label_2->setText(QCoreApplication::translate("UpdateCheck", "This version is the newest version.", nullptr));
        currentVersionLabel->setText(QCoreApplication::translate("UpdateCheck", "TextLabel", nullptr));
        closeButton->setText(QCoreApplication::translate("UpdateCheck", "Close", nullptr));
        label_3->setText(QCoreApplication::translate("UpdateCheck", "Checking for update. Please wait....", nullptr));
        label_4->setText(QCoreApplication::translate("UpdateCheck", "An error occurred while checking for updates.", nullptr));
        closeButton2->setText(QCoreApplication::translate("UpdateCheck", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UpdateCheck: public Ui_UpdateCheck {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPDATECHECK_H
