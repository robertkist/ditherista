#pragma once
#ifndef COLORCOPYMENU_H
#define COLORCOPYMENU_H

#include <QMenu>

class ColorCopyMenu final : public QObject {
    Q_OBJECT
signals:
    void copyColor();
    void colorEdited(QColor color);
public:
    explicit ColorCopyMenu(QWidget* parent=nullptr);
    ~ColorCopyMenu();
    void drawMenu(QPoint pos, QColor color);
    void sendToClipboard(QString color);
private:
    QMenu *menu;
    QAction* actionEditColor;
    QAction* actionCopyColor;
    QAction* actionPasteColor;
    QColor pastedColor;
    QColor currentColor;
public slots:
    void editColor(QColor color);
private slots:
    void copyColorSlot();
    void editColorSlot();
    void pasteColorSlot();
};

#endif // COLORCOPYMENU_H
