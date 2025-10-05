#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QFile>
#include <QFontDatabase>
#include <stdio.h>
#include "modernredux/style.h"

void setModernReduxStyleSheet(QWidget* window) {
    QFile file(":/modernredux/stylesheet.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = file.readAll();
    file.close();
    window->setStyleSheet(styleSheet);
}

void setModernRedux(QWidget* window) {
    QStringList font_list = {
        ":/modernredux/third_party/fonts/Roboto/Roboto-Black.ttf",
        ":/modernredux/third_party/fonts/Roboto/Roboto-BlackItalic.ttf",
        ":/modernredux/third_party/fonts/Roboto/Roboto-Bold.ttf",
        ":/modernredux/third_party/fonts/Roboto/Roboto-BoldItalic.ttf",
        ":/modernredux/third_party/fonts/Roboto/Roboto-Italic.ttf",
        ":/modernredux/third_party/fonts/Roboto/Roboto-Light.ttf",
        ":/modernredux/third_party/fonts/Roboto/Roboto-LightItalic.ttf",
        ":/modernredux/third_party/fonts/Roboto/Roboto-Medium.ttf",
        ":/modernredux/third_party/fonts/Roboto/Roboto-MediumItalic.ttf",
        ":/modernredux/third_party/fonts/Roboto/Roboto-Regular.ttf",
        ":/modernredux/third_party/fonts/Roboto/Roboto-Thin.ttf",
        ":/modernredux/third_party/fonts/Roboto/Roboto-ThinItalic.ttf"
    };
    for(int i = 0; i < font_list.count(); i++) {
        QFontDatabase::addApplicationFont(font_list[i]);
    }

    QPalette palette;
    palette.setColor(QPalette::Highlight, QColor("#2a82da"));
    // text
    palette.setColor(QPalette::WindowText, QColor("#e7e7e7"));
    palette.setColor(QPalette::BrightText, QColor("#e7e7e7"));
    palette.setColor(QPalette::ButtonText, QColor("#e7e7e7"));
    palette.setColor(QPalette::HighlightedText, QColor("#e7e7e7"));
    palette.setColor(QPalette::ToolTipText, QColor("#e7e7e7"));
    // hyperlinks
    palette.setColor(QPalette::LinkVisited, QColor("#505050"));
    palette.setColor(QPalette::Link, QColor("#2a82da"));
    // misc elements
    palette.setColor(QPalette::Button, QColor("#5c5c5c"));
    palette.setColor(QPalette::Light, QColor("#e7e7e7"));
    palette.setColor(QPalette::Midlight, QColor("#5a5a5a"));
    palette.setColor(QPalette::Dark, QColor("#232323"));
    palette.setColor(QPalette::Text, QColor("#e7e7e7"));
    palette.setColor(QPalette::Base, QColor("#2a2a2a"));
    palette.setColor(QPalette::Window, QColor("#353535"));
    palette.setColor(QPalette::Shadow, QColor("#141414"));
    palette.setColor(QPalette::AlternateBase, QColor("#424242"));
    palette.setColor(QPalette::ToolTipBase, QColor("#5a5a5a"));
    // disabled
    palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor("#7f7f7f"));
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor("#7f7f7f"));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#7f7f7f"));
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor("#7f7f7f"));
    palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor("#505050"));

    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication::setPalette(palette);
    setModernReduxStyleSheet(window);
}
