#include "mainwindow.h"
#include "modernredux/style.h"
#include "consts.h"
#include <QClipboard>
#include <QMimeData>

void MainWindow::runDitherThread() {
    /* executes a ditherer in a thread */
    isDithering = true;
    while(!fthread.isFinished()) {
        QThread::msleep(THREAD_SLEEP_DELAY_MS);
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents); // keep GUI thread alive while dithering
    }
    fthread.waitForFinished();
    isDithering = false;
}

/**************************************
 * MENU ACTIONS SLOTS - FILE I/O      *
 **************************************/

void MainWindow::fileOpenSlot() {
    /* shows a file open dialog and loads an image file from disk */
    QString fileName;
    if(fileManager.getOpenFileName(&fileName)) {
        loadImageFromFileSlot(fileName);
    }
}

void MainWindow::fileSaveAsSlot() {
    /* save current image under new name */
    fileSaveSlotImpl(true);
}

void MainWindow::fileSaveSlot() {
    /* save current image */
    fileSaveSlotImpl(false);
}

void MainWindow::fileSaveSlotImpl(const bool saveAs) {
    QString fileName;
    fileName = activeTreeWidget->getCurrentDitherFileName();
    if(!fileName.isEmpty()) {
        fileName = fileManager.fileSave(saveAs, fileName);
        if (!fileName.isEmpty()) {
            saveFile(fileName);
        }
    }
}

/**************************************
 * MENU ACTIONS SLOTS - COPY & PASTE  *
 **************************************/

void MainWindow::copySlot() const {
    /* copy current image to clipboard */
    if (isActiveWindow()) {
        QClipboard *clipboard = QGuiApplication::clipboard();
        QImage image;
        if (lastTabIndex == TAB_INDEX_MONO) {
            image = imageHashMono.getDitheredImage(current_dither_number)->convertToFormat(QImage::Format_ARGB32);
        } else {
            image = imageHashColor.getDitheredImage(current_dither_number)->convertToFormat(QImage::Format_ARGB32);
        }
        clipboard->setImage(image, QClipboard::Clipboard);
        notification->showText(tr("copied to clipboard"), 1000);
    }
}

void MainWindow::pasteSlot() {
    /* user pasted an image or a file name from the clipboard*/
    if (isActiveWindow()) {
        const QClipboard *clipboard = QGuiApplication::clipboard();
        if (const QMimeData *md = clipboard->mimeData(); md->hasUrls()) { // pasted content is a file name or path
            if (md->urls()[0].isLocalFile()) {
                loadImageFromFileSlot(md->urls()[0].toLocalFile());
            }
        } else { // pasted content is a supported image
            const QImage image = clipboard->image(QClipboard::Clipboard);
            if (!image.isNull()) {
                loadImage(&image);
            }
        }
    }
}
