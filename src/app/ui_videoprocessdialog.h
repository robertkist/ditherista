/********************************************************************************
** Form generated from reading UI file 'videoprocessdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOPROCESSDIALOG_H
#define UI_VIDEOPROCESSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_VideoProcessDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *statusLabel;
    QLabel *previewLabel;
    QLabel *currentFrameLabel;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout;
    QLabel *totalFramesLabel;
    QSpacerItem *horizontalSpacer;
    QLabel *percentageLabel;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *cancelButton;

    void setupUi(QDialog *VideoProcessDialog)
    {
        if (VideoProcessDialog->objectName().isEmpty())
            VideoProcessDialog->setObjectName("VideoProcessDialog");
        VideoProcessDialog->resize(600, 450);
        verticalLayout = new QVBoxLayout(VideoProcessDialog);
        verticalLayout->setObjectName("verticalLayout");
        statusLabel = new QLabel(VideoProcessDialog);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(statusLabel);

        previewLabel = new QLabel(VideoProcessDialog);
        previewLabel->setObjectName("previewLabel");
        previewLabel->setMinimumSize(QSize(400, 300));
        previewLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(previewLabel);

        currentFrameLabel = new QLabel(VideoProcessDialog);
        currentFrameLabel->setObjectName("currentFrameLabel");
        currentFrameLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(currentFrameLabel);

        progressBar = new QProgressBar(VideoProcessDialog);
        progressBar->setObjectName("progressBar");
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        totalFramesLabel = new QLabel(VideoProcessDialog);
        totalFramesLabel->setObjectName("totalFramesLabel");

        horizontalLayout->addWidget(totalFramesLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        percentageLabel = new QLabel(VideoProcessDialog);
        percentageLabel->setObjectName("percentageLabel");

        horizontalLayout->addWidget(percentageLabel);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        cancelButton = new QPushButton(VideoProcessDialog);
        cancelButton->setObjectName("cancelButton");

        horizontalLayout_2->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(VideoProcessDialog);

        QMetaObject::connectSlotsByName(VideoProcessDialog);
    } // setupUi

    void retranslateUi(QDialog *VideoProcessDialog)
    {
        VideoProcessDialog->setWindowTitle(QCoreApplication::translate("VideoProcessDialog", "Processing Video", nullptr));
        statusLabel->setText(QCoreApplication::translate("VideoProcessDialog", "Processing video...", nullptr));
        previewLabel->setText(QCoreApplication::translate("VideoProcessDialog", "Preview", nullptr));
        previewLabel->setStyleSheet(QCoreApplication::translate("VideoProcessDialog", "QLabel { background-color: #1e1e1e; border: 1px solid #3e3e3e; }", nullptr));
        currentFrameLabel->setText(QCoreApplication::translate("VideoProcessDialog", "Frame 0 of 0", nullptr));
        totalFramesLabel->setText(QCoreApplication::translate("VideoProcessDialog", "Total frames: 0", nullptr));
        percentageLabel->setText(QCoreApplication::translate("VideoProcessDialog", "0%", nullptr));
        cancelButton->setText(QCoreApplication::translate("VideoProcessDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VideoProcessDialog: public Ui_VideoProcessDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOPROCESSDIALOG_H
