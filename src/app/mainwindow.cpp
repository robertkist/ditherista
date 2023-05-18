#include "mainwindow.h"
#include <QClipboard>
#include <QMimeData>
#include <QtConcurrent>
#include <QImageWriter>
#include <QImageReader>
#include <QSvgRenderer>
#include <QLineEdit>
#include "modernredux/style.h"
#include "treewidget.h"


const int THREAD_SLEEP_DELAY_MS = 80;  // delay for sleeping while dithering is in progress
const int IMAGE_MAX_SIZE = 4096 * 4096;
const int TIMEOUT_FOREVER = 1000 * 60 * 60 * 24 * 7;  // forever is a whole week
const int DBS_PROGRESS_TRIGGER_SIZE = 256 * 256;  // image size when to pop up a warning that DBS dither takes long
const int DEFAULT_DITHERER = (int)ALL;

/****************************
 * CONSTRUCTOR / DESTRUCTOR *
 ****************************/

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    // Qt GUI setup
    ui->setupUi(this);
    setWindowIcon(QIcon(":/resources/appicon.png"));
    setModernRedux(this);
    fileManager.setParent(this);
    aboutWindow = new AboutWindow(this);
    helpWindow = new HelpWindow(this);
    setWindowTitle(aboutWindow->appTitle);
    // Splitter
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 0);
    ui->splitter->setSizes({-1, 190});
    ui->ditherSettings->setCurrentIndex(0);
    // set custom icon for image adjustment reset buttons
    QIcon reset_icon;
    QPixmap* pm_enabled = loadSvg(":/resources/times.svg");
    QPixmap* pm_disabled = loadSvg(":/resources/times_disabled.svg");
    reset_icon.addPixmap(*pm_enabled, QIcon::Normal);
    reset_icon.addPixmap(*pm_disabled, QIcon::Disabled);
    ui->resetContrastButton->setIcon(reset_icon);
    ui->resetContrastButton->setIconSize(QSize(16, 16));
    ui->resetContrastButton->setFixedSize(QSize(20, 20));
    ui->resetBrightnessButton->setIcon(reset_icon);
    ui->resetBrightnessButton->setIconSize(QSize(16, 16));
    ui->resetBrightnessButton->setFixedSize(QSize(20, 20));
    ui->resetGammaButton->setIcon(reset_icon);
    ui->resetGammaButton->setIconSize(QSize(16, 16));
    ui->resetGammaButton->setFixedSize(QSize(20, 20));
    // Add items to Mac application menu
    QAction *actionAbout = new QAction(this);
    actionAbout->setText(tr("About Ditherista"));
    actionAbout->setMenuRole(QAction::ApplicationSpecificRole);
#if defined(__APPLE__) && defined(__MACH__)  // menubar for macOS
    QMenuBar *menubar = menuBar();
    QMenu *menu = new QMenu(this);
    menu->setTitle("Menu");
    menu->addAction(actionAbout);
    menubar->addMenu(menu);
    menuBar()->setNativeMenuBar(true);
#else  // menubar for other OSes
    ui->menuHelp->addAction(actionAbout);
    ui->menuFile->addSeparator();
    QAction *actionQuit = new QAction(this);
    actionQuit->setText(tr("Quit"));
    actionQuit->setMenuRole(QAction::ApplicationSpecificRole);
    actionQuit->setShortcut(QKeySequence(Qt::ALT | Qt::Key_F4));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    ui->menuFile->addAction(actionQuit);
