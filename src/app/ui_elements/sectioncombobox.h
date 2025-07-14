#pragma once
#ifndef SECTIONCOMBOBOX_H
#define SECTIONCOMBOBOX_H

#include <QComboBox>
#include <QPaintEvent>

class SectionComboBox final : public QComboBox {
    Q_OBJECT
public:
    /* methods */
    explicit SectionComboBox(QWidget* parent);
    void paintEvent(QPaintEvent* event) override;
};

#endif // SECTIONCOMBOBOX_H
