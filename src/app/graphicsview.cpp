#include "graphicsview.h"
#include <QFile>
#include <QMimeData>
#include <QFileInfo>

/* A GraphicsView class that supports dragging and dropping */

GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent) {
    setAcceptDrops(true);
    setScene(&scene);
    setStyleSheet("QAbstractScrollArea::corner {\n"
                  "    background: #2a2a2a;\n"
                  "    border: none;\n"
                  "}");
}

void GraphicsView::showSource(bool show) {
    out_pix_item->setVisible(!show);
    src_pix_item->setVisible(show);
}

void GraphicsView::setDitherImage(const QImage* img, QString partialFileName) {
    if(out_pix_item != nullptr) {
        disconnect(this, SLOT(tempFileCreatedSlot(QString)));
        deleteTempFiles();
    }
    QPixmap out_pix = QPixmap::fromImage(*img);
    out_pix_item = new GraphicsPixmapItem(out_pix);
    connect(out_pix_item, SIGNAL(tempFileCreated(QString)), this, SLOT(tempFileCreatedSlot(QString)));
    out_pix_item->setData(0, partialFileName);
    scene.addItem(out_pix_item);
}

void GraphicsView::deleteTempFiles() {
    for (int i = 0; i < tempFiles.size(); ++i)
        QFile::remove(tempFiles[i]);
    tempFiles.clear();
}

void GraphicsView::setSourceImage(const QImage* img) {
    QPixmap src_pix = QPixmap::fromImage(*img);
    src_pix_item = new QGraphicsPixmapItem(src_pix);
    scene.addItem(src_pix_item);
}

void GraphicsView::resetScene(int width, int height) {
    scene.setSceneRect(0, 0, width, height);
    scene.clear(); // clear scene and remove all QGraphicsItems
}

void GraphicsView::dropEvent(QDropEvent* event) {
    QStringList extensions = {"jpg", "jpeg", "png", "bmp", "gif"};
    const QMimeData* mimedata = event->mimeData();
    if(!mimedata->hasUrls())
        return;
    QList<QUrl> url = mimedata->urls();
    for(int i = 0; i < url.count(); i++) {
        if (!url[i].isValid())
            continue;
        if (url[i].scheme() != "file")
            continue;
        QString fileName = url[i].toLocalFile();
        QFileInfo fileInfo(fileName);
        for (int j = 0; j < extensions.count(); j++) {
            if (extensions[j] == fileInfo.suffix().toLower()) {
                emit loadImageSignal(fileName);
                event->accept();
                return;
            }
        }
    }
}