#endif
    // connect signals - menu
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(fileSaveSlot()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(fileSaveAsSlot()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(fileOpenSlot()));
    connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(copySlot()));
    connect(ui->actionPaste, SIGNAL(triggered()), this, SLOT(pasteSlot()));
    connect(ui->actionQuickTour, SIGNAL(triggered()), helpWindow, SLOT(showHelp()));
    connect(ui->actionChangelog, SIGNAL(triggered()), helpWindow, SLOT(showChangelog()));
    connect(actionAbout, SIGNAL(triggered()), aboutWindow, SLOT(show()));
    connect(ui->menuEdit, SIGNAL(aboutToShow()), this, SLOT(editMenuAboutToShowSlot()));  // en/disable "paste" entry
    // connect signals - ditherer specific GUI controls
    connect(ui->DBS_formula, SIGNAL(currentIndexChanged(int)), this, SLOT(DBS_setFormulaSlot(int)));
    connect(ui->ERR_serpentine, SIGNAL(toggled(bool)), this, SLOT(serpentineToggledSlot(bool)));
    connect(ui->ERR_jitter, SIGNAL(valueChanged(double)), this, SLOT(jitterValueChangedSlot(double)), Qt::QueuedConnection);
    connect(ui->THR_threshold, SIGNAL(valueChanged(double)), this, SLOT(THR_thresholdValueChangedSlot(double)), Qt::QueuedConnection);
    connect(ui->THR_jitter, SIGNAL(valueChanged(double)), this, SLOT(jitterValueChangedSlot(double)), Qt::QueuedConnection);
    connect(ui->THR_autoThreshold, SIGNAL(toggled(bool)), this, SLOT(THR_toggleAutoThresholdSlot(bool)));
    connect(ui->ORD_jitter, SIGNAL(valueChanged(double)), this, SLOT(jitterValueChangedSlot(double)), Qt::QueuedConnection);
    connect(ui->ORD_IGR_step, SIGNAL(valueChanged(int)), this, SLOT(ORD_stepValueChangedSlot(int)), Qt::QueuedConnection);
    connect(ui->ORD_IGR_jitter, SIGNAL(valueChanged(double)), this, SLOT(jitterValueChangedSlot(double)), Qt::QueuedConnection);
    connect(ui->ORD_IGR_a, SIGNAL(valueChanged(double)), this, SLOT(ORD_IGR_aValueChangedSlot(double)), Qt::QueuedConnection);
    connect(ui->ORD_IGR_b, SIGNAL(valueChanged(double)), this, SLOT(ORD_IGR_bValueChangedSlot(double)), Qt::QueuedConnection);
    connect(ui->ORD_IGR_c, SIGNAL(valueChanged(double)), this, SLOT(ORD_IGR_cValueChangedSlot(double)), Qt::QueuedConnection);
    connect(ui->ORD_VAR_jitter, SIGNAL(valueChanged(double)), this, SLOT(jitterValueChangedSlot(double)), Qt::QueuedConnection);
    connect(ui->ORD_VAR_step, SIGNAL(valueChanged(int)), this, SLOT(ORD_stepValueChangedSlot(int)), Qt::QueuedConnection);
    connect(ui->VAR_serpentine, SIGNAL(toggled(bool)), this, SLOT(serpentineToggledSlot(bool)));
    connect(ui->RIM_modRiemersma, SIGNAL(toggled(bool)), this, SLOT(RIM_modRiemersmaToggledSlot(bool)));
    connect(ui->ALL_randomize, SIGNAL(toggled(bool)), this, SLOT(ALL_randomizeToggleSlot(bool)));
    connect(ui->GRD_altAlgorithm, SIGNAL(toggled(bool)), this, SLOT(GRD_altAlgorithmToggleSlot(bool)));
    connect(ui->GRD_width, SIGNAL(valueChanged(int)), this, SLOT(GRD_widthValueChangedSlot(int)), Qt::QueuedConnection);
    connect(ui->GRD_height, SIGNAL(valueChanged(int)), this, SLOT(GRD_heightValueChangedSlot(int)), Qt::QueuedConnection);
    connect(ui->GRD_minPixels, SIGNAL(valueChanged(int)), this, SLOT(GRD_minPixelsValueChangedSlot(int)), Qt::QueuedConnection);
    // connect signals - image adjustments
    connect(ui->resetContrastButton, SIGNAL(pressed()), this, SLOT(resetContrastButtonPressedSlot()));
    connect(ui->resetBrightnessButton, SIGNAL(pressed()), this, SLOT(resetBrightnessButtonPressedSlot()));
    connect(ui->resetGammaButton, SIGNAL(pressed()), this, SLOT(resetGammaButtonPressedSlot()));
    connect(ui->contrastSlider, SIGNAL(valueChanged(int)), this, SLOT(adjustImageSlot(int)));
    connect(ui->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(adjustImageSlot(int)));
    connect(ui->gammaSlider, SIGNAL(valueChanged(int)), this, SLOT(adjustImageSlot(int)));
    connect(ui->showOriginal, SIGNAL(stateChanged(int)), this, SLOT(showOriginalButtonPressedSlot(int)));
    // connect signals - misc
    connect(ui->treeWidget, SIGNAL(itemChangedSignal(QTreeWidgetItem*)), this, SLOT(treeWidgetItemChangedSlot(QTreeWidgetItem*)));
    connect(ui->graphicsView, SIGNAL(loadImageSignal(QString)), this, SLOT(loadImageFromFileSlot(QString)));
    // add ditherers
    ui->treeWidget->addTreeItem(ALL, ALL_ALL, tr("Allebach"));
    ui->treeWidget->addTreeItem(DBS, DBS_DBS, tr("DBS"));
    ui->treeWidget->addTreeItem(DOT, DOT_KNU, tr("Knuth"));
    ui->treeWidget->addTreeItem(DOT, DOT_MKN, tr("Mini Knuth"));
    ui->treeWidget->addTreeItem(DOT, DOT_OKN, tr("Optimized Knuth"));
    ui->treeWidget->addTreeItem(DOT, DOT_M08, tr("Mese 8x8"));
    ui->treeWidget->addTreeItem(DOT, DOT_M16, tr("Mese 16x16"));
    ui->treeWidget->addTreeItem(DOT, DOT_G08, tr("GuoLiu 8x8"));
    ui->treeWidget->addTreeItem(DOT, DOT_G16, tr("GuoLiu 16x16"));
    ui->treeWidget->addTreeItem(DOT, DOT_SPR, tr("Spiral"));
    ui->treeWidget->addTreeItem(DOT, DOT_ISP, tr("Inverted Spiral"));
    ui->treeWidget->addTreeItem(ERR, ERR_XOT, tr("Xot Diffusion"));
    ui->treeWidget->addTreeItem(ERR, ERR_DIA, tr("Diagonal Dither"));
    ui->treeWidget->addTreeItem(ERR, ERR_FFS, tr("Floyd Steinberg"));
    ui->treeWidget->addTreeItem(ERR, ERR_SF3, tr("Shiau Fan 3"));
    ui->treeWidget->addTreeItem(ERR, ERR_SF2, tr("Shiau Fan 2"));
    ui->treeWidget->addTreeItem(ERR, ERR_SF1, tr("Shiau Fan 1"));
    ui->treeWidget->addTreeItem(ERR, ERR_STK, tr("Stucki"));
    ui->treeWidget->addTreeItem(ERR, ERR_D1D, tr("Error Diffusion 1D"));
    ui->treeWidget->addTreeItem(ERR, ERR_D2D, tr("Error Diffusion 2D"));
    ui->treeWidget->addTreeItem(ERR, ERR_FFS, tr("Fake Floyd Steinberg"));
    ui->treeWidget->addTreeItem(ERR, ERR_JJN, tr("Jarvis-Judice-Ninke"));
    ui->treeWidget->addTreeItem(ERR, ERR_ATK, tr("Atkinson"));
    ui->treeWidget->addTreeItem(ERR, ERR_BRK, tr("Burkes"));
    ui->treeWidget->addTreeItem(ERR, ERR_SI3, tr("Sierra 3"));
    ui->treeWidget->addTreeItem(ERR, ERR_SI2, tr("Sierra 2-Row"));
    ui->treeWidget->addTreeItem(ERR, ERR_SIL, tr("Sierra Lite"));
    ui->treeWidget->addTreeItem(ERR, ERR_SPG, tr("Steve Pigeon"));
    ui->treeWidget->addTreeItem(ERR, ERR_RKI, tr("Robert Kist"));
    ui->treeWidget->addTreeItem(ERR, ERR_SSA, tr("Stevenson-Arce"));
    ui->treeWidget->addTreeItem(GRD, GRD_GRD, tr("Grid"));
    ui->treeWidget->addTreeItem(LIP, LIP_LI1, tr("Lippens 1"));
    ui->treeWidget->addTreeItem(LIP, LIP_LI2, tr("Lippens 2"));
    ui->treeWidget->addTreeItem(LIP, LIP_LI3, tr("Lippens 3"));
    ui->treeWidget->addTreeItem(LIP, LIP_GUO, tr("GuoLiu 16x16"));
    ui->treeWidget->addTreeItem(LIP, LIP_MSE, tr("Mese 16x16"));
    ui->treeWidget->addTreeItem(LIP, LIP_KNU, tr("Knuth"));
    ui->treeWidget->addTreeItem(ORD, ORD_BLU, tr("Blue Noise"));
    ui->treeWidget->addTreeItem(ORD, ORD_BA2, tr("Bayer 2x2"));
    ui->treeWidget->addTreeItem(ORD, ORD_BA3, tr("Bayer 3x3"));
    ui->treeWidget->addTreeItem(ORD, ORD_BA4, tr("Bayer 4x4"));
    ui->treeWidget->addTreeItem(ORD, ORD_BA8, tr("Bayer 8x8"));
    ui->treeWidget->addTreeItem(ORD, ORD_B16, tr("Bayer 16x16"));
    ui->treeWidget->addTreeItem(ORD, ORD_B32, tr("Bayer 32x32"));
    ui->treeWidget->addTreeItem(ORD, ORD_DD1, tr("Dispersed Dots 1"));
    ui->treeWidget->addTreeItem(ORD, ORD_DD2, tr("Dispersed Dots 2"));
    ui->treeWidget->addTreeItem(ORD, ORD_VDD, tr("Void Dispersed Dots"));
    ui->treeWidget->addTreeItem(ORD, ORD_NR1, tr("Non-Rectangular 1"));
    ui->treeWidget->addTreeItem(ORD, ORD_NR2, tr("Non-Rectangular 2"));
    ui->treeWidget->addTreeItem(ORD, ORD_NR3, tr("Non-Rectangular 3"));
    ui->treeWidget->addTreeItem(ORD, ORD_NR4, tr("Non-Rectangular 4"));
    ui->treeWidget->addTreeItem(ORD, ORD_ULY, tr("Ulichney"));
    ui->treeWidget->addTreeItem(ORD, ORD_UB5, tr("Ulichney Bayer 5x5"));
    ui->treeWidget->addTreeItem(ORD, ORD_CD1, tr("Clustered Dot 1"));
    ui->treeWidget->addTreeItem(ORD, ORD_CD2, tr("Clustered Dot 2"));
    ui->treeWidget->addTreeItem(ORD, ORD_CD3, tr("Clustered Dot 3"));
    ui->treeWidget->addTreeItem(ORD, ORD_CD4, tr("Clustered Dot 4"));
    ui->treeWidget->addTreeItem(ORD, ORD_CD5, tr("Clustered Dot 5"));
    ui->treeWidget->addTreeItem(ORD, ORD_CD6, tr("Clustered Dot 6"));
    ui->treeWidget->addTreeItem(ORD, ORD_CD7, tr("Clustered Dot 7"));
    ui->treeWidget->addTreeItem(ORD, ORD_CD8, tr("Clustered Dot 8"));
    ui->treeWidget->addTreeItem(ORD, ORD_CD9, tr("Clustered Dot 9"));
    ui->treeWidget->addTreeItem(ORD, ORD_CDA, tr("Clustered Dot 10"));
    ui->treeWidget->addTreeItem(ORD, ORD_CDB, tr("Clustered Dot 11"));
    ui->treeWidget->addTreeItem(ORD, ORD_CWP, tr("Central White-Point"));
    ui->treeWidget->addTreeItem(ORD, ORD_BWP, tr("Balanced White-Point"));
    ui->treeWidget->addTreeItem(ORD, ORD_DIO, tr("Diagonal Ordered"));
    ui->treeWidget->addTreeItem(ORD, ORD_UCD, tr("Ulichney Cluster Dot"));
    ui->treeWidget->addTreeItem(ORD, ORD_M5C, tr("Magic 5x5 Circle"));
    ui->treeWidget->addTreeItem(ORD, ORD_M6C, tr("Magic 6x6 Circle"));
    ui->treeWidget->addTreeItem(ORD, ORD_M7C, tr("Magic 7x7 Circle"));
    ui->treeWidget->addTreeItem(ORD, ORD_M44, tr("Magic 4x4 45"));
    ui->treeWidget->addTreeItem(ORD, ORD_M64, tr("Magic 6x6 45"));
    ui->treeWidget->addTreeItem(ORD, ORD_M84, tr("Magic 8x8 45"));
    ui->treeWidget->addTreeItem(ORD, ORD_MA4, tr("Magic 4x4"));
    ui->treeWidget->addTreeItem(ORD, ORD_MA6, tr("Magic 6x6"));
    ui->treeWidget->addTreeItem(ORD, ORD_MA8, tr("Magic 8x8"));
    ui->treeWidget->addTreeItem(ORD, ORD_VA2, tr("Variable 2x2"));
    ui->treeWidget->addTreeItem(ORD, ORD_VA4, tr("Variable 4x4"));
    ui->treeWidget->addTreeItem(ORD, ORD_IGR, tr("Interleaved Gradient"));
    ui->treeWidget->addTreeItem(PAT, PAT_22A, tr("2x2 Pattern"));
    ui->treeWidget->addTreeItem(PAT, PAT_33A, tr("3x3 Pattern 1"));
    ui->treeWidget->addTreeItem(PAT, PAT_33B, tr("3x3 Pattern 2"));
    ui->treeWidget->addTreeItem(PAT, PAT_33C, tr("3x3 Pattern 3"));
    ui->treeWidget->addTreeItem(PAT, PAT_44A, tr("4x4 Pattern"));
    ui->treeWidget->addTreeItem(PAT, PAT_52A, tr("5x2 Pattern"));
    ui->treeWidget->addTreeItem(RIM, RIM_HIL, tr("Hilbert Curve"));
    ui->treeWidget->addTreeItem(RIM, RIM_HIM, tr("Hilbert Mod Curve"));
    ui->treeWidget->addTreeItem(RIM, RIM_PEA, tr("Peano Curve"));
    ui->treeWidget->addTreeItem(RIM, RIM_FS0, tr("Fass 0 Curve"));
    ui->treeWidget->addTreeItem(RIM, RIM_FS1, tr("Fass 1 Curve"));
    ui->treeWidget->addTreeItem(RIM, RIM_FS2, tr("Fass 2 Curve"));
    ui->treeWidget->addTreeItem(RIM, RIM_GOS, tr("Gosper Curve"));
    ui->treeWidget->addTreeItem(RIM, RIM_FSS, tr("Fass Spiral"));
    ui->treeWidget->addTreeItem(THR, THR_THR, tr("Threshold Dither"));
    ui->treeWidget->addTreeItem(VAR, VAR_OST, tr("Ostromoukhov"));
    ui->treeWidget->addTreeItem(VAR, VAR_ZHF, tr("ZhouFang"));
    ui->treeWidget->setItemActive(DEFAULT_DITHERER);
    // default values
    ui->treeWidget->setValue(ERR_XOT, setting_serpentine, true);
    ui->treeWidget->setValue(ERR_SF2, setting_serpentine, true);
    ui->treeWidget->setValue(ERR_D1D, setting_serpentine, true);
    ui->treeWidget->setValue(ERR_D2D, setting_serpentine, true);
    ui->treeWidget->setValue(ERR_SIL, setting_serpentine, true);
    ui->treeWidget->setValue(ERR_SSA, setting_serpentine, true);
    ui->treeWidget->setValue(THR_THR, setting_threshold, 0.5);
    ui->treeWidget->setValue(THR_THR, setting_auto_threshold, true);
    ui->treeWidget->setValue(THR_THR, setting_jitter, 0.55);
    ui->treeWidget->setValue(VAR_OST, setting_serpentine, true);
    ui->treeWidget->setValue(VAR_ZHF, setting_serpentine, true);
    ui->treeWidget->setValue(ORD_VA2, setting_step, 55);
    ui->treeWidget->setValue(ORD_VA4, setting_step, 14);
    ui->treeWidget->setValue(ORD_IGR, setting_step, 4);
    ui->treeWidget->setValue(ORD_IGR, setting_a, 52.9829189);
    ui->treeWidget->setValue(ORD_IGR, setting_b, 0.06711056);
    ui->treeWidget->setValue(ORD_IGR, setting_c, 0.00583715);
    ui->treeWidget->setValue(GRD_GRD, setting_alt_algorithm, true);
    ui->treeWidget->setValue(GRD_GRD, setting_width, 4);
    ui->treeWidget->setValue(GRD_GRD, setting_height, 4);
    ui->treeWidget->setValue(RIM_FS0, setting_use_riemersma, false);
    ui->treeWidget->setValue(RIM_FS1, setting_use_riemersma, false);
    ui->treeWidget->setValue(RIM_FS2, setting_use_riemersma, false);
    ui->treeWidget->setValue(RIM_FSS, setting_use_riemersma, false);
    ui->treeWidget->setValue(RIM_GOS, setting_use_riemersma, false);
    ui->treeWidget->setValue(RIM_HIL, setting_use_riemersma, false);
    ui->treeWidget->setValue(RIM_HIM, setting_use_riemersma, false);
    ui->treeWidget->setValue(RIM_PEA, setting_use_riemersma, false);
    ui->THR_threshold->setEnabled(false);  // because we're setting auto-threshold as default for THR ditherer
    // notification label
    notification = new NotificationLabel(ui->graphicsView);
}

