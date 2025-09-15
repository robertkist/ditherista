#include "mainwindow.h"
#include "modernredux/style.h"
#include "treewidget.h"
#include "consts.h"
#include "ui_elements/svg.h"
#include "ui_elements/signalblocker.h"

#include <QClipboard>
#include <QMimeData>
#include <QtConcurrent>
#include <QImageWriter>
#include <QImageReader>
#include <QSvgRenderer>
#include <QMainWindow>
#include <QNetworkReply>
#include <QApplication>
#include <qevent.h>
#include <QPainter>
#include <QMenuBar>
#include <QIntValidator>
#include <QFileDialog>

void MainWindow::refreshUiColorDitherStatus(bool resetLab, bool updateSwatches) {
    /* refreshes all relevant UI elements when the color palette changes */
    imageHashColor.clearAllDitheredImages();
    ui->treeWidgetColor->clearAllDitherFlags();
    if (updateSwatches) {
        updatePaletteColorSwatches(cachedPalette->target_palette);
    }
    if (resetLab) {
        resetLabHCVspinBoxes();
    }
}

/*************************************************
 * CONSTRUCTOR / DESTRUCTOR / GUI INITIALIZATION *
 *************************************************/

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    /* Constructor */
    uiSetup();  // Qt GUI setup
    // Ditherista component setup
    fileManager.setParent(this);
    helpWindow = new HelpWindow(this);
    batchDitherDialog = new BatchDitherDialog(this);
    notification = new NotificationLabel(ui->graphicsView);
    updateCheck = new UpdateCheck(this);
    // populate widgets and set default values
    setDithererDefaults();           // default values for various ditherers
    addPredefinedPalettes();         // adds built in palettes (in resources/palettes) to the palette color combo
    populateColorComparisonCombo();  // e.g. if we use LAB94, sRGB, etc. comparison
    lastBuiltInPalette = QString(":/resources/palettes/%1.pal").arg(DEFAULT_PALETTE);
    populateDithererSelection();     // add color and mono ditherers to the list
    setApplicationDefaults();
    expandColorComparisonArea(false);
    connectSignals();
}

MainWindow::~MainWindow() {
    /* Destructor */
    CachedPalette_free(cachedPalette);
    BytePalette_free(customPalette);
    delete ui;
}

/****************************************************
 * GUI & EVENT SLOTS                                *
 * Major UI events: color/mono/palette tab changed, *
 *                  user picks a ditherer, etc.     *
 ****************************************************/

void MainWindow::editMenuAboutToShowSlot() {
    /* disable edit->paste menu item if clipboard doesn't contain something we can load */
    ui->actionPaste->setEnabled(false);
    const QClipboard* clipboard = QGuiApplication::clipboard();
    if(const QMimeData* md = clipboard->mimeData(); md->hasUrls()) {
        ui->actionPaste->setEnabled(true);
    } else if(!clipboard->image(QClipboard::Clipboard).isNull()) {
        ui->actionPaste->setEnabled(true);
    }
}

void MainWindow::tabWidgetChangedSlot(int index) {
    /* user changes to another tab: Mono, Color, Palette */
    // TODO we may not need the lastTabIndex anymore (is lastTabIndex redundant?)
    if (index < TAB_INDEX_PALETTE) {
        lastTabIndex = index; // holds dither tab was last active (color or mono)
        if (index == TAB_INDEX_MONO) {      // trigger a re-dither for the active ditherer in the tab we're switching to
            ui->imageSettingsStackedWidget->setCurrentIndex(0);
            ui->graphicsView->showSourceMono(ui->showOriginalMono->checkState() == Qt::Checked);
            activeTreeWidget = ui->treeWidgetMono;
        } else {
            ui->imageSettingsStackedWidget->setCurrentIndex(1);
            ui->graphicsView->showSourceColor(ui->showOriginalColor->checkState() == Qt::Checked);
            activeTreeWidget = ui->treeWidgetColor;
        }
        treeWidgetItemChangedSlot(activeTreeWidget->currentItem());
    } else {
        ui->ditherSettings->setCurrentIndex(0); // hide ditherer settings in palette tab
    }
}

