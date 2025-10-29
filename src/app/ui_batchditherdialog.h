/********************************************************************************
** Form generated from reading UI file 'batchditherdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BATCHDITHERDIALOG_H
#define UI_BATCHDITHERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_BatchDitherDialog
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QCheckBox *prefixCheck;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *targetEdit;
    QPushButton *targetBrowseButton;
    QLabel *label;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout;
    QLineEdit *sourceEdit;
    QPushButton *sourceBrowseButton;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *postfixEdit;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *prefixEdit;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *postfixCheck;
    QLabel *dithererNameLabel;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *cancelButton;
    QPushButton *startButton;

    void setupUi(QDialog *BatchDitherDialog)
    {
        if (BatchDitherDialog->objectName().isEmpty())
            BatchDitherDialog->setObjectName("BatchDitherDialog");
        BatchDitherDialog->resize(462, 248);
        BatchDitherDialog->setMinimumSize(QSize(462, 248));
        BatchDitherDialog->setMaximumSize(QSize(462, 248));
        BatchDitherDialog->setModal(true);
        gridLayout_2 = new QGridLayout(BatchDitherDialog);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        prefixCheck = new QCheckBox(BatchDitherDialog);
        prefixCheck->setObjectName("prefixCheck");
        prefixCheck->setMinimumSize(QSize(0, 32));
        prefixCheck->setMaximumSize(QSize(16777215, 32));

        gridLayout->addWidget(prefixCheck, 3, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        targetEdit = new QLineEdit(BatchDitherDialog);
        targetEdit->setObjectName("targetEdit");

        horizontalLayout_2->addWidget(targetEdit);

        targetBrowseButton = new QPushButton(BatchDitherDialog);
        targetBrowseButton->setObjectName("targetBrowseButton");
        targetBrowseButton->setMinimumSize(QSize(76, 0));
        targetBrowseButton->setMaximumSize(QSize(76, 16777215));

        horizontalLayout_2->addWidget(targetBrowseButton);


        gridLayout->addLayout(horizontalLayout_2, 2, 1, 1, 1);

        label = new QLabel(BatchDitherDialog);
        label->setObjectName("label");
        label->setMinimumSize(QSize(0, 26));
        label->setMaximumSize(QSize(16777215, 26));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_4 = new QLabel(BatchDitherDialog);
        label_4->setObjectName("label_4");
        label_4->setMinimumSize(QSize(0, 32));
        label_4->setMaximumSize(QSize(16777215, 32));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        sourceEdit = new QLineEdit(BatchDitherDialog);
        sourceEdit->setObjectName("sourceEdit");
        sourceEdit->setReadOnly(false);

        horizontalLayout->addWidget(sourceEdit);

        sourceBrowseButton = new QPushButton(BatchDitherDialog);
        sourceBrowseButton->setObjectName("sourceBrowseButton");
        sourceBrowseButton->setMinimumSize(QSize(76, 0));
        sourceBrowseButton->setMaximumSize(QSize(76, 16777215));

        horizontalLayout->addWidget(sourceBrowseButton);


        gridLayout->addLayout(horizontalLayout, 1, 1, 1, 1);

        label_3 = new QLabel(BatchDitherDialog);
        label_3->setObjectName("label_3");
        label_3->setMinimumSize(QSize(0, 32));
        label_3->setMaximumSize(QSize(16777215, 32));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        postfixEdit = new QLineEdit(BatchDitherDialog);
        postfixEdit->setObjectName("postfixEdit");
        postfixEdit->setEnabled(false);

        horizontalLayout_3->addWidget(postfixEdit);

        horizontalSpacer = new QSpacerItem(76, 20, QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout_3, 4, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        prefixEdit = new QLineEdit(BatchDitherDialog);
        prefixEdit->setObjectName("prefixEdit");
        prefixEdit->setEnabled(false);

        horizontalLayout_4->addWidget(prefixEdit);

        horizontalSpacer_2 = new QSpacerItem(76, 20, QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout_4, 3, 1, 1, 1);

        postfixCheck = new QCheckBox(BatchDitherDialog);
        postfixCheck->setObjectName("postfixCheck");
        postfixCheck->setMinimumSize(QSize(0, 32));
        postfixCheck->setMaximumSize(QSize(16777215, 32));

        gridLayout->addWidget(postfixCheck, 4, 0, 1, 1);

        dithererNameLabel = new QLabel(BatchDitherDialog);
        dithererNameLabel->setObjectName("dithererNameLabel");

        gridLayout->addWidget(dithererNameLabel, 0, 1, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);

        cancelButton = new QPushButton(BatchDitherDialog);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setMinimumSize(QSize(72, 0));
        cancelButton->setMaximumSize(QSize(72, 16777215));

        horizontalLayout_5->addWidget(cancelButton);

        startButton = new QPushButton(BatchDitherDialog);
        startButton->setObjectName("startButton");
        startButton->setMinimumSize(QSize(72, 0));
        startButton->setMaximumSize(QSize(72, 16777215));

        horizontalLayout_5->addWidget(startButton);


        gridLayout->addLayout(horizontalLayout_5, 5, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        QWidget::setTabOrder(sourceEdit, targetEdit);
        QWidget::setTabOrder(targetEdit, prefixCheck);
        QWidget::setTabOrder(prefixCheck, prefixEdit);
        QWidget::setTabOrder(prefixEdit, postfixCheck);
        QWidget::setTabOrder(postfixCheck, postfixEdit);
        QWidget::setTabOrder(postfixEdit, sourceBrowseButton);
        QWidget::setTabOrder(sourceBrowseButton, targetBrowseButton);
        QWidget::setTabOrder(targetBrowseButton, cancelButton);
        QWidget::setTabOrder(cancelButton, startButton);

        retranslateUi(BatchDitherDialog);

        QMetaObject::connectSlotsByName(BatchDitherDialog);
    } // setupUi

    void retranslateUi(QDialog *BatchDitherDialog)
    {
        BatchDitherDialog->setWindowTitle(QCoreApplication::translate("BatchDitherDialog", "Batch Convert", nullptr));
        prefixCheck->setText(QCoreApplication::translate("BatchDitherDialog", "File name prefix:", nullptr));
        targetBrowseButton->setText(QCoreApplication::translate("BatchDitherDialog", "Browse", nullptr));
        label->setText(QCoreApplication::translate("BatchDitherDialog", "Ditherer:", nullptr));
        label_4->setText(QCoreApplication::translate("BatchDitherDialog", "Target directory:", nullptr));
        sourceBrowseButton->setText(QCoreApplication::translate("BatchDitherDialog", "Browse", nullptr));
        label_3->setText(QCoreApplication::translate("BatchDitherDialog", "Source directory:", nullptr));
        postfixCheck->setText(QCoreApplication::translate("BatchDitherDialog", "File name suffix:", nullptr));
        dithererNameLabel->setText(QCoreApplication::translate("BatchDitherDialog", "TODO", nullptr));
        cancelButton->setText(QCoreApplication::translate("BatchDitherDialog", "Cancel", nullptr));
        startButton->setText(QCoreApplication::translate("BatchDitherDialog", "Start", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BatchDitherDialog: public Ui_BatchDitherDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BATCHDITHERDIALOG_H
