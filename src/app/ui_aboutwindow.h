/********************************************************************************
** Form generated from reading UI file 'aboutwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTWINDOW_H
#define UI_ABOUTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AboutWindow
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *iconLabel;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QLabel *appTitle;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_3;
    QLabel *versionLabel;
    QLabel *label_6;
    QLabel *authorLabel;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_5;
    QLabel *linkLabel;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_9;
    QLabel *label_4;
    QLabel *libditherVersion;
    QSpacerItem *horizontalSpacer_10;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_12;
    QLabel *label_7;
    QLabel *qtVersion;
    QSpacerItem *horizontalSpacer_11;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *verticalSpacer_2;
    QTextEdit *licenseLabel;

    void setupUi(QDialog *AboutWindow)
    {
        if (AboutWindow->objectName().isEmpty())
            AboutWindow->setObjectName("AboutWindow");
        AboutWindow->resize(350, 438);
        gridLayout = new QGridLayout(AboutWindow);
        gridLayout->setObjectName("gridLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        iconLabel = new QLabel(AboutWindow);
        iconLabel->setObjectName("iconLabel");
        iconLabel->setMinimumSize(QSize(64, 64));
        iconLabel->setMaximumSize(QSize(64, 64));

        horizontalLayout->addWidget(iconLabel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 8, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        appTitle = new QLabel(AboutWindow);
        appTitle->setObjectName("appTitle");
        appTitle->setMinimumSize(QSize(0, 22));
        QFont font;
        font.setBold(true);
        appTitle->setFont(font);
        appTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(appTitle);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        label_3 = new QLabel(AboutWindow);
        label_3->setObjectName("label_3");

        horizontalLayout_2->addWidget(label_3);

        versionLabel = new QLabel(AboutWindow);
        versionLabel->setObjectName("versionLabel");
        versionLabel->setMinimumSize(QSize(0, 22));

        horizontalLayout_2->addWidget(versionLabel);

        label_6 = new QLabel(AboutWindow);
        label_6->setObjectName("label_6");

        horizontalLayout_2->addWidget(label_6);

        authorLabel = new QLabel(AboutWindow);
        authorLabel->setObjectName("authorLabel");
        authorLabel->setMinimumSize(QSize(0, 22));

        horizontalLayout_2->addWidget(authorLabel);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        linkLabel = new QLabel(AboutWindow);
        linkLabel->setObjectName("linkLabel");
        linkLabel->setOpenExternalLinks(true);

        horizontalLayout_3->addWidget(linkLabel);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_9);

        label_4 = new QLabel(AboutWindow);
        label_4->setObjectName("label_4");
        label_4->setMinimumSize(QSize(0, 30));
        label_4->setAlignment(Qt::AlignmentFlag::AlignBottom|Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft);

        horizontalLayout_6->addWidget(label_4);

        libditherVersion = new QLabel(AboutWindow);
        libditherVersion->setObjectName("libditherVersion");
        libditherVersion->setMinimumSize(QSize(0, 30));
        libditherVersion->setAlignment(Qt::AlignmentFlag::AlignBottom|Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft);

        horizontalLayout_6->addWidget(libditherVersion);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_10);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_12);

        label_7 = new QLabel(AboutWindow);
        label_7->setObjectName("label_7");
        label_7->setAlignment(Qt::AlignmentFlag::AlignBottom|Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft);

        horizontalLayout_5->addWidget(label_7);

        qtVersion = new QLabel(AboutWindow);
        qtVersion->setObjectName("qtVersion");
        qtVersion->setAlignment(Qt::AlignmentFlag::AlignBottom|Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft);

        horizontalLayout_5->addWidget(qtVersion);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_11);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        label = new QLabel(AboutWindow);
        label->setObjectName("label");
        label->setMinimumSize(QSize(0, 30));
        label->setAlignment(Qt::AlignmentFlag::AlignBottom|Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft);

        horizontalLayout_4->addWidget(label);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);


        verticalLayout->addLayout(horizontalLayout_4);

        verticalSpacer_2 = new QSpacerItem(20, 8, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        licenseLabel = new QTextEdit(AboutWindow);
        licenseLabel->setObjectName("licenseLabel");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Courier New")});
        licenseLabel->setFont(font1);
        licenseLabel->setMouseTracking(false);
        licenseLabel->setAcceptDrops(false);
        licenseLabel->setFrameShadow(QFrame::Shadow::Plain);
        licenseLabel->setLineWidth(0);
        licenseLabel->setDocumentTitle(QString::fromUtf8(""));
        licenseLabel->setUndoRedoEnabled(false);
        licenseLabel->setLineWrapMode(QTextEdit::LineWrapMode::NoWrap);
        licenseLabel->setLineWrapColumnOrWidth(80);
        licenseLabel->setReadOnly(true);
        licenseLabel->setAcceptRichText(false);

        verticalLayout->addWidget(licenseLabel);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(AboutWindow);

        QMetaObject::connectSlotsByName(AboutWindow);
    } // setupUi

    void retranslateUi(QDialog *AboutWindow)
    {
        AboutWindow->setWindowTitle(QCoreApplication::translate("AboutWindow", "About Ditherista", nullptr));
        iconLabel->setText(QString());
        appTitle->setText(QCoreApplication::translate("AboutWindow", "APP_TITLE", nullptr));
        label_3->setText(QCoreApplication::translate("AboutWindow", "version ", nullptr));
        versionLabel->setText(QCoreApplication::translate("AboutWindow", "TextLabel", nullptr));
        label_6->setText(QCoreApplication::translate("AboutWindow", " created by ", nullptr));
        authorLabel->setText(QCoreApplication::translate("AboutWindow", "Robert Kist", nullptr));
        linkLabel->setText(QCoreApplication::translate("AboutWindow", "homepage_link", nullptr));
        label_4->setText(QCoreApplication::translate("AboutWindow", "libdither version: ", nullptr));
        libditherVersion->setText(QCoreApplication::translate("AboutWindow", "TextLabel", nullptr));
        label_7->setText(QCoreApplication::translate("AboutWindow", "Qt version: ", nullptr));
        qtVersion->setText(QCoreApplication::translate("AboutWindow", "TextLabel", nullptr));
        label->setText(QCoreApplication::translate("AboutWindow", "Credits and Licenses:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutWindow: public Ui_AboutWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTWINDOW_H
