#include "svg.h"
#include <QXmlStreamReader>
#include <QSvgRenderer>
#include <QPainter>

QString rgbToHexString(int r, int g, int b) {
    /* helper function which returns the color as hex string (without #) - e.g. "ff0000" for red */
    return QString("%1%2%3").arg(r, 2, 16, QChar('0')).arg(g, 2, 16, QChar('0')).arg(b, 2, 16, QChar('0'));
}

QPixmap* getColorDotSvg(QString hexColor, QWidget* parent) {
    /* returns a SVG with a circle, filled with the specified hex color (e.g. "ff0000") */
    QString svgCircle = QString("<svg enable-background=\"new 0 0 48 48\" viewBox=\"0 0 48 48\" xmlns=\"http://www.w3.org/2000/svg\"><circle cx=\"24\" cy=\"24\" fill=\"#%1\" r=\"22\"/></svg>").arg(hexColor);
    QXmlStreamReader xml(svgCircle);
    QSvgRenderer* renderer = new QSvgRenderer(&xml, parent); //this);
    QPixmap* pixmap = new QPixmap(renderer->defaultSize() * parent->devicePixelRatio());
    pixmap->fill(Qt::transparent);
    QPainter painter;
    painter.begin(pixmap);
    renderer->render(&painter);
    painter.end();
    pixmap->setDevicePixelRatio(parent->devicePixelRatio());
    return pixmap;
}
