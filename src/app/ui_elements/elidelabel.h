#pragma once
#ifndef ELIDELABEL_H
#define ELIDELABEL_H

/* A QLabel which elides its text (used for some labels in the .ui files) */

#include <QLabel>

class ElideLabel final : public QLabel {
    Q_OBJECT
public:
    /* methods */
    explicit ElideLabel(QWidget* parent);
    void setText(const QString& text);
    void paintEvent(QPaintEvent* event) override;
private:
    /* attributes */
    QString content;
};

#endif // ELIDELABEL_H