void MainWindow::treeWidgetItemChangedSlot(QTreeWidgetItem* item) { // ignore clang-tidy, don't add const here!
    /* User clicked on a ditherer in the Mono ditherer list. This function updates the UI to reflect the current
     * displayed GUI settings for the selected ditherer, and then dithers the image accordingly.
     * NOTE: There is seemingly duplicate code here - but we allow this for the possibility that color ditherers
     *       settings may diverge from mono ditherer settings in the future.
     * */
    // TODO investigate: do we need current_dither_number/current_sub_dither_type outside this method?
    setMouseBusy(true);
    current_dither_type = static_cast<DitherType>(item->data(ITEM_DATA_DTYPE, Qt::UserRole).toInt());
    current_sub_dither_type = static_cast<SubDitherType>(item->data(ITEM_DATA_DSUBTYPE, Qt::UserRole).toInt());
    current_dither_number = current_sub_dither_type;
    int index = ditherPage[current_dither_type]; // index of stackedwidget page with the ditherer's settings
    switch(current_dither_type) {
        case ALL: {  // Allebach mono
            bool randomize = activeTreeWidget->getValue(current_sub_dither_type, setting_randomize).toBool();
            whileBlocking(ui->ALL_randomize)->setChecked(randomize);
        } break;
        case DBS: {  // DBS mono
            int dbsIndex = activeTreeWidget->getValue(current_sub_dither_type, setting_formula).toInt();
            whileBlocking(ui->DBS_formula)->setCurrentIndex(dbsIndex);
        } break;
        case DOT: {  // Dot diffusion mono
            ui->Generic_dither_name->setText(item->text(0));
            ui->Generic_groupBox->setTitle(tr("Dot Diffusion Settings"));
        } break;
        case ERR_C: {  // Error diffusion color
            bool serpentine = activeTreeWidget->getValue(current_sub_dither_type, setting_serpentine).toBool();
            whileBlocking(ui->ERR_C_serpentine)->setChecked(serpentine);
        } break;
        case ERR: {  // Error diffusion mono
            bool serpentine = activeTreeWidget->getValue(current_sub_dither_type, setting_serpentine).toBool();
            double jitter = activeTreeWidget->getValue(current_sub_dither_type, setting_jitter).toDouble();
            whileBlocking(ui->ERR_serpentine)->setChecked(serpentine);
            whileBlocking(ui->ERR_jitter)->setValue(jitter);
        }break;
        case GRD: {  // Grid dithering mono
            bool altAlgorithm = activeTreeWidget->getValue(current_sub_dither_type, setting_alt_algorithm).toBool();
            int width = activeTreeWidget->getValue(current_sub_dither_type, setting_width).toInt();
            int height = activeTreeWidget->getValue(current_sub_dither_type, setting_height).toInt();
            int minPixels = activeTreeWidget->getValue(current_sub_dither_type, setting_min_pixels).toInt();
            whileBlocking(ui->GRD_altAlgorithm)->setChecked(altAlgorithm);
            whileBlocking(ui->GRD_width)->setValue(width);
            whileBlocking(ui->GRD_height)->setValue(height);
            whileBlocking(ui->GRD_minPixels)->setValue(minPixels);
        } break;
        case LIP: {  // Dot Lippens dithering mono
            ui->Generic_dither_name->setText(item->text(0));
            ui->Generic_groupBox->setTitle(tr("Dot Lippens Settings"));
        } break;
        case ORD_C: {  // Ordered dithering color
            switch (current_sub_dither_type) {
                case ORD_VA2_C:
                case ORD_VA4_C: {
                    index = ditherPage[current_sub_dither_type];
                    whileBlocking(ui->ORD_VAR_C_step)->setValue(activeTreeWidget->getValue(current_sub_dither_type, setting_step).toInt());
                } break;
                case ORD_IGR_C: {
                    index = ditherPage[current_sub_dither_type];
                    whileBlocking(ui->ORD_IGR_C_step)->setValue(activeTreeWidget->getValue(current_sub_dither_type, setting_step).toInt());
                    whileBlocking(ui->ORD_IGR_C_a)->setValue(activeTreeWidget->getValue(current_sub_dither_type, setting_a).toDouble());
                    whileBlocking(ui->ORD_IGR_C_b)->setValue(activeTreeWidget->getValue(current_sub_dither_type, setting_b).toDouble());
                    whileBlocking(ui->ORD_IGR_C_c)->setValue(activeTreeWidget->getValue(current_sub_dither_type, setting_c).toDouble());
                } break;
                default: {
                    ui->Generic_dither_name->setText(item->text(0));
                    ui->Generic_groupBox->setTitle(tr("Ordered Dither Settings"));
                } break;
            }
        } break;
        case ORD: {  // Ordered dithering mono
            switch (current_sub_dither_type) {
                case ORD_VA2:
                case ORD_VA4: {
                    index = ditherPage[current_sub_dither_type];
                    whileBlocking(ui->ORD_VAR_jitter)->setValue(activeTreeWidget->getValue(current_sub_dither_type, setting_jitter).toDouble());
                    whileBlocking(ui->ORD_VAR_step)->setValue(activeTreeWidget->getValue(current_sub_dither_type, setting_step).toInt());
                } break;
                case ORD_IGR: {
                    index = ditherPage[current_sub_dither_type];
                    whileBlocking(ui->ORD_IGR_jitter)->setValue(activeTreeWidget->getValue(current_sub_dither_type, setting_jitter).toDouble());
                    whileBlocking(ui->ORD_IGR_step)->setValue(activeTreeWidget->getValue(current_sub_dither_type, setting_step).toInt());
                    whileBlocking(ui->ORD_IGR_a)->setValue(activeTreeWidget->getValue(current_sub_dither_type, setting_a).toDouble());
                    whileBlocking(ui->ORD_IGR_b)->setValue(activeTreeWidget->getValue(current_sub_dither_type, setting_b).toDouble());
                    whileBlocking(ui->ORD_IGR_c)->setValue(activeTreeWidget->getValue(current_sub_dither_type, setting_c).toDouble());
                } break;
                default: {
                    double jitter = activeTreeWidget->getValue(current_sub_dither_type, setting_jitter).toDouble();
                    whileBlocking(ui->ORD_jitter)->setValue(jitter);
                } break;
            }
        } break;
        case PAT: {  // Pattern dithering mono
            ui->Generic_dither_name->setText(item->text(0));
            ui->Generic_groupBox->setTitle(tr("Pattern Dither Settings"));
        } break;
        case RIM: {  // Riemersma dithering mono
            bool altAlgorithm = !activeTreeWidget->getValue(current_sub_dither_type, setting_use_riemersma).toBool();
            whileBlocking(ui->RIM_modRiemersma)->setChecked(altAlgorithm);
        } break;
        case THR: {  // Thresholding mono
            bool autoThreshold = activeTreeWidget->getValue(current_sub_dither_type, setting_auto_threshold).toBool();
            double threshold = activeTreeWidget->getValue(current_sub_dither_type, setting_threshold).toDouble();
            double jitter = activeTreeWidget->getValue(current_sub_dither_type, setting_jitter).toDouble();
            whileBlocking(ui->THR_autoThreshold)->setChecked(autoThreshold);
            whileBlocking(ui->THR_threshold)->setValue(threshold);
            whileBlocking(ui->THR_jitter)->setValue(jitter);
        } break;
        case VAR: {  // Variable error diffusion dithering mono
            bool serpentine = activeTreeWidget->getValue(current_sub_dither_type, setting_serpentine).toBool();
            whileBlocking(ui->VAR_serpentine)->setChecked(serpentine);
        } break;
        default: break;
    }
    ui->ditherSettings->setCurrentIndex(index); // display parameters for chosen ditherer
    activeTreeWidget->scrollToItem(activeTreeWidget->currentItem());
    setMouseBusy(false);
    reDither(false);
}