MainWindow::~MainWindow() {
    delete ui;
}

/***************
 * HELPERS     *
 ***************/

QPixmap* MainWindow::loadSvg(QString fileName) {
    /* properly loads SVG images for HiDPI screens */
    QSvgRenderer* renderer = new QSvgRenderer(fileName, this);
    QPixmap* pixmap = new QPixmap(renderer->defaultSize() * devicePixelRatio());
    pixmap->fill(Qt::transparent);
    QPainter painter;
    painter.begin(pixmap);
    renderer->render(&painter);
    painter.end();
    pixmap->setDevicePixelRatio(devicePixelRatio());
    return pixmap;
}

void MainWindow::enableUi(bool enable) {
    /* en/disables menu items and mouse interaction when the program is busy */
    if(!enable) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QApplication::instance()->installEventFilter(&eventFilter);
    } else {
        while(QApplication::overrideCursor())  // safeguard against accidental double-overrides
            QApplication::restoreOverrideCursor();
        QApplication::instance()->removeEventFilter(&eventFilter);
    }
    if(!firstLoad) {
        ui->imageSettingsContainer->setEnabled(enable);
        ui->ditherSettingsContainer->setEnabled(enable);
        ui->treeWidget->setEnabled(enable);
        ui->actionOpen->setEnabled(enable);
        ui->actionSave->setEnabled(enable);
        ui->actionSaveAs->setEnabled(enable);
        ui->actionCopy->setEnabled(enable);
        ui->actionPaste->setEnabled(enable);
    }
}

