/********************************************************************************
** Form generated from reading UI file 'fileexistsdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEEXISTSDIALOG_H
#define UI_FILEEXISTSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FileExistsDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *fileExistsLabel;
    QHBoxLayout *horizontalLayout;
    QPushButton *skipButton;
    QPushButton *skipAllButton;
    QPushButton *overwriteButton;
    QPushButton *overwriteAllButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *FileExistsDialog)
    {
        if (FileExistsDialog->objectName().isEmpty())
            FileExistsDialog->setObjectName("FileExistsDialog");
        FileExistsDialog->resize(470, 82);
        gridLayout = new QGridLayout(FileExistsDialog);
        gridLayout->setObjectName("gridLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        fileExistsLabel = new QLabel(FileExistsDialog);
        fileExistsLabel->setObjectName("fileExistsLabel");
        fileExistsLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(fileExistsLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        skipButton = new QPushButton(FileExistsDialog);
        skipButton->setObjectName("skipButton");

        horizontalLayout->addWidget(skipButton);

        skipAllButton = new QPushButton(FileExistsDialog);
        skipAllButton->setObjectName("skipAllButton");

        horizontalLayout->addWidget(skipAllButton);

        overwriteButton = new QPushButton(FileExistsDialog);
        overwriteButton->setObjectName("overwriteButton");

        horizontalLayout->addWidget(overwriteButton);

        overwriteAllButton = new QPushButton(FileExistsDialog);
        overwriteAllButton->setObjectName("overwriteAllButton");

        horizontalLayout->addWidget(overwriteAllButton);

        cancelButton = new QPushButton(FileExistsDialog);
        cancelButton->setObjectName("cancelButton");

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(FileExistsDialog);

        QMetaObject::connectSlotsByName(FileExistsDialog);
    } // setupUi

    void retranslateUi(QDialog *FileExistsDialog)
    {
        FileExistsDialog->setWindowTitle(QCoreApplication::translate("FileExistsDialog", "File Exists", nullptr));
        fileExistsLabel->setText(QCoreApplication::translate("FileExistsDialog", "TextLabel", nullptr));
        skipButton->setText(QCoreApplication::translate("FileExistsDialog", "skip", nullptr));
        skipAllButton->setText(QCoreApplication::translate("FileExistsDialog", "skip all", nullptr));
        overwriteButton->setText(QCoreApplication::translate("FileExistsDialog", "overwrite", nullptr));
        overwriteAllButton->setText(QCoreApplication::translate("FileExistsDialog", "overwrite all", nullptr));
        cancelButton->setText(QCoreApplication::translate("FileExistsDialog", "cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileExistsDialog: public Ui_FileExistsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEEXISTSDIALOG_H
