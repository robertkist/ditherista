#pragma once
#ifndef DITHERISTA_SVG_H
#define DITHERISTA_SVG_H

/* helper to get circular color swatches as SVG */

#include <QWidget>
#include <QString>
#include <QPixmap>

QString rgbToHexString(int r, int g, int b);
QPixmap* getColorDotSvg(QString hexColor, QWidget* parent);

#endif //DITHERISTA_SVG_H