/*****************************************
 * GUI & EVENT/SLOT RELATED METHODS      *
 *****************************************/

void MainWindow::adjustImageSlot(int) {
    /* applies the current image adjustments to the source image */
    double contrast = (double) ui->contrastSlider->value() / 100.0;
    double brightness = (double) ui->brightnessSlider->value() / 100.0;
    double gamma = (double) ui->gammaSlider->value() / 100.0;
    imageHash.adjustSource(gamma, brightness, contrast);
    imageHash.clearAllDitheredImages();
    ui->treeWidget->clearAllDitherFlags();
    enableUi(true);
    if (ui->showOriginal->checkState() == Qt::Checked)
        ui->graphicsView->setSourceImage(imageHash.getSourceQImage());
    else
        reDither(false);
}

void MainWindow::showOriginalButtonPressedSlot(int checked) {
    /* toggles between showing the source image and the dithered image */
    if(checked == Qt::Checked) {
        ui->graphicsView->showSource(true);
    } else {
        ui->graphicsView->showSource(false);
        reDither(false);
    }
}

void MainWindow::editMenuAboutToShowSlot() {
    /* disable edit->paste menu item if clipboard doesn't contain something we can load */
    ui->actionPaste->setEnabled(false);
    QClipboard* clipboard = QGuiApplication::clipboard();
    const QMimeData* md = clipboard->mimeData();
    if(md->hasUrls())
        ui->actionPaste->setEnabled(true);
    else if(!clipboard->image(QClipboard::Clipboard).isNull())
        ui->actionPaste->setEnabled(true);
}

