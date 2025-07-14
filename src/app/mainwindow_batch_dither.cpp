#include "mainwindow.h"
#include "consts.h"
#include "batch/batchditherprogress.h"
#include "batch/fileexistsdialog.h"
#include "batch/ditherresults.h"
#include <QFileInfo>
#include <QDir>

/*****************************************
 * BATCH DITHERING                       *
 *****************************************/

void MainWindow::batchDitherRequestedSlot() {
    // TODO adjust this for the color ditherers!
    /* user requested a batch render from a tree widget item's context menu */
    if(isDithering)
        return; // it shouldn't be possible to trigger this while a dither is in process, but better be safe...
    // display batch conversion dialog
    QStringList dither_name_cased;
    QHash<DitherType, QString> ditherName = {
            {ALL, tr("Allebach ")},
            {DBS, tr("Direct Binary Search")},
            {DOT, tr("Dot Diffusion")},
            {ERR, tr("Error Diffusion")},
            {GRD, tr("Grid Dithering")},
            {LIP, tr("Dot Lippens Dithering")},
            {ORD, tr("Ordered Dithering")},
            {PAT, tr("Pattern Dithering")},
            {RIM, tr("Riemersma Dithering")},
            {THR, tr("Thresholding")},
            {VAR, tr("Variable Error Diffusion")}
    };
    QString dither_name = ditherName[current_dither_type] + " | " + ui->treeWidgetMono->currentItem()->text(0);
    if (batchDitherDialog->sourcePath.isEmpty()) {
        batchDitherDialog->sourcePath = currentDirectory;
        batchDitherDialog->targetPath = currentDirectory;
        batchDitherDialog->prefix = "dithered_";
    }
    batchDitherDialog->setDitherer(dither_name);
    batchDitherDialog->exec();
    if (!batchDitherDialog->cancelled) {
        // pre process file names
        const QString sourcePath = batchDitherDialog->sourcePath;
        const QString targetPath = batchDitherDialog->targetPath.isEmpty() ? sourcePath : batchDitherDialog->targetPath;
        // gather files
        QStringList dir = QDir(sourcePath).entryList(FILE_FILTERS, QDir::Readable | QDir::Files);
        QStringList inFiles;
        QStringList outFiles;
        for (const QString& file : dir) {
            QFileInfo fi(file);
            QString inFilePath = sourcePath + QDir::separator() + file;
            QString outFileName = batchDitherDialog->prefix + fi.baseName() + batchDitherDialog->postfix + "." + fi.completeSuffix();
            QString outFilePath = targetPath + QDir::separator() + outFileName;
            inFiles.append(inFilePath);
            outFiles.append(outFilePath);
        }
        // init and display progress dialog
        auto* progress = new BatchDitherProgress(this);
        if (inFiles.size() > 1) {
            progress->setModal(true);
            progress->show();
            progress->setMaximum(static_cast<int>(inFiles.size()) - 1);
        }
        // start dithering files...
        ReturnStatus overwriteMode = RS_CREATE;
        auto* resultsDialog = new DitherResults(this);
        for (int i = 0; i < inFiles.size(); ++i) {
            QApplication::processEvents();
            QString fileName = QFileInfo(inFiles[i]).fileName();
            const bool fileExists = QFile(outFiles[i]).exists();
            // if destination file exists and we're not skipping/overwriteing all, ask the user what to do...
            if (overwriteMode != RS_OVERWRITE_ALL && overwriteMode != RS_SKIP_ALL) {
                if (fileExists) {
                    auto* dialog = new FileExistsDialog(fileName, this);
                    dialog->exec();
                    overwriteMode = dialog->status;
                } else {
                    overwriteMode = RS_CREATE;
                }
            }
            // dither the image, or skip it, and add result to list of results...
            if (overwriteMode == RS_CREATE || overwriteMode == RS_OVERWRITE || overwriteMode == RS_OVERWRITE_ALL) {
                BatchDitherResult result = ditherSingleImage(inFiles[i], outFiles[i]);
                resultsDialog->addResult(fileName, result, overwriteMode);
                if (progress->cancelled) {  // progress dialog cancel button pressed
                    progress->close();
                    resultsDialog->exec(); // show summary dialog
                    return;
                }
            } else if (overwriteMode == RS_CANCEL) {  // cancel pressed in overwrite/skip/etc. dialog
                progress->close();
                resultsDialog->exec(); // show summary dialog
                return;
            } else {
                resultsDialog->addResult(fileName, DR_OK, overwriteMode);
            }
            progress->setValue(i);
            progress->setText(fileName);
        }
        progress->close();
        resultsDialog->exec(); // show summary dialog
    }
}

