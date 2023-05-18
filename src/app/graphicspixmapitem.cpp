#include "graphicspixmapitem.h"
#include <QCursor>
#include <QDrag>
#include <QGraphicsView>
#include <QMimeData>
#include <QUrl>
#include <QDir>
#include <QTemporaryFile>
#include <QImageWriter>

#include <QBuffer>


GraphicsPixmapItem::GraphicsPixmapItem(const QPixmap& p)
        : QGraphicsPixmapItem(p) {
    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void GraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QDrag* drag = new QDrag(event->widget());
    QGraphicsView *view = (QGraphicsView*)event->widget();
    view->setAcceptDrops(false);  // prevent circular image imports
    // create temp file
    QString fileNameTemplate = QDir::tempPath() + QDir::separator() + data(0).toString() + "_XXXXXX.png";
    QTemporaryFile tempFile;
    tempFile.setFileTemplate(fileNameTemplate);
    tempFile.setAutoRemove(false);
    tempFile.open();
    QString tempFileName = tempFile.fileName();
    // save image to temp file
    QImageWriter writer;
    writer.setFileName(tempFileName);
    if(writer.canWrite()) {
        QImage image(pixmap().toImage().convertToFormat(QImage::Format_RGB32));
        writer.write(image);
        if(writer.error() == 0) {
            // prepare mime data
            QList <QUrl> urlList;
            urlList << QUrl::fromLocalFile(tempFileName);
            QMimeData* mimeData = new QMimeData;
            mimeData->setUrls(urlList);

            // create drag object
            drag->setMimeData(mimeData);
            drag->setHotSpot(QPoint(15, 15));
            drag->exec(Qt::CopyAction);  // execute drag operation
            emit tempFileCreated(tempFileName);
        } else if(QFile::exists(tempFileName)) {  // clean up if an error happens and a file exists
            QFile::remove(tempFileName);
        }
    }
    // cleanup
    setCursor(Qt::OpenHandCursor);
    view->setAcceptDrops(true);
}

void GraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent*) {
    setCursor(Qt::ClosedHandCursor);
}

void GraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent*) {
    setCursor(Qt::OpenHandCursor);
}