void MainWindow::treeWidgetItemChangedSlot(QTreeWidgetItem* item) {
    /*
     * Dithers with the currently selected ditherer when ditherer selection changes.
     * i.e. the user clicks a different entry in the ditherer list.
     * */
    enableUi(false);
    int index = 0;
    current_dither_number = item->data(ITEM_DATA_COUNT, Qt::UserRole).toInt();
    current_dither_type = (enum DitherType)item->data(ITEM_DATA_DTYPE, Qt::UserRole).toInt();
    current_sub_dither_type = (enum SubDitherType)item->data(ITEM_DATA_DSUBTYPE, Qt::UserRole).toInt();
    if (current_sub_dither_type == ORD_VA2 || current_sub_dither_type == ORD_VA4 || current_sub_dither_type == ORD_IGR) {
        // apply last used values for ditherer settings (or default value)
        index = ditherPage[current_sub_dither_type];
        if(current_sub_dither_type == ORD_IGR) {
            qDebug()<<"applying settings";
            ui->ORD_IGR_jitter->blockSignals(true);
            ui->ORD_IGR_jitter->setValue(ui->treeWidget->getValue(ORD_IGR, setting_jitter).toDouble());
            ui->ORD_IGR_jitter->blockSignals(false);
            ui->ORD_IGR_step->blockSignals(true);
            ui->ORD_IGR_step->setValue(ui->treeWidget->getValue(ORD_IGR, setting_step).toInt());
            ui->ORD_IGR_step->blockSignals(false);
            ui->ORD_IGR_a->blockSignals(true);
            ui->ORD_IGR_a->setValue(ui->treeWidget->getValue(ORD_IGR, setting_a).toDouble());
            ui->ORD_IGR_a->blockSignals(false);
            ui->ORD_IGR_b->blockSignals(true);
            ui->ORD_IGR_b->setValue(ui->treeWidget->getValue(ORD_IGR, setting_b).toDouble());
            ui->ORD_IGR_b->blockSignals(false);
            ui->ORD_IGR_c->blockSignals(true);
            ui->ORD_IGR_c->setValue(ui->treeWidget->getValue(ORD_IGR, setting_c).toDouble());
            ui->ORD_IGR_c->blockSignals(false);
        } else {
            ui->ORD_VAR_jitter->blockSignals(true);
            ui->ORD_VAR_jitter->setValue(ui->treeWidget->getValue(current_sub_dither_type, setting_jitter).toDouble());
            ui->ORD_VAR_jitter->blockSignals(false);
            ui->ORD_VAR_step->blockSignals(true);
            ui->ORD_VAR_step->setValue(ui->treeWidget->getValue(current_sub_dither_type, setting_step).toInt());
            ui->ORD_VAR_step->blockSignals(false);
        }
    } else {
        index = ditherPage[current_dither_type];
        switch(current_dither_type) {
            case ALL:
                ui->ALL_randomize->blockSignals(true);
                ui->ALL_randomize->setChecked(ui->treeWidget->getValue(current_sub_dither_type, setting_randomize).toBool());
                ui->ALL_randomize->blockSignals(false);
                break;
            case DBS:
                ui->DBS_formula->blockSignals(true);
                ui->DBS_formula->setCurrentIndex(ui->treeWidget->getValue(current_sub_dither_type, setting_formula).toInt());
                ui->DBS_formula->blockSignals(false);
                break;
            case DOT:
                ui->Generic_dither_name->setText(item->text(0));
                ui->Generic_groupBox->setTitle(tr("Dot Diffusion Settings"));
                break;
            case ERR:
                ui->ERR_serpentine->blockSignals(true);
                ui->ERR_serpentine->setChecked(ui->treeWidget->getValue(current_sub_dither_type, setting_serpentine).toBool());
                ui->ERR_serpentine->blockSignals(false);
                ui->ERR_jitter->blockSignals(true);
                ui->ERR_jitter->setValue(ui->treeWidget->getValue(current_sub_dither_type, setting_jitter).toDouble());
                ui->ERR_jitter->blockSignals(false);
                break;
            case GRD:
                ui->GRD_altAlgorithm->blockSignals(true);
                ui->GRD_altAlgorithm->setChecked(ui->treeWidget->getValue(current_sub_dither_type, setting_alt_algorithm).toBool());
                ui->GRD_altAlgorithm->blockSignals(false);
                ui->GRD_width->blockSignals(true);
                ui->GRD_width->setValue(ui->treeWidget->getValue(current_sub_dither_type, setting_width).toInt());
                ui->GRD_width->blockSignals(false);
                ui->GRD_height->blockSignals(true);
                ui->GRD_height->setValue(ui->treeWidget->getValue(current_sub_dither_type, setting_height).toInt());
                ui->GRD_height->blockSignals(false);
                ui->GRD_minPixels->blockSignals(true);
                ui->GRD_minPixels->setValue(ui->treeWidget->getValue(current_sub_dither_type, setting_min_pixels).toInt());
                ui->GRD_minPixels->blockSignals(false);
                break;
            case LIP:
                ui->Generic_dither_name->setText(item->text(0));
                ui->Generic_groupBox->setTitle(tr("Dot Lippens Settings"));
                break;
            case ORD:
                ui->ORD_jitter->blockSignals(true);
                ui->ORD_jitter->setValue(ui->treeWidget->getValue(current_sub_dither_type, setting_jitter).toDouble());
                ui->ORD_jitter->blockSignals(false);
                break;
            case PAT:
                ui->Generic_dither_name->setText(item->text(0));
                ui->Generic_groupBox->setTitle(tr("Pattern Dither Settings"));
                break;
            case RIM:
                ui->RIM_modRiemersma->blockSignals(true);
                ui->RIM_modRiemersma->setChecked(!ui->treeWidget->getValue(current_sub_dither_type, setting_use_riemersma).toBool());
                ui->RIM_modRiemersma->blockSignals(false);
                break;
            case THR:
                ui->THR_autoThreshold->blockSignals(true);
                ui->THR_autoThreshold->setChecked(ui->treeWidget->getValue(current_sub_dither_type, setting_auto_threshold).toBool());
                ui->THR_autoThreshold->blockSignals(false);
                ui->THR_threshold->blockSignals(true);
                ui->THR_threshold->setValue(ui->treeWidget->getValue(current_sub_dither_type, setting_threshold).toDouble());
                ui->THR_threshold->blockSignals(false);
                ui->THR_jitter->blockSignals(true);
                ui->THR_jitter->setValue(ui->treeWidget->getValue(current_sub_dither_type, setting_jitter).toDouble());
                ui->THR_jitter->blockSignals(false);
                break;
            case VAR:
                ui->VAR_serpentine->blockSignals(true);
                ui->VAR_serpentine->setChecked(ui->treeWidget->getValue(current_sub_dither_type, setting_serpentine).toBool());
                ui->VAR_serpentine->blockSignals(false);
                break;
            default: break;
        }
    }
    ui->ditherSettings->setCurrentIndex(index);
    ui->showOriginal->setCheckState(Qt::Unchecked);
    ui->treeWidget->scrollToItem(ui->treeWidget->currentItem());
    enableUi(true);
    reDither(false);
    ui->treeWidget->setFocus(); // restore focus after re-enabling GUI
}

void MainWindow::DBS_setFormulaSlot(int formula) {
    /* DBS formula changed in ComboBox */
    ui->treeWidget->setValue(current_sub_dither_type, setting_formula, formula);
    forceReDitherSlot();
}

void MainWindow::ALL_randomizeToggleSlot(bool value) {
    /* randomize checkbox toggle */
    ui->treeWidget->setValue(current_sub_dither_type, setting_randomize, value);
    forceReDitherSlot();
}

void MainWindow::GRD_altAlgorithmToggleSlot(bool value) {
    /* Grid dither checkbox toggle */
    ui->treeWidget->setValue(current_sub_dither_type, setting_alt_algorithm, value);
    forceReDitherSlot();
}

void MainWindow::GRD_widthValueChangedSlot(int value) {
    /* Grid dither width spinBox */
    ui->treeWidget->setValue(current_sub_dither_type, setting_width, value);
    forceReDitherSlot();
}

void MainWindow::GRD_heightValueChangedSlot(int value) {
    /* Grid dither height spinbox */
    ui->treeWidget->setValue(current_sub_dither_type, setting_height, value);
    forceReDitherSlot();
}

void MainWindow::GRD_minPixelsValueChangedSlot(int value) {
    /* Grid dither min-pixels spinBox */
    ui->treeWidget->setValue(current_sub_dither_type, setting_min_pixels, value);
    forceReDitherSlot();
}

void MainWindow::serpentineToggledSlot(bool serpentine) {
    /* Serpentine toggle checkbox for Error Diffusion and Variable Error Diffusion */
    ui->treeWidget->setValue(current_sub_dither_type,setting_serpentine, serpentine);
    forceReDitherSlot();
}

void MainWindow::jitterValueChangedSlot(double jitter) {
    /* Jitter spinBox changed */
    ui->treeWidget->setValue(current_sub_dither_type, setting_jitter, jitter);
    forceReDitherSlot();
}

void MainWindow::RIM_modRiemersmaToggledSlot(bool value) {
    /* checkbox toggle for modified Riemersma algorithm */
    ui->treeWidget->setValue(current_sub_dither_type, setting_use_riemersma, value);
    forceReDitherSlot();
}

void MainWindow::THR_thresholdValueChangedSlot(double threshold) {
    /* Thresholding mid-value spinBox */
    ui->treeWidget->setValue(current_sub_dither_type, setting_threshold, threshold);
    forceReDitherSlot();
}