//BatchDitherResult MainWindow::ditherSingleImage(const QString& inFileName, const QString& outFileName) {
BatchDitherResult MainWindow::ditherSingleImage(const QString& , const QString& ) {
    /* This function dithers a single image and saves it to file.
       This function is part of the batch-dither functionality
     */
//    ImageHash imageHash;
//    QImageReader reader;
//    reader.setFileName(inFileName);
//    if(!reader.canRead()) {
//        qDebug() << "ERROR: Image read error:"<<reader.errorString();
//        return DR_IMAGE_READ_ERROR;
//    }
//    QImage image = reader.read();
//    if (reader.error() != 0) {
//        qDebug() << "ERROR: Image read error:"<<reader.errorString();
//        return DR_IMAGE_READ_ERROR;
//    }
//    if(image.width() * image.height() > IMAGE_MAX_SIZE) {
//        return DR_RESOLUTION_ERROR;
//    }
//    imageHashMono.setSourceImage(&image); // TODO why do we need to set the source here?
//
//    const double contrast = static_cast<double>(ui->contrastSliderMonoMono->value()) / 100.0;
//    const double brightness = static_cast<double>(ui->brightnessSliderMono->value()) / 100.0;
//    const double gamma = static_cast<double>(ui->gammaSliderMono->value()) / 100.0;
//
//    //    imageHashMono.adjustSource(gamma, brightness, contrast); // TODO reenable
//
//    const int width = imageHashMono.getSourceImage()->width;
//    const int height = imageHashMono.getSourceImage()->height;
//    uint8_t* out_buf = static_cast<uint8_t*>(calloc(width * height, sizeof(uint8_t)));
//    switch (current_dither_type) {
//        case ALL: ALL_dither(&imageHashMono, out_buf); break;
//        case GRD: GRD_dither(&imageHashMono, out_buf); break;
//        case DBS: DBS_dither(&imageHashMono, out_buf); break;
//        case THR: THR_dither(&imageHashMono, out_buf); break;
//        case DOT: DOT_dither(&imageHashMono, out_buf, current_sub_dither_type); break;
//        case ERR: ERR_dither(&imageHashMono, out_buf, current_sub_dither_type); break;
//        case LIP: LIP_dither(&imageHashMono, out_buf, current_sub_dither_type); break;
//        case ORD: ORD_dither(&imageHashMono, out_buf, current_sub_dither_type); break;
//        case PAT: PAT_dither(&imageHashMono, out_buf, current_sub_dither_type); break;
//        case RIM: RIM_dither(&imageHashMono, out_buf, current_sub_dither_type); break;
//        case VAR: VAR_dither(&imageHashMono, out_buf, current_sub_dither_type); break;
//        default: break;
//    }
//    imageHashMono.setImageFromDither(current_dither_number, out_buf);
//
//    const QImage imageOut(imageHashMono.getDitheredImage(current_dither_number)->convertToFormat(QImage::Format_RGB32));
//    QImageWriter writer;
//    writer.setFileName(outFileName);
//    if(!writer.canWrite()) {
//        qDebug() << "ERROR: Image write error:"<<writer.errorString();
//        return DR_IMAGE_WRITE_ERROR;
//    }
//    writer.write(imageOut);
//    if(writer.error() != 0) {
//        qDebug() << "ERROR: Image write error:"<<writer.errorString();
//        return DR_IMAGE_WRITE_ERROR;
//    }
//    free(out_buf);
    return DR_OK;
}