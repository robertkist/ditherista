#include "graphicsview.h"
#include "../consts.h"
#include <QFile>
#include <QMimeData>
#include <QFileInfo>

/* A GraphicsView class that supports dragging and dropping */

GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent) {
    /* Constructor */
    setAcceptDrops(true);
    setScene(&scene);
    setStyleSheet("QAbstractScrollArea::corner {\n"
                  "    background: #2a2a2a;\n"
                  "    border: none;\n"
                  "}");
}

void GraphicsView::showSourceMono(const bool show) const {
    /* shows the original imported mono source image */
    if (show) {
        if (out_pix_item_color != nullptr) out_pix_item_color->setVisible(false);
        if (out_pix_item_mono != nullptr) out_pix_item_mono->setVisible(false);
        src_pix_item_color->setVisible(false);
        src_pix_item_mono->setVisible(true);
    } else {
        if (out_pix_item_color != nullptr) out_pix_item_color->setVisible(false);
        if (out_pix_item_mono != nullptr) out_pix_item_mono->setVisible(true);
        src_pix_item_color->setVisible(false);
        src_pix_item_mono->setVisible(false);
    }
}

void GraphicsView::showSourceColor(const bool show) const {
    /* shows the original imported color source image */
    if (show) {
        if (out_pix_item_color != nullptr) out_pix_item_color->setVisible(false);
        if (out_pix_item_mono != nullptr) out_pix_item_mono->setVisible(false);
        src_pix_item_mono->setVisible(false);
        src_pix_item_color->setVisible(true);
    } else {
        if (out_pix_item_color != nullptr) out_pix_item_color->setVisible(true);
        if (out_pix_item_mono != nullptr) out_pix_item_mono->setVisible(false);
        src_pix_item_mono->setVisible(false);
        src_pix_item_color->setVisible(false);
    }
}

void GraphicsView::setDitherImageMono(const QImage* img, const QString& partialFileName) {
    if(out_pix_item_mono != nullptr) {
        disconnect(this, SLOT(tempFileCreatedSlot(QString)));
        deleteTempFiles();
        scene.removeItem(out_pix_item_mono);
    }
    const QPixmap out_pix = QPixmap::fromImage(*img);
    out_pix_item_mono = new GraphicsPixmapItem(out_pix);
    connect(out_pix_item_mono, SIGNAL(tempFileCreated(QString)), this, SLOT(tempFileCreatedSlot(QString)));
    out_pix_item_mono->setData(0, partialFileName);
    scene.addItem(out_pix_item_mono);
}

void GraphicsView::setDitherImageColor(const QImage* img, const QString& partialFileName) {
    if(out_pix_item_color != nullptr) {
        disconnect(this, SLOT(tempFileCreatedSlot(QString)));
        deleteTempFiles();
        scene.removeItem(out_pix_item_color);
    }
    const QPixmap out_pix = QPixmap::fromImage(*img);
    out_pix_item_color = new GraphicsPixmapItem(out_pix);
    connect(out_pix_item_color, SIGNAL(tempFileCreated(QString)), this, SLOT(tempFileCreatedSlot(QString)));
    out_pix_item_color->setData(0, partialFileName);
    scene.addItem(out_pix_item_color);
}

void GraphicsView::deleteTempFiles() {
    /* remove temporary file used when user drags an image out of the viewport */
    for (int i = 0; i < tempFiles.size(); ++i) {
        QFile::remove(tempFiles[i]);
    }
    tempFiles.clear();
}

void GraphicsView::setSourceImageMono(const QImage* img) {
    /* sets the original source image */
    const QPixmap pix = QPixmap::fromImage(*img);
    src_pix_item_mono = new QGraphicsPixmapItem(pix);
    scene.addItem(src_pix_item_mono);
}

void GraphicsView::setSourceImageColor(const QImage* img) {
    /* sets the original source image */
    const QPixmap pix = QPixmap::fromImage(*img);
    src_pix_item_color = new QGraphicsPixmapItem(pix);
    scene.addItem(src_pix_item_color);
}

void GraphicsView::resetScene(const int width, const int height) {
    /* resets and clears the GraphicsView */
    scene.setSceneRect(0, 0, width, height);
    scene.clear(); // clear scene and remove all QGraphicsItems
    zoomLevel = 100;
    emit zoomLevelChangedSignal(zoomLevel);
    out_pix_item_mono = nullptr;
    out_pix_item_color = nullptr;
    src_pix_item_mono = nullptr;
    src_pix_item_color = nullptr;
}

void GraphicsView::dropEvent(QDropEvent* event) {
    /* a file has been dropped onto the graphics view. Trigger import if file is a supported image */
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
        for (int j = 0; j < FILE_FILTERS.count(); j++) {
            if (QString suffix = "*." + fileInfo.suffix().toLower(); FILE_FILTERS[j] == suffix) {
                emit loadImageSignal(fileName);
                resetTransform();
                event->accept();
                return;
            }
        }
    }
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent* event) {
    /* handle mouse double click -> resets zoom level */
    resetTransform();
    zoomLevel = 100;
    emit zoomLevelChangedSignal(zoomLevel);
    event->accept();
}

void GraphicsView::wheelEvent(QWheelEvent* event) {
    /* zoom in / out when user uses mouse wheel */
    setFocus();
    if (event->angleDelta().y() > 0 && zoomLevel > MIN_ZOOM) {
        zoomLevel -= ZOOM_STEP_WHEEL;
        setZoomLevel(zoomLevel, true);
    } else if (event->angleDelta().y() < 0 && zoomLevel < MAX_ZOOM) {
        zoomLevel += ZOOM_STEP_WHEEL;
        setZoomLevel(zoomLevel, true);
    }
    event->accept();
};

int GraphicsView::getZoomLevel() {
    /* returns the current zoom level */
    return zoomLevel;
}

void GraphicsView::setZoomLevel(int level, bool update) {
    /* sets the zoom level of the view; emits a signal if 'update' is true */
    if (level >= MIN_ZOOM && level <= MAX_ZOOM) {
        zoomLevel = level;
        float zl = (float) level / 100.0f;
        setTransform(QTransform(zl, 0, 0, 0, zl, 0, 0, 0, 1));
        if (update) {
            emit zoomLevelChangedSignal(zoomLevel);
        }
    }
}

void GraphicsView::dragEnterEvent(QDragEnterEvent* event) {
    /* accepts drop events into the viewport */
    event->acceptProposedAction();
}

void GraphicsView::dragMoveEvent(QDragMoveEvent* event) {
    /* for dragging images into the viewport */
    event->acceptProposedAction();
}

void GraphicsView::tempFileCreatedSlot(const QString& fileName) {
    tempFiles<<fileName;
}

GraphicsView::~GraphicsView() {
    /* destructor */
    deleteTempFiles();
}