void MainWindow::THR_toggleAutoThresholdSlot(bool autoThreshold) {
    /* Thresholding auto-threshold checkbox toggle */
    ui->treeWidget->setValue(current_sub_dither_type, setting_auto_threshold, autoThreshold);
    ui->THR_threshold->setEnabled(ui->THR_autoThreshold->checkState() != Qt::Checked);
    forceReDitherSlot();
}

void MainWindow::ORD_stepValueChangedSlot(int step) {
    ui->treeWidget->setValue(current_sub_dither_type, setting_step, step);
    forceReDitherSlot();
}

void MainWindow::ORD_IGR_aValueChangedSlot(double a) {
    ui->treeWidget->setValue(current_sub_dither_type, setting_a, a);
    forceReDitherSlot();
}

void MainWindow::ORD_IGR_bValueChangedSlot(double b) {
    ui->treeWidget->setValue(current_sub_dither_type, setting_b, b);
    forceReDitherSlot();
}

void MainWindow::ORD_IGR_cValueChangedSlot(double c) {
    ui->treeWidget->setValue(current_sub_dither_type, setting_c, c);
    forceReDitherSlot();
}

void MainWindow::fileOpenSlot() {
    QString fileName;
    if(fileManager.getOpenFileName(&fileName)) {
        loadImageFromFileSlot(fileName);
    }
}

void MainWindow::fileSaveAsSlot() {
    QString fileName = ui->treeWidget->getCurrentDitherFileName();
    if(fileName != "")
        saveFile(fileManager.fileSave(true, fileName));
}

void MainWindow::fileSaveSlot() {
    QString fileName = ui->treeWidget->getCurrentDitherFileName();
    if(fileName != "")
        saveFile(fileManager.fileSave(false, fileName));
}

void MainWindow::copySlot() {
    QClipboard* clipboard = QGuiApplication::clipboard();
    QImage image(imageHash.getDitheredImage(current_dither_number)->convertToFormat(QImage::Format_RGB32));
    clipboard->setImage(image, QClipboard::Clipboard);
}

void MainWindow::pasteSlot() {
    QClipboard* clipboard = QGuiApplication::clipboard();
    const QMimeData* md = clipboard->mimeData();
    if(md->hasUrls()) {
        if(md->urls()[0].isLocalFile())
            loadImageFromFileSlot(md->urls()[0].toLocalFile());
    } else {
        QImage image = clipboard->image(QClipboard::Clipboard);
        if (!image.isNull())
            loadImage(&image);
    }
}

/****************************
 * DITHERERS                *
 ****************************/

void MainWindow::runDitherThread() {
    /* Executes a ditherer in a thread */
    isDithering = true;
    while(!fthread.isFinished()) {
        QThread::msleep(THREAD_SLEEP_DELAY_MS);
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents); // keep GUI thread alive while dithering
    }
    fthread.waitForFinished();
    isDithering = false;
}

void MainWindow::ALL_dither(uint8_t* out_buf) {
    /* Runs the Allebach ditherer */
    bool randomize = ui->ALL_randomize->isChecked();
    fthread = QtConcurrent::run(kallebach_dither, imageHash.getSourceDitherImage(), randomize, out_buf);
    runDitherThread();
}

void MainWindow::GRD_dither(uint8_t* out_buf) {
    /* Runs the Grid ditherer */
    bool altAlgorithm = ui->GRD_altAlgorithm->isChecked();
    int width = ui->GRD_width->value();
    int height = ui->GRD_height->value();
    int minPixels = ui->GRD_minPixels->value();
    DitherImage* image = imageHash.getSourceDitherImage();
    // use std::bind as work-around for Qt5, where QtConcurrent::run only supports up to 5 arguments
    fthread = QtConcurrent::run(std::bind(&grid_dither, image, width, height, minPixels, altAlgorithm, out_buf));
    runDitherThread();
}

void MainWindow::DBS_dither(uint8_t* out_buf) {
    /* Runs the DBS ditherer */
    DitherImage* image = imageHash.getSourceDitherImage();
    if(image->width * image->height > DBS_PROGRESS_TRIGGER_SIZE)  // only show if image is bigger than a certain size
        notification->showText(tr("please wait..."), TIMEOUT_FOREVER);
    int formula = ui->DBS_formula->currentIndex();
    fthread = QtConcurrent::run(dbs_dither, image, formula, out_buf);
    runDitherThread();
    if(image->width * image->height > DBS_PROGRESS_TRIGGER_SIZE)
        notification->cancel();
}

void MainWindow::DOT_dither(uint8_t* out_buf, enum SubDitherType n) {
    /* Runs the Dot Diffusion ditherer */
    DotClassMatrix* cm = nullptr;
    DotDiffusionMatrix* dm = nullptr;
    switch(n) {
        case DOT_KNU: dm = get_default_diffusion_matrix(); cm = get_knuth_class_matrix(); break;
        case DOT_MKN: dm = get_default_diffusion_matrix(); cm = get_mini_knuth_class_matrix(); break;
        case DOT_OKN: dm = get_default_diffusion_matrix(); cm = get_optimized_knuth_class_matrix(); break;
        case DOT_M08: dm = get_default_diffusion_matrix(); cm = get_mese_8x8_class_matrix(); break;
        case DOT_M16: dm = get_default_diffusion_matrix(); cm = get_mese_16x16_class_matrix(); break;
        case DOT_G08: dm = get_guoliu8_diffusion_matrix(); cm = get_guoliu_8x8_class_matrix(); break;
        case DOT_G16: dm = get_guoliu16_diffusion_matrix(); cm = get_guoliu_16x16_class_matrix(); break;
        case DOT_SPR: dm = get_guoliu8_diffusion_matrix(); cm = get_spiral_class_matrix(); break;
        case DOT_ISP: dm = get_guoliu8_diffusion_matrix(); cm = get_spiral_inverted_class_matrix(); break;
        default: qDebug() << "WARNING: requested DOT ditherer " << n << " not found"; break;
    }
    if(dm != nullptr) {
        fthread = QtConcurrent::run(dot_diffusion_dither, imageHash.getSourceDitherImage(), dm, cm, out_buf);
        runDitherThread();
        DotClassMatrix_free(cm);
        DotDiffusionMatrix_free(dm);
    }
}