/****************************
 * TRIGGER DITHERING        *
 ****************************/

void MainWindow::reDither(const bool force) {
    /* dithers the loaded image with the currently selected ditherer. if the image already exists in the imagehash
	 * then the image is retrieved from the hash. The force parameter forces a re-dither, even if the dithered image
	 * exists in the hash. */
    if(isDithering)
        return;
    setMouseBusy(true);
    int width = imageHashMono.getSourceImage()->width;
    int height = imageHashMono.getSourceImage()->height;
    if(force) {
        if (current_dither_number < COLOR_DITHER_START) {
            imageHashMono.clearDitheredImage(current_dither_number);
        } else {
            imageHashColor.clearDitheredImage(current_dither_number);
        }
    }
    if (current_dither_number < COLOR_DITHER_START) { // MONO DITHERING
        if(!imageHashMono.hasDitheredImage(current_dither_number)) {  // if dithered image isn't cached, then (re)compute it
            uint8_t *out_buf = static_cast<uint8_t *>(calloc(width * height, sizeof(uint8_t)));
            switch (current_dither_type) {
                case ALL: ALL_dither(out_buf); break;
                case GRD: GRD_dither(out_buf); break;
                case DBS: DBS_dither(out_buf); break;
                case THR: THR_dither(out_buf); break;
                case DOT: DOT_dither(out_buf, current_sub_dither_type); break;
                case ERR: ERR_dither(out_buf, current_sub_dither_type); break;
                case LIP: LIP_dither(out_buf, current_sub_dither_type); break;
                case ORD: ORD_dither(out_buf, current_sub_dither_type); break;
                case PAT: PAT_dither(out_buf, current_sub_dither_type); break;
                case RIM: RIM_dither(out_buf, current_sub_dither_type); break;
                case VAR: VAR_dither(out_buf, current_sub_dither_type); break;
                default: break;
            }
            imageHashMono.setImageFromDither(current_dither_number, out_buf);
            ui->treeWidgetMono->setCurrentItemDitherFlag(true);
            free(out_buf);
        }
        setDitherImageMono(); // also applies custom light/dark colors
        ui->graphicsView->showSourceMono(ui->showOriginalMono->checkState() == Qt::Checked); // is show original checked?
    } else {  // COLOR DITHERING
        if(!imageHashColor.hasDitheredImage(current_dither_number)) {  // if dithered image isn't cached, then (re)compute it
            int* out_buf = static_cast<int*>(calloc(width * height, sizeof(int)));
            switch (current_dither_type) {
                case ERR_C: ERR_C_dither(out_buf, current_sub_dither_type); break;
                case ORD_C: ORD_C_dither(out_buf, current_sub_dither_type); break;
                default: break;
            }
            imageHashColor.setImageFromDither(current_dither_number, cachedPalette->target_palette, out_buf);
            ui->treeWidgetColor->setCurrentItemDitherFlag(true);
            free(out_buf);
        }
        ui->graphicsView->setDitherImageColor(imageHashColor.getDitheredImage(current_dither_number), ui->treeWidgetColor->getCurrentDitherFileName());
        ui->graphicsView->showSourceColor(ui->showOriginalColor->checkState() == Qt::Checked); // is show original checked?
    }
    setMouseBusy(false);
}

