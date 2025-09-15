#pragma once
#ifndef EDITCOMBOBOX_H
#define EDITCOMBOBOX_H

#include <QComboBox>

class EditComboBox final : public QComboBox {
    Q_OBJECT
signals:
    void currentComboIndexChanged(int index);
public:
    /* methods */
    explicit EditComboBox(QWidget* parent);
    void showPopup();
    void hidePopup();
private:
    int indexHelper = 0;
private slots:
    void highlightedSlot(int index);
};

#endif // EDITCOMBOBOX_H