void MainWindow::ERR_dither(uint8_t* out_buf, enum SubDitherType n) {
    /* Runs the Error Diffusion ditherer */
    ErrorDiffusionMatrix* m = nullptr;
    switch(n) {
        case ERR_XOT: m = get_xot_matrix(); break;
        case ERR_DIA: m = get_diagonal_matrix(); break;
        case ERR_FYD: m = get_floyd_steinberg_matrix(); break;
        case ERR_SF3: m = get_shiaufan3_matrix(); break;
        case ERR_SF2: m = get_shiaufan2_matrix(); break;
        case ERR_SF1: m = get_shiaufan1_matrix(); break;
        case ERR_STK: m = get_stucki_matrix(); break;
        case ERR_D1D: m = get_diffusion_1d_matrix(); break;
        case ERR_D2D: m = get_diffusion_2d_matrix(); break;
        case ERR_FFS: m = get_fake_floyd_steinberg_matrix(); break;
        case ERR_JJN: m = get_jarvis_judice_ninke_matrix(); break;
        case ERR_ATK: m = get_atkinson_matrix(); break;
        case ERR_BRK: m = get_burkes_matrix(); break;
        case ERR_SI3: m = get_sierra_3_matrix(); break;
        case ERR_SI2: m = get_sierra_2row_matrix(); break;
        case ERR_SIL: m = get_sierra_lite_matrix(); break;
        case ERR_SPG: m = get_steve_pigeon_matrix(); break;
        case ERR_RKI: m = get_robert_kist_matrix(); break;
        case ERR_SSA: m = get_stevenson_arce_matrix(); break;
        default: qDebug() << "WARNING: requested ERR ditherer " << n << " not found"; break;
    }
    if(m != nullptr) {
        double jitter = ui->ERR_jitter->value();
        bool serpentine = ui->ERR_serpentine->isChecked();
        fthread = QtConcurrent::run(error_diffusion_dither, imageHash.getSourceDitherImage(), m, serpentine, jitter, out_buf);
        runDitherThread();
        ErrorDiffusionMatrix_free(m);
    }
}

void MainWindow::LIP_dither(uint8_t* out_buf, enum SubDitherType n) {
    /* Runs the Lippens & Philips Dot ditherer */
    DotClassMatrix* cm = nullptr;
    DotLippensCoefficients* coe = nullptr;
    switch(n) {
        case LIP_LI1:
            cm = get_dotlippens_class_matrix();
            coe = get_dotlippens_coefficients1();
            break;
        case LIP_LI2:
            cm = get_dotlippens_class_matrix();
            coe = get_dotlippens_coefficients2();
            break;
        case LIP_LI3:
            cm = get_dotlippens_class_matrix();
            coe = get_dotlippens_coefficients3();
            break;
        case LIP_GUO:
            cm = get_guoliu_16x16_class_matrix();
            coe = get_dotlippens_coefficients1();
            break;
        case LIP_MSE:
            cm = get_mese_16x16_class_matrix();
            coe = get_dotlippens_coefficients1();
            break;
        case LIP_KNU:
            cm = get_knuth_class_matrix();
            coe = get_dotlippens_coefficients1();
            break;
        default: qDebug() << "WARNING: requested VAR ditherer " << n << " not found"; break;
    }
    if(cm != nullptr && coe != nullptr) {
        fthread = QtConcurrent::run(dotlippens_dither, imageHash.getSourceDitherImage(), cm, coe, out_buf);
        runDitherThread();
        DotLippensCoefficients_free(coe);
        DotClassMatrix_free(cm);
    }
}

void MainWindow::ORD_dither(uint8_t* out_buf, enum SubDitherType n) {
    /* Runs the Ordered ditherer */
    OrderedDitherMatrix* m = nullptr;
    switch(n) {
        case ORD_BLU: m = get_blue_noise_128x128(); break;
        case ORD_BA2: m = get_bayer2x2_matrix(); break;
        case ORD_BA3: m = get_bayer3x3_matrix(); break;
        case ORD_BA4: m = get_bayer4x4_matrix(); break;
        case ORD_BA8: m = get_bayer8x8_matrix(); break;
        case ORD_B16: m = get_bayer16x16_matrix(); break;
        case ORD_B32: m = get_bayer32x32_matrix(); break;
        case ORD_DD1: m = get_dispersed_dots_1_matrix(); break;
        case ORD_DD2: m = get_dispersed_dots_2_matrix(); break;
        case ORD_VDD: m = get_ulichney_void_dispersed_dots_matrix(); break;
        case ORD_NR1: m = get_non_rectangular_1_matrix(); break;
        case ORD_NR2: m = get_non_rectangular_2_matrix(); break;
        case ORD_NR3: m = get_non_rectangular_3_matrix(); break;
        case ORD_NR4: m = get_non_rectangular_4_matrix(); break;
        case ORD_ULY: m = get_ulichney_matrix(); break;
        case ORD_UB5: m = get_ulichney_bayer_5_matrix(); break;
        case ORD_CD1: m = get_bayer_clustered_dot_1_matrix(); break;
        case ORD_CD2: m = get_bayer_clustered_dot_2_matrix(); break;
        case ORD_CD3: m = get_bayer_clustered_dot_3_matrix(); break;
        case ORD_CD4: m = get_bayer_clustered_dot_4_matrix(); break;
        case ORD_CD5: m = get_bayer_clustered_dot_5_matrix(); break;
        case ORD_CD6: m = get_bayer_clustered_dot_6_matrix(); break;
        case ORD_CD7: m = get_bayer_clustered_dot_7_matrix(); break;
        case ORD_CD8: m = get_bayer_clustered_dot_8_matrix(); break;
        case ORD_CD9: m = get_bayer_clustered_dot_9_matrix(); break;
        case ORD_CDA: m = get_bayer_clustered_dot_10_matrix(); break;
        case ORD_CDB: m = get_bayer_clustered_dot_11_matrix(); break;
        case ORD_CWP: m = get_central_white_point_matrix(); break;
        case ORD_BWP: m = get_balanced_centered_point_matrix(); break;
        case ORD_DIO: m = get_diagonal_ordered_matrix_matrix(); break;
        case ORD_UCD: m = get_ulichney_clustered_dot_matrix(); break;
        case ORD_M5C: m = get_magic5x5_circle_matrix(); break;
        case ORD_M6C: m = get_magic6x6_circle_matrix(); break;
        case ORD_M7C: m = get_magic7x7_circle_matrix(); break;
        case ORD_M44: m = get_magic4x4_45_matrix(); break;
        case ORD_M64: m = get_magic6x6_45_matrix(); break;
        case ORD_M84: m = get_magic8x8_45_matrix(); break;
        case ORD_MA4: m = get_magic4x4_matrix(); break;
        case ORD_MA6: m = get_magic6x6_matrix(); break;
        case ORD_MA8: m = get_magic8x8_matrix(); break;
        case ORD_VA2: m = get_variable_2x2_matrix(ui->ORD_VAR_step->value()); break; // 50
        case ORD_VA4: m = get_variable_4x4_matrix(ui->ORD_VAR_step->value()); break;
        case ORD_IGR: {
            double a = ui->ORD_IGR_a->value();
            double b = ui->ORD_IGR_b->value();
            double c = ui->ORD_IGR_c->value();
            m = get_interleaved_gradient_noise(ui->ORD_IGR_step->value(), a, b, c);
        } break;
        default: qDebug() << "WARNING: requested ORD ditherer " << n << " not found"; break;
    }
    if(m != nullptr) {
        double jitter;
        switch(n) {
            case ORD_VA2:
            case ORD_VA4: jitter = ui->ORD_VAR_jitter->value(); break;
            case ORD_IGR: jitter = ui->ORD_IGR_jitter->value(); break;
            default: jitter = ui->ORD_jitter->value(); break;
        }
        fthread = QtConcurrent::run(ordered_dither, imageHash.getSourceDitherImage(), m, jitter, out_buf);
        runDitherThread();
        OrderedDitherMatrix_free(m);
    }
}