/****************************
 * IMAGE LOADING & SAVING   *
 ****************************/

void MainWindow::saveFile(const QString &fileName) {
    /* saves the dithered image */
    setMouseBusy(true);
    QImage image;
    QImageWriter writer;
    if (lastTabIndex == TAB_INDEX_MONO) {
        image = imageHashMono.getDitheredImage(current_dither_number)->convertToFormat(QImage::Format_ARGB32);
    } else {
        image = imageHashColor.getDitheredImage(current_dither_number)->convertToFormat(QImage::Format_ARGB32);
    }
    writer.setFileName(fileName);
    if(writer.canWrite()) {
        writer.write(image);
        if(writer.error() == 0) {
            setMouseBusy(false);
            notification->showText(tr("file saved successfully") + fileName, 2000);
            return;
        }
    }
    setMouseBusy(false);
    notification->showText("<font color=#ec6a5e>" + tr("ERROR") + "</font>\n" +
        tr("failed to save file"), 2000);
}

void MainWindow::loadImageFromFileSlot(const QString &fileName) {
    /* loads the given image and dithers it */
    setMouseBusy(true);
    QImageReader reader;
    reader.setFileName(fileName);
    if(reader.canRead()) {
        const QImage image = reader.read();
        if (reader.error() == 0) {
            currentDirectory = QFileInfo(fileName).absolutePath();
            if (fileManager.isDefaultDirectory()) {
                // if there's no previous dir to remember, we'll use the source image's directory
                fileManager.setDirectory(currentDirectory);
            }
            fileManager.clearCurrentFileName();
            loadImage(&image);
            setMouseBusy(false);
            return;
        }
    }
    setMouseBusy(false);
    notification->showText("<font color=#ec6a5e>" + tr("ERROR") + "</font>\n" +
        tr("failed to load image"), 2000);
}

void MainWindow::loadImage(const QImage* image) {
    /* called after an image has been loaded / copy-pasted / dragged into the tool */
    if(image->width() * image->height() > IMAGE_MAX_SIZE) {
        notification->showText("<font color=#ec6a5e>" + tr("ERROR") + "</font>\n" +
            tr("image resolution is bigger than 4k"), 2000);
        return;
    }
    // reset UI
    notification->cancel();
    ui->graphicsView->resetScene(image->width(), image->height());
    ui->resolutionLabel->setText(QString("%1 \u00D7 %2").arg(image->width()).arg(image->height()));

    // set mono image
    imageHashMono.setSourceImage(image);
    ui->treeWidgetMono->clearAllDitherFlags();
    ui->showOriginalMono->setCheckState(Qt::Unchecked);
    ui->graphicsView->setSourceImageMono(imageHashMono.getSourceQImage());
    resetContrastButtonMonoClickedSlot();
    resetBrightnessButtonMonoClickedSlot();
    resetGammaButtonMonoClickedSlot();

    // set color image
    imageHashColor.setSourceImage(image);
    generateCachedPalette(false, true, true);
    ui->showOriginalColor->setCheckState(Qt::Unchecked);
    ui->graphicsView->setSourceImageColor(imageHashColor.getSourceQImage());
    resetContrastButtonColorClickedSlot();
    resetBrightnessButtonColorClickedSlot();
    resetGammaButtonColorClickedSlot();
    resetSaturationButtonColorClickedSlot();

    if(firstLoad) { // on first load, we're in mono dithering mode
        firstLoad = false;
        enableGui(true);
    }
    treeWidgetItemChangedSlot(activeTreeWidget->currentItem());
}
