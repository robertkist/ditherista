#include "colorcopymenu.h"
#include <QGuiApplication>
#include <QClipboard>
#include <QRegularExpression>
#include <QColorDialog>

ColorCopyMenu::ColorCopyMenu(QWidget* parent) : QObject(parent) {
    menu = new QMenu(parent);
    actionCopyColor = new QAction(tr("copy color"), this);
    connect(actionCopyColor, SIGNAL(triggered()), this, SLOT(copyColorSlot()));
    menu->addAction(actionCopyColor);
    actionPasteColor = new QAction(tr("paste color"), this);
    connect(actionPasteColor, SIGNAL(triggered()), this, SLOT(pasteColorSlot()));
    menu->addAction(actionPasteColor);
    actionEditColor = new QAction(tr("edit color"), this);
    connect(actionEditColor, SIGNAL(triggered()), this, SLOT(editColorSlot()));
    menu->addAction(actionEditColor);
}

void ColorCopyMenu::sendToClipboard(QString color) {
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(color.toLower());
}

ColorCopyMenu::~ColorCopyMenu() {
    delete menu;
}

void ColorCopyMenu::editColor(QColor color) {
    QColor result = QColorDialog::getColor(color, (QWidget*)parent());
    if (result.isValid()) {
        emit colorEdited(result);
    }
}

void ColorCopyMenu::copyColorSlot() {
    emit copyColor();
}

void ColorCopyMenu::editColorSlot() {
    editColor(currentColor);
}

void ColorCopyMenu::pasteColorSlot() {
    emit colorEdited(pastedColor);
}

void ColorCopyMenu::drawMenu(QPoint pos, QColor color) {
    currentColor = color;
    actionPasteColor->setVisible(false);
    QClipboard* clipboard = QGuiApplication::clipboard();
    QString clipboardText = clipboard->text(QClipboard::Clipboard);
    if (clipboardText.length() == 9 || clipboardText.length() == 7) {
        QRegularExpression re("^#(?:[0-9a-fA-F]{3,4}){2}$", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch match = re.match(clipboardText);
        if (match.hasMatch()) {
            actionPasteColor->setVisible(true);
            if (clipboardText.length() == 7) {
                clipboardText.remove(0, 1);
                clipboardText = QString("#ff") + clipboardText;
            }
            pastedColor = QColor(clipboardText.toLower());
        }
    }
    menu->popup(pos);
}