void MainWindow::PAT_dither(uint8_t* out_buf, enum SubDitherType n) {
    /* Runs the Pattern ditherer */
    TilePattern *tp = nullptr;
    switch(n) {
        case PAT_22A: tp = get_2x2_pattern(); break;
        case PAT_33A: tp = get_3x3_v1_pattern(); break;
        case PAT_33B: tp = get_3x3_v2_pattern(); break;
        case PAT_33C: tp = get_3x3_v3_pattern(); break;
        case PAT_44A: tp = get_4x4_pattern(); break;
        case PAT_52A: tp = get_5x2_pattern(); break;
        default: qDebug() << "WARNING: requested PAT ditherer " << n << " not found"; break;
    }
    if(tp != nullptr) {
        fthread = QtConcurrent::run(pattern_dither, imageHash.getSourceDitherImage(), tp, out_buf);
        runDitherThread();
        TilePattern_free(tp);
    }
}

void MainWindow::THR_dither(uint8_t* out_buf) {
    /* Runs the Threshold ditherer */
    double threshold, jitter;
    if (ui->THR_autoThreshold->isChecked())
        threshold = auto_threshold(imageHash.getSourceDitherImage());
    else
        threshold = ui->THR_threshold->value();
    jitter = ui->THR_jitter->value();
    fthread = QtConcurrent::run(threshold_dither, imageHash.getSourceDitherImage(), threshold, jitter, out_buf);
    runDitherThread();
}

void MainWindow::VAR_dither(uint8_t* out_buf, enum SubDitherType n) {
    /* Runs the Variable Error Diffusion ditherer */
    bool s = ui->VAR_serpentine->isChecked();
    switch((enum SubDitherType)n) {
        case VAR_OST:
            fthread = QtConcurrent::run(variable_error_diffusion_dither, imageHash.getSourceDitherImage(), Ostromoukhov, s, out_buf);
            runDitherThread();
            break;
        case VAR_ZHF:
            fthread = QtConcurrent::run(variable_error_diffusion_dither, imageHash.getSourceDitherImage(), Zhoufang, s, out_buf);
            runDitherThread();
            break;
        default: qDebug() << "WARNING: requested VAR ditherer " << n << " not found"; break;
    }
}

void MainWindow::RIM_dither(uint8_t* out_buf, enum SubDitherType n) {
    /* Runs the Riemersma ditherer */
    RiemersmaCurve *c = nullptr;
    switch(n) {
        case RIM_HIL: c = get_hilbert_curve(); break;
        case RIM_HIM: c = get_hilbert_mod_curve(); break;
        case RIM_PEA: c = get_peano_curve(); break;
        case RIM_FS0: c = get_fass0_curve(); break;
        case RIM_FS1: c = get_fass1_curve(); break;
        case RIM_FS2: c = get_fass2_curve(); break;
        case RIM_GOS: c = get_gosper_curve(); break;
        case RIM_FSS: c = get_fass_spiral_curve(); break;
        default: qDebug() << "WARNING: requested RIM ditherer " << n << " not found"; break;
    }
    if(c != nullptr) {
        bool mod = ui->RIM_modRiemersma->isChecked();
        fthread = QtConcurrent::run(riemersma_dither, imageHash.getSourceDitherImage(), c, !mod, out_buf);
        runDitherThread();
        RiemersmaCurve_free(c);
    }
}

void MainWindow::reDither(bool force) {
    /* dithers the loaded image with the currently selected ditherer */
    if(isDithering)
        return;
    enableUi(false);
    int width = imageHash.getSourceDitherImage()->width;
    int height = imageHash.getSourceDitherImage()->height;
    ui->graphicsView->resetScene(width, height);
    if(force)
        imageHash.clearDitheredImage(current_dither_number);
    if(!imageHash.hasDitheredImage(current_dither_number)) {
        uint8_t* out_buf = (uint8_t*) calloc(width * height, sizeof(uint8_t));
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
        imageHash.setImageFromDither(current_dither_number, out_buf);
        free(out_buf);
    }
    ui->graphicsView->setSourceImage(imageHash.getSourceQImage());
    ui->graphicsView->setDitherImage(imageHash.getDitheredImage(current_dither_number),
                                     ui->treeWidget->getCurrentDitherFileName());
    ui->graphicsView->showSource(ui->showOriginal->checkState() == Qt::Checked);
    ui->treeWidget->setCurrentItemDitherFlag(true);
    enableUi(true);
}

/****************************
 * IMAGE LOADING & SAVING   *
 ****************************/

void MainWindow::saveFile(QString fileName) {
    enableUi(false);
    QImage image(imageHash.getDitheredImage(current_dither_number)->convertToFormat(QImage::Format_RGB32));
    QImageWriter writer;
    writer.setFileName(fileName);
    if(writer.canWrite()) {
        writer.write(image);
        if(writer.error() == 0) {
            enableUi(true);
            return;
        }
    }
    enableUi(true);
    notification->showText("<font color=#ec6a5e>" + tr("ERROR") + "</font>\n" + tr("failed to save file"), 2000);
}

void MainWindow::loadImageFromFileSlot(QString fileName) {
    /* loads the given image and dithers it */
    enableUi(false);
    ui->treeWidget->clearAllDitherFlags();
    QImageReader reader;
    reader.setFileName(fileName);
    if(reader.canRead()) {
        QImage image = reader.read();
        if (reader.error() == 0) {
            loadImage(&image);
            enableUi(true);
            return;
        }
    }
    enableUi(true);
    notification->showText("<font color=#ec6a5e>" + tr("ERROR") + "</font>\n" + tr("failed to load image"), 2000);
}

void MainWindow::loadImage(const QImage* image) {
    /* called after an image has been loaded/copy-pasted/dragged into the tool */
    if(image->width() * image->height() > IMAGE_MAX_SIZE) {
        notification->showText("<font color=#ec6a5e>" + tr("ERROR") + "</font>\n" + tr("image resolution is bigger than 4k"), 2000);
        return;
    }
    notification->cancel();
    imageHash.setSourceImage(image);
    ui->showOriginal->setCheckState(Qt::Unchecked);
    // reset image adjustment sliders
    ui->contrastSlider->setValue(0);
    ui->brightnessSlider->setValue(0);
    ui->gammaSlider->setValue(0);
    if(firstLoad) {
        firstLoad = false;
        ui->treeWidget->setEnabled(true);
        ui->ditherSettingsContainer->setEnabled(true);
        ui->imageSettingsContainer->setEnabled(true);
        ui->actionSave->setEnabled(true);
        ui->actionSaveAs->setEnabled(true);
        ui->actionCopy->setEnabled(true);
        treeWidgetItemChangedSlot(ui->treeWidget->currentItem());
    } else {
        reDither(false);
    }
}
