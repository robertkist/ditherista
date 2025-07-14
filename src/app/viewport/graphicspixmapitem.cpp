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

GraphicsPixmapItem::GraphicsPixmapItem(const QPixmap& p) : QGraphicsPixmapItem(p) {
    setAcceptedMouseButtons(Qt::LeftButton);
}

void GraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QDrag* dragOp = new QDrag(event->widget());
    QGraphicsView *view = static_cast<QGraphicsView *>(event->widget());
    view->setAcceptDrops(false);  // prevent circular image imports
    // create temp file (the _XXXXXX.png postfix is intentional!)
    const QString fileNameTemplate = QDir::tempPath() + QDir::separator() + data(0).toString() + "_XXXXXX.png";
    QTemporaryFile tempFile;
    tempFile.setFileTemplate(fileNameTemplate);
    tempFile.setAutoRemove(false);
    tempFile.open();
    const QString tempFileName = tempFile.fileName();
    // save image to temp file
    QImageWriter writer;
    writer.setFileName(tempFileName);
    if(writer.canWrite()) {
        const QImage image(pixmap().toImage().convertToFormat(QImage::Format_ARGB32));
        writer.write(image);
        if(writer.error() == 0) {
            // prepare mime data
            QList <QUrl> urlList;
            urlList << QUrl::fromLocalFile(tempFileName);
            QMimeData* mimeData = new QMimeData; // TODO we should free this once the file has been dropped?
            mimeData->setUrls(urlList);
            // create drag object
            dragOp->setMimeData(mimeData);
            dragOp->setHotSpot(QPoint(15, 15));
            dragOp->exec(Qt::CopyAction);  // execute drag operation
            emit tempFileCreated(tempFileName);
        } else if(QFile::exists(tempFileName)) {  // clean up if an error happens and a file exists
            QFile::remove(tempFileName);
        }
    }
    view->setAcceptDrops(true);
}

void GraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    event->accept();
}

void GraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    event->accept();
}
