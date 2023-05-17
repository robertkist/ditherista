#pragma once
#ifndef ELIDELABEL_H
#define ELIDELABEL_H

#include <QLabel>

class ElideLabel : public QLabel {
    Q_OBJECT
public:
    ElideLabel(QWidget* parent);
    void setText(QString text);
    void paintEvent(QPaintEvent*);
private:
    QString content;
};

#endif // ELIDELABEL_H
