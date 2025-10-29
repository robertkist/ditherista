/********************************************************************************
** Form generated from reading UI file 'batchditherprogress.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BATCHDITHERPROGRESS_H
#define UI_BATCHDITHERPROGRESS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_BatchDitherProgress
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QProgressBar *progressBar;
    QLabel *progressLabel;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *cancelButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *BatchDitherProgress)
    {
        if (BatchDitherProgress->objectName().isEmpty())
            BatchDitherProgress->setObjectName("BatchDitherProgress");
        BatchDitherProgress->resize(341, 108);
        gridLayout = new QGridLayout(BatchDitherProgress);
        gridLayout->setObjectName("gridLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        progressBar = new QProgressBar(BatchDitherProgress);
        progressBar->setObjectName("progressBar");
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);

        progressLabel = new QLabel(BatchDitherProgress);
        progressLabel->setObjectName("progressLabel");
        progressLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(progressLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        cancelButton = new QPushButton(BatchDitherProgress);
        cancelButton->setObjectName("cancelButton");

        horizontalLayout->addWidget(cancelButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(BatchDitherProgress);

        QMetaObject::connectSlotsByName(BatchDitherProgress);
    } // setupUi

    void retranslateUi(QDialog *BatchDitherProgress)
    {
        BatchDitherProgress->setWindowTitle(QCoreApplication::translate("BatchDitherProgress", "Batch Convert Progress", nullptr));
        progressLabel->setText(QCoreApplication::translate("BatchDitherProgress", "converting 1 of 1223: foobarbaz.gif", nullptr));
        cancelButton->setText(QCoreApplication::translate("BatchDitherProgress", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BatchDitherProgress: public Ui_BatchDitherProgress {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BATCHDITHERPROGRESS_H
