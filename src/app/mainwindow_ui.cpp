#include "mainwindow.h"
#include "consts.h"
#include "modernredux/style.h"
#include "ui_elements/signalblocker.h"
#include <QSvgRenderer>

/* This file contains:
 * - GUI initialization methods (default values, connecting Qt signals, etc.)
 * - GUI helper methods
 */

/********************************
 * QT GUI EVENTS / EVENT-FILTER *
 ********************************/

void MainWindow::resizeEvent(QResizeEvent* event) {
    /* Qt's window resize event - keeps notification box centered while resizing */
    if (notification->isVisible()) {
        notification->adjustPosition();
    }
    event->accept();
}

void MainWindow::keyEventSlot(QKeyEvent* event) {
    /* receives cursor key events from the event-filter and passes them to the active tree widget but only when
     * the window is active and either the mono or color dithering tabs are active */
    if (!firstLoad &&
        isActiveWindow() && (ui->tabWidget->currentIndex() == TAB_INDEX_MONO ||
                             ui->tabWidget->currentIndex() == TAB_INDEX_COLOR)) {
        if (event->key() == Qt::Key_Plus) {
            ui->graphicsView->setZoomLevel(ui->graphicsView->getZoomLevel() + ZOOM_STEP_KEYBOARD, true);
        } else if (event->key() == Qt::Key_Minus) {
            ui->graphicsView->setZoomLevel(ui->graphicsView->getZoomLevel() - ZOOM_STEP_KEYBOARD, true);
        } else {
            activeTreeWidget->handleKeyPressEvent(event);
        }
    }
}

/*******************
 * GUI HELPERS     *
 *******************/

void MainWindow::enableGui(bool enable) {
    /* enables / disables UI items while the program is busy (e.g. when dithering) */
    ui->imageSettingsContainer->setEnabled(enable);
    // menu actions
    ui->actionOpen->setEnabled(enable);
    ui->actionSave->setEnabled(enable);
    ui->actionSaveAs->setEnabled(enable);
    ui->actionCopy->setEnabled(enable);
    ui->actionPaste->setEnabled(enable);
    ui->statusBarWidget->setEnabled(enable);
}

void MainWindow::setResetIcon(QPushButton* button, const QIcon* icon) {
    /* helper for setting QIcons on QPushButtons (e.g. for the reset button's X icon) */
    button->setIcon(*icon);
    button->setIconSize(resetButtonIconSize);
    button->setFixedSize(resetButtonSize);
}

QPixmap* MainWindow::loadSvg(const QString& fileName) {
    /* properly loads SVG images for HiDPI screens (avoids pixelation) */
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

void MainWindow::setMouseBusy(bool isBusy) {
    /* en/disables menu items and mouse interaction when the program is busy */
    if(isBusy) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        eventFilter.disableUi();
    } else {
        while(QApplication::overrideCursor()) {
            QApplication::restoreOverrideCursor(); // safeguard against accidental double-overrides
        }
        eventFilter.enableUi();
    }
    if(!firstLoad) {
        enableGui(!isBusy);
    }
}

void MainWindow::imageSettingsStackedWidgetIndexChangedSlot(int index) {
    /* adjusts the height of the 'Input Image Settings' group box, depending if dithering is in mono or in color */
    ui->imageSettingsStackedWidget->setMinimumHeight(index == IMAGE_SETTINGS_STACKED_WIDGET_MONO_INDEX ?
                     IMAGE_SETTINGS_STACKED_WIDGET_MONO_HEIGHT : IMAGE_SETTINGS_STACKED_WIDGET_COLOR_HEIGHT);
    ui->imageSettingsStackedWidget->setMaximumHeight(index == IMAGE_SETTINGS_STACKED_WIDGET_MONO_INDEX ?
                     IMAGE_SETTINGS_STACKED_WIDGET_MONO_HEIGHT : IMAGE_SETTINGS_STACKED_WIDGET_COLOR_HEIGHT);
}

void MainWindow::paletteSourceWidgetIndexChangedSlot(int index) {
    /* adjusts the height of the section where users can choose the source for the reduced palette */
    ui->paletteSourceWidget->setMinimumHeight(index == PALETTE_SOURCE_CUSTOM_INDEX ?
                    PALETTE_SOURCE_HEIGHT_CUSTOM : PALETTE_SOURCE_HEIGHT_DEFAULT);
    ui->paletteSourceWidget->setMaximumHeight(index == PALETTE_SOURCE_CUSTOM_INDEX ?
                    PALETTE_SOURCE_HEIGHT_CUSTOM : PALETTE_SOURCE_HEIGHT_DEFAULT);
}

void MainWindow::expandColorComparisonArea(bool expand) {
    /* helper for properly expanding color comparison options (e.g. for LAB comparison) */
    if (expand) {
        ui->widget_3->setMinimumHeight(COLOR_COMPARISON_HEIGHT_MAX);
        ui->widget_3->setMaximumHeight(COLOR_COMPARISON_HEIGHT_MAX);
    } else {
        ui->widget_3->setMinimumHeight(COLOR_COMPARISON_HEIGHT);
        ui->widget_3->setMaximumHeight(COLOR_COMPARISON_HEIGHT);
    }
    ui->resetHueWeightButton->setVisible(expand);
    ui->resetChromaWeightButton->setVisible(expand);
    ui->resetValueWeightButton->setVisible(expand);
    ui->spinBoxHue->setVisible(expand);
    ui->spinBoxChroma->setVisible(expand);
    ui->spinBoxValue->setVisible(expand);
    ui->hueWeightLabel->setVisible(expand);
    ui->chromaWeightLabel->setVisible(expand);
    ui->valueWeightLabel->setVisible(expand);
    ui->srcIlluminantCombo->setVisible(expand);
    ui->srcIlluminantLabel->setVisible(expand);
}

/**************************
 * GUI INITIALIZATION     *
 **************************/

void MainWindow::uiSetup() {
    /* main UI setup method - needs to be called first */
    ui->setupUi(this);
    setModernRedux(this);
    setWindowIcon(QIcon(":/resources/appicon.png"));
    resize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    setMinimumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    // QSplitter setup
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 0);
    ui->splitter->setSizes({-1, MIN_SPLITTER_WIDTH});
    ui->imageSettingsContainer->setMinimumWidth(MIN_SPLITTER_WIDTH);
    // set custom icon for image adjustment reset buttons
    QIcon reset_icon;
    QPixmap* pm_enabled = loadSvg(":/resources/times.svg");
    reset_icon.addPixmap(*pm_enabled, QIcon::Normal);
    delete pm_enabled;
    QPixmap* pm_disabled = loadSvg(":/resources/times_disabled.svg");
    reset_icon.addPixmap(*pm_disabled, QIcon::Disabled);
    delete pm_disabled;
    // set reset icon for 'reset color adjustment' buttons
    setResetIcon(ui->resetContrastButtonColor, &reset_icon);
    setResetIcon(ui->resetBrightnessButtonColor, &reset_icon);
    setResetIcon(ui->resetGammaButtonColor, &reset_icon);
    setResetIcon(ui->resetSaturationButtonColor, &reset_icon);
    setResetIcon(ui->resetContrastButtonMono, &reset_icon);
    setResetIcon(ui->resetBrightnessButtonMono, &reset_icon);
    setResetIcon(ui->resetGammaButtonMono, &reset_icon);
    setResetIcon(ui->resetMonoColors, &reset_icon);
    setResetIcon(ui->resetHueWeightButton, &reset_icon);
    setResetIcon(ui->resetChromaWeightButton, &reset_icon);
    setResetIcon(ui->resetValueWeightButton, &reset_icon);
    // Add items to MacOS application menu and about-window
    aboutWindow = new AboutWindow(this);
    setWindowTitle(aboutWindow->appTitle); // needs to be set up after the about window has been initialized
    QAction* actionAbout = new QAction(this);
    actionAbout->setText(tr("About Ditherista"));
    actionAbout->setMenuRole(QAction::ApplicationSpecificRole);
    connect(actionAbout, SIGNAL(triggered()), aboutWindow, SLOT(show()));
#if defined(__APPLE__) && defined(__MACH__)  // menubar for macOS
    QMenuBar* menubar = menuBar();
    QMenu* menu = new QMenu(this);
    menu->setTitle("Menu");
    menu->addAction(actionAbout);
    menubar->addMenu(menu);
    menuBar()->setNativeMenuBar(true);
#else  // menubar for other OSes
    ui->menuHelp->addAction(actionAbout);
    ui->menuFile->addSeparator();
    QAction* actionQuit = new QAction(this);
    actionQuit->setText(tr("Quit"));
    actionQuit->setMenuRole(QAction::ApplicationSpecificRole);
    actionQuit->setShortcut(QKeySequence(Qt::ALT | Qt::Key_F4));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    ui->menuFile->addAction(actionQuit);
#endif
    // set input validators for data input fields
    ui->paletteColorsEdit->setValidator(new QIntValidator(PALETTE_MIN_COLORS, PALETTE_MAX_COLORS, this));
}

void MainWindow::setApplicationDefaults() {
    /* sets various application default values */
    ui->imageSettingsContainer->setEnabled(false);
    // ditherer tab settings
    ui->tabWidget->setCurrentIndex(TAB_INDEX_MONO);  // start with: mono ditherer tab
    activeTreeWidget = ui->treeWidgetMono;           // start with: mono ditherer tab - this is the active tab
    ui->treeWidgetMono->setItemActive(0);      // make first mono ditherer the active mono ditherer
    ui->treeWidgetColor->setItemActive(0);     // make first color ditherer the active color ditherer
    ui->ditherSettings->setCurrentIndex(0);          // hide / show empty page for dither settings when no image loaded
    // palette GUI settings
    ui->paletteSourceWidget->setCurrentIndex(PALETTE_BUILT_IN); // start with: built-in palettes tab
    ui->predefinedPaletteCombo->setCurrentIndex(ui->predefinedPaletteCombo->findData(DEFAULT_PALETTE));
    colorComparisonMode = (enum ColorComparisonMode)DEFAULT_COLOR_COMPARISON_MODE;
    colorReductionMode = (enum QuantizationMethod)DEFAULT_COLOR_REDUCTION_MODE;
    ui->colorComparisonCombo->setCurrentIndex(ui->colorComparisonCombo->findData(DEFAULT_COLOR_COMPARISON_MODE));
    FloatColor_from_FloatColor(&srcIlluminant, &DEFAULT_ILLUMINANT); // set default LAB illuminant
    ui->srcIlluminantCombo->setCurrentIndex(4);
    ui->monoColorOneButton->setColor(DEFAULT_DARK_MONO_COLOR);
    ui->monoColorOneLabel->setColor(DEFAULT_DARK_MONO_COLOR);
    ui->monoColorTwoButton->setColor(DEFAULT_LIGHT_MONO_COLOR);
    ui->monoColorTwoLabel->setColor(DEFAULT_LIGHT_MONO_COLOR);
    // settings for 'Input Image Settings' group box
    ui->imageSettingsStackedWidget->setCurrentIndex(IMAGE_SETTINGS_STACKED_WIDGET_MONO_INDEX);
    ui->imageSettingsStackedWidget->setMinimumHeight(IMAGE_SETTINGS_STACKED_WIDGET_MONO_HEIGHT);
    ui->imageSettingsStackedWidget->setMaximumHeight(IMAGE_SETTINGS_STACKED_WIDGET_MONO_HEIGHT);
    // settings for palette source area (expandable built-in, from file, custom options)
    ui->paletteSourceWidget->setMinimumHeight(PALETTE_SOURCE_HEIGHT_DEFAULT);
    ui->paletteSourceWidget->setMaximumHeight(PALETTE_SOURCE_HEIGHT_DEFAULT);
    // settings for color comparison area (choose btw sRGB, Linear, LAB94, etc.)
    ui->widget_2->setMinimumHeight(COLOR_COMPARISON_HEIGHT_MAX);
    ui->widget_2->setMaximumHeight(COLOR_COMPARISON_HEIGHT_MAX);
}

void MainWindow::connectSignals() {
    /* connects all Qt signals */
    // connect signals - application menu
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(fileSaveSlot()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(fileSaveAsSlot()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(fileOpenSlot()));
    connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(copySlot()));
    connect(ui->actionPaste, SIGNAL(triggered()), this, SLOT(pasteSlot()));
    connect(ui->actionQuickTour, SIGNAL(triggered()), helpWindow, SLOT(showHelp()));
    connect(ui->actionChangelog, SIGNAL(triggered()), helpWindow, SLOT(showChangelog()));
    connect(ui->actionCheck_for_Updates, SIGNAL(triggered()), this, SLOT(updateCheckSlot()));
    connect(ui->menuEdit, SIGNAL(aboutToShow()), this, SLOT(editMenuAboutToShowSlot()));  // en/disable "paste" entry
    // connect signals - ditherer specific GUI controls / mono
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
    // connect signals - ditherer specific GUI controls / color
    connect(ui->ERR_C_serpentine, SIGNAL(toggled(bool)), this, SLOT(serpentineColorToggledSlot(bool)));
    connect(ui->ORD_IGR_C_step, SIGNAL(valueChanged(int)), this, SLOT(ORD_stepColorValueChangedSlot(int)), Qt::QueuedConnection);
    connect(ui->ORD_IGR_C_a, SIGNAL(valueChanged(double)), this, SLOT(ORD_IGR_aColorValueChangedSlot(double)), Qt::QueuedConnection);
    connect(ui->ORD_IGR_C_b, SIGNAL(valueChanged(double)), this, SLOT(ORD_IGR_bColorValueChangedSlot(double)), Qt::QueuedConnection);
    connect(ui->ORD_IGR_C_c, SIGNAL(valueChanged(double)), this, SLOT(ORD_IGR_cColorValueChangedSlot(double)), Qt::QueuedConnection);
    connect(ui->ORD_VAR_C_step, SIGNAL(valueChanged(int)), this, SLOT(ORD_stepColorValueChangedSlot(int)), Qt::QueuedConnection);
    // connect signals - image adjustments Mono
    connect(ui->resetContrastButtonMono, SIGNAL(clicked()), this, SLOT(resetContrastButtonMonoClickedSlot()));
    connect(ui->resetBrightnessButtonMono, SIGNAL(clicked()), this, SLOT(resetBrightnessButtonMonoClickedSlot()));
    connect(ui->resetGammaButtonMono, SIGNAL(clicked()), this, SLOT(resetGammaButtonMonoClickedSlot()));
    connect(ui->contrastEditMono, SIGNAL(valueChanged(double)), this, SLOT(contrastEditMonoEditingFinishedSlot(double)), Qt::QueuedConnection);
    connect(ui->brightnessEditMono, SIGNAL(valueChanged(double)), this, SLOT(brightnessEditMonoEditingFinishedSlot(double)), Qt::QueuedConnection);
    connect(ui->gammaEditMono, SIGNAL(valueChanged(double)), this, SLOT(gammaEditMonoEditingFinishedSlot(double)), Qt::QueuedConnection);
    connect(ui->contrastSliderMono, SIGNAL(valueChanged(int)), this, SLOT(contrastSliderMonoValueChangedSlot(int)), Qt::QueuedConnection);
    connect(ui->brightnessSliderMono, SIGNAL(valueChanged(int)), this, SLOT(brightnessSliderMonoValueChangedSlot(int)), Qt::QueuedConnection);
    connect(ui->gammaSliderMono, SIGNAL(valueChanged(int)), this, SLOT(gammaSliderMonoValueChangedSlot(int)), Qt::QueuedConnection);
    connect(ui->showOriginalMono, SIGNAL(stateChanged(int)), this, SLOT(showOriginalMonoButtonClickedSlot(int)));
    // connect signals - color image adjustments
    connect(ui->resetContrastButtonColor, SIGNAL(clicked()), this, SLOT(resetContrastButtonColorClickedSlot()));
    connect(ui->resetBrightnessButtonColor, SIGNAL(clicked()), this, SLOT(resetBrightnessButtonColorClickedSlot()));
    connect(ui->resetGammaButtonColor, SIGNAL(clicked()), this, SLOT(resetGammaButtonColorClickedSlot()));
    connect(ui->resetSaturationButtonColor, SIGNAL(clicked()), this, SLOT(resetSaturationButtonColorClickedSlot()));
    connect(ui->contrastEditColor, SIGNAL(valueChanged(double)), this, SLOT(contrastEditColorEditingFinishedSlot(double)), Qt::QueuedConnection);
    connect(ui->brightnessEditColor, SIGNAL(valueChanged(double)), this, SLOT(brightnessEditColorEditingFinishedSlot(double)), Qt::QueuedConnection);
    connect(ui->gammaEditColor, SIGNAL(valueChanged(double)), this, SLOT(gammaEditColorEditingFinishedSlot(double)), Qt::QueuedConnection);
    connect(ui->saturationEditColor, SIGNAL(valueChanged(double)), this, SLOT(saturationEditColorEditingFinishedSlot(double)), Qt::QueuedConnection);
    connect(ui->contrastSliderColor, SIGNAL(valueChanged(int)), this, SLOT(contrastSliderColorValueChangedSlot(int)), Qt::QueuedConnection);
    connect(ui->brightnessSliderColor, SIGNAL(valueChanged(int)), this, SLOT(brightnessSliderColorValueChangedSlot(int)), Qt::QueuedConnection);
    connect(ui->gammaSliderColor, SIGNAL(valueChanged(int)), this, SLOT(gammaSliderColorValueChangedSlot(int)), Qt::QueuedConnection);
    connect(ui->saturationSliderColor, SIGNAL(valueChanged(int)), this, SLOT(saturationSliderColorValueChangedSlot(int)), Qt::QueuedConnection);
    connect(ui->showOriginalColor, SIGNAL(stateChanged(int)), this, SLOT(showOriginalColorButtonClickedSlot(int)));
    // connect signals - mono palette color adjustments
    connect(ui->monoColorOneButton, SIGNAL(colorChanged(QColor)), this, SLOT(monoColorOneChangedSlot(QColor)));
    connect(ui->monoColorTwoButton, SIGNAL(colorChanged(QColor)), this, SLOT(monoColorTwoChangedSlot(QColor)));
    connect(ui->monoColorOneLabel, SIGNAL(colorChanged(QColor)), this, SLOT(monoColorOneChangedSlot(QColor)));
    connect(ui->monoColorTwoLabel, SIGNAL(colorChanged(QColor)), this, SLOT(monoColorTwoChangedSlot(QColor)));
    connect(ui->resetMonoColors, SIGNAL(clicked()), this, SLOT(resetMonoColorsClickedSlot()));
    // connect signals - right-side tab widget and components
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabWidgetChangedSlot(int)));
    connect(ui->treeWidgetColor, SIGNAL(itemChangedSignal(QTreeWidgetItem*)), this, SLOT(treeWidgetItemChangedSlot(QTreeWidgetItem*)));
    connect(ui->treeWidgetMono, SIGNAL(itemChangedSignal(QTreeWidgetItem*)), this, SLOT(treeWidgetItemChangedSlot(QTreeWidgetItem*)));
    // connect signals - batch rendering
    // connect(ui->treeWidgetMono, SIGNAL(batchDitherSignal()), this, SLOT(batchDitherRequestedSlot())); // TODO temporarily disabled
    // connect signals - palette tab
    connect(ui->paletteSourceCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(paletteSourceComboChangedSlot(int)));
    connect(ui->predefinedPaletteCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(predefinedPaletteComboChangedSlot(int)));
    connect(ui->colorComparisonCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(colorComparisonComboChangedSlot(int)));
    connect(ui->graphicsView, SIGNAL(loadImageSignal(QString)), this, SLOT(loadImageFromFileSlot(QString)));
    connect(ui->paletteBrowseButton, SIGNAL(clicked()), this, SLOT(paletteBrowseButtonClickedSlot()));
    connect(ui->palettePathEdit, SIGNAL(editingFinished()), this, SLOT(palettePathEditEditingFinishedSlot()));
    connect(ui->paletteColorsEdit, SIGNAL(editingFinished()), this, SLOT(paletteColorsEditEditingFinishedSlot()));
    connect(ui->palGenUniqueColorsCheck, SIGNAL(stateChanged(int)), this, SLOT(paletteIncludeExtremeColorsSlot(int)));
    connect(ui->palGenBWCheck, SIGNAL(stateChanged(int)), this, SLOT(paletteIncludeExtremeColorsSlot(int)));
    connect(ui->palGenRGBCheck, SIGNAL(stateChanged(int)), this, SLOT(paletteIncludeExtremeColorsSlot(int)));
    connect(ui->palGenCMYCheck, SIGNAL(stateChanged(int)), this, SLOT(paletteIncludeExtremeColorsSlot(int)));
    connect(ui->colorReductionCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(colorReductionComboChangedSlot(int)));
    connect(ui->paletteSourceWidget, SIGNAL(currentChanged(int)), this, SLOT(paletteSourceWidgetIndexChangedSlot(int)));
    connect(ui->imageSettingsStackedWidget, SIGNAL(currentChanged(int)), this, SLOT(imageSettingsStackedWidgetIndexChangedSlot(int)));
    connect(ui->spinBoxValue, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChangedSlot(double)), Qt::QueuedConnection);
    connect(ui->spinBoxHue, SIGNAL(valueChanged(double)), this, SLOT(spinBoxHueChangedSlot(double)), Qt::QueuedConnection);
    connect(ui->spinBoxChroma, SIGNAL(valueChanged(double)), this, SLOT(spinBoxChromaChangedSlot(double)), Qt::QueuedConnection);
    connect(ui->srcIlluminantCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(srcIlluminantComboChangedSlot(int)));
    connect(ui->colorListWidget, SIGNAL(colorChanged(int, QColor)), this, SLOT(paletteColorChangedSlot(int, QColor)));
    connect(ui->resetHueWeightButton, SIGNAL(clicked()), this, SLOT(resetHueWeightButtonClickedSlot()));
    connect(ui->resetChromaWeightButton, SIGNAL(clicked()), this, SLOT(resetChromaWeightButtonClickedSlot()));
    connect(ui->resetValueWeightButton, SIGNAL(clicked()), this, SLOT(resetValueWeightButtonClickedSlot()));
    connect(ui->savePaletteButton, SIGNAL(clicked()), this, SLOT(savePaletteButtonClickedSlot()));

    connect(ui->graphicsView, SIGNAL(zoomLevelChangedSignal(int)), this, SLOT(zoomLevelChangedSlot(int)));
    connect(ui->zoomLevelCombo, SIGNAL(currentComboIndexChanged(int)), this, SLOT(zoomLevelComboCurrentIndexChangedSlot(int)));
    connect(ui->zoomLevelCombo->lineEdit(), SIGNAL(editingFinished()), this, SLOT(zoomLevelEditingFinishedSlot()));
    ui->zoomLevelCombo->lineEdit()->setFocusPolicy(Qt::ClickFocus);

    // install event filter
    connect(&eventFilter, SIGNAL(keyEventSignal(QKeyEvent*)), this, SLOT(keyEventSlot(QKeyEvent*)));
    QApplication::instance()->installEventFilter(&eventFilter);
}

void MainWindow::zoomLevelChangedSlot(int zoomLevel) {
    /* updates the text in the zoom level combo-box; called from graphicsView */
    if (ui->zoomLevelCombo->isEnabled()) {
        whileBlocking(ui->zoomLevelCombo)->setEditText(QVariant(zoomLevel).toString() + "%");
    }
}

void MainWindow::zoomLevelComboCurrentIndexChangedSlot(int index) {
    /* user selects a zoom level from the zoom level combobox */
    if (ui->statusBarWidget->isEnabled()) {
        switch (index) {
            case 0: ui->graphicsView->setZoomLevel(50, false); break;
            case 1: ui->graphicsView->setZoomLevel(100, false); break;
            case 2: ui->graphicsView->setZoomLevel(200, false); break;
            case 3: ui->graphicsView->setZoomLevel(300, false); break;
            case 4: ui->graphicsView->setZoomLevel(400, false); break;
            case 5: ui->graphicsView->setZoomLevel(500, false); break;
        }
        ui->graphicsView->setFocus();
    }
}

void MainWindow::zoomLevelEditingFinishedSlot() {
    /* user keyed in a desired zoom level; this function checks if it's a valid value */
    QString zl = ui->zoomLevelCombo->lineEdit()->text().trimmed();
    if (zl.contains("%", Qt::CaseInsensitive)) {
        zl.replace("%", "", Qt::CaseInsensitive);
    }
    bool ok = false;
    int zoomLevel = QVariant(zl).toInt(&ok);
    if (ok) {
        ui->graphicsView->setZoomLevel(zoomLevel, true);
    }
}

void MainWindow::setDithererDefaults() {
    /* sets sensible default values for all color and mono ditherers */
    ui->treeWidgetMono->setValue(ALL_ALL, setting_randomize, false);  // allebach
    ui->treeWidgetMono->setValue(DBS_DBS, setting_formula, 1);        // DBS
    ui->treeWidgetMono->setValue(ERR_XOT, setting_serpentine, true);  // error diffusion XOT
    ui->treeWidgetMono->setValue(ERR_XOT, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_DIA, setting_serpentine, false); // error diffusion diagonal
    ui->treeWidgetMono->setValue(ERR_DIA, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_FYD, setting_serpentine, false); // error diffusion floyd steinberg
    ui->treeWidgetMono->setValue(ERR_FYD, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_SF1, setting_serpentine, false);  // error diffusion shiau fan 1
    ui->treeWidgetMono->setValue(ERR_SF1, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_SF2, setting_serpentine, true);  // error diffusion shiau fan 2
    ui->treeWidgetMono->setValue(ERR_SF2, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_SF3, setting_serpentine, false);  // error diffusion shiau fan 3
    ui->treeWidgetMono->setValue(ERR_SF3, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_STK, setting_serpentine, false);  // error diffusion stucki
    ui->treeWidgetMono->setValue(ERR_STK, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_D1D, setting_serpentine, true);   // error diffusion 1D
    ui->treeWidgetMono->setValue(ERR_D1D, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_D2D, setting_serpentine, true);   // error diffusion 2D
    ui->treeWidgetMono->setValue(ERR_D2D, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_FFS, setting_serpentine, false);  // error diffusion fake floyd steinberg
    ui->treeWidgetMono->setValue(ERR_FFS, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_JJN, setting_serpentine, false);  // error diffusion jarvis judice ninke
    ui->treeWidgetMono->setValue(ERR_JJN, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_ATK, setting_serpentine, false);  // error diffusion atkinson
    ui->treeWidgetMono->setValue(ERR_ATK, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_BRK, setting_serpentine, false);  // error diffusion burkes
    ui->treeWidgetMono->setValue(ERR_BRK, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_SI3, setting_serpentine, false);  // error diffusion sierra3
    ui->treeWidgetMono->setValue(ERR_SI3, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_SI2, setting_serpentine, false);  // error diffusion sierra 2 row
    ui->treeWidgetMono->setValue(ERR_SI2, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_SIL, setting_serpentine, true);   // error diffusion sierra lite
    ui->treeWidgetMono->setValue(ERR_SIL, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_SPG, setting_serpentine, false);  // error diffusion steve pigeon
    ui->treeWidgetMono->setValue(ERR_SPG, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_RKI, setting_serpentine, false);  // error diffusion robert kist
    ui->treeWidgetMono->setValue(ERR_RKI, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ERR_SSA, setting_serpentine, true);   // error diffusion stevenson-arce
    ui->treeWidgetMono->setValue(ERR_SSA, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(GRD_GRD, setting_width, 4);           // grid ditherer
    ui->treeWidgetMono->setValue(GRD_GRD, setting_height, 4);
    ui->treeWidgetMono->setValue(GRD_GRD, setting_min_pixels, 0);
    ui->treeWidgetMono->setValue(GRD_GRD, setting_alt_algorithm, true);
    ui->treeWidgetMono->setValue(ORD_BLU, setting_jitter, 0.0);        // ordered dither blue noise
    ui->treeWidgetMono->setValue(ORD_BA2, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_BA3, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_BA4, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_BA8, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_B16, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_B32, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_DD1, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_DD2, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_VDD, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_NR1, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_NR2, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_NR3, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_NR4, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_ULY, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_UB5, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_CD1, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_CD2, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_CD3, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_CD4, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_CD5, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_CD6, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_CD7, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_CD8, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_CD9, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_CDA, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_CDB, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_CWP, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_BWP, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_DIO, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_UCD, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_M5C, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_M6C, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_M7C, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_M44, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_M64, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_M84, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_MA4, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_MA6, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_MA8, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_VA2, setting_step, 55);         // ordered dither variable 2x2
    ui->treeWidgetMono->setValue(ORD_VA2, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_VA4, setting_step, 14);         // ordered dither variable 4x4
    ui->treeWidgetMono->setValue(ORD_VA4, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_IGR, setting_step, 4);          // ordered dither interleaved gradient
    ui->treeWidgetMono->setValue(ORD_IGR, setting_jitter, 0.0);
    ui->treeWidgetMono->setValue(ORD_IGR, setting_a, 52.9829189f);
    ui->treeWidgetMono->setValue(ORD_IGR, setting_b, 0.06711056f);
    ui->treeWidgetMono->setValue(ORD_IGR, setting_c, 0.00583715f);
    ui->treeWidgetMono->setValue(RIM_HIL, setting_use_riemersma, false); // riemersma hilbert curve
    ui->treeWidgetMono->setValue(RIM_FS0, setting_use_riemersma, false);
    ui->treeWidgetMono->setValue(RIM_FS1, setting_use_riemersma, false);
    ui->treeWidgetMono->setValue(RIM_FS2, setting_use_riemersma, false);
    ui->treeWidgetMono->setValue(RIM_FSS, setting_use_riemersma, false);
    ui->treeWidgetMono->setValue(RIM_GOS, setting_use_riemersma, false);
    ui->treeWidgetMono->setValue(RIM_HIM, setting_use_riemersma, false);
    ui->treeWidgetMono->setValue(RIM_PEA, setting_use_riemersma, false);
    ui->treeWidgetMono->setValue(THR_THR, setting_jitter, 0.55f);        // threshold
    ui->treeWidgetMono->setValue(THR_THR, setting_threshold, 0.5f);
    ui->treeWidgetMono->setValue(THR_THR, setting_auto_threshold, true);
    ui->treeWidgetMono->setValue(VAR_OST, setting_serpentine, true);     // variable error diffusion ostromoukhov
    ui->treeWidgetMono->setValue(VAR_ZHF, setting_serpentine, true);
    ui->THR_threshold->setEnabled(false);  // because we're setting auto-threshold as default for THR ditherer
    // color dither default settings
    ui->treeWidgetColor->setValue(ORD_VA2_C, setting_step, 90);         // ordered dither variable 2x2
    ui->treeWidgetColor->setValue(ORD_VA4_C, setting_step, 40);         // ordered dither variable 4x4
    ui->treeWidgetColor->setValue(ORD_IGR_C, setting_step, 4);
    ui->treeWidgetColor->setValue(ORD_IGR_C, setting_a, 52.9829189f);
    ui->treeWidgetColor->setValue(ORD_IGR_C, setting_b, 0.06711056f);
    ui->treeWidgetColor->setValue(ORD_IGR_C, setting_c, 0.00583715f);
}

void MainWindow::addPredefinedPalettes() {
    /* adds a built-in palettes to the build-in palette combo box in the palette tab */
    ui->predefinedPaletteCombo->addItem(tr("Computer Systems"));
    ui->predefinedPaletteCombo->setItemData(ui->predefinedPaletteCombo->count() - 1, false, Qt::UserRole -1);
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("CGA 1 (4)"), "p_cga0");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("CGA 2 (4)"), "p_cga1");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("CGA 3 (4)"), "p_cga2");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("TRS-80 1 (4)"), "p_trs80_1");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("TRS-80 2 (4)"), "p_trs80_2");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Teletext (8)"), "p_teletext");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Sam Coupé (14)"), "p_samcoupe");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("MSX (15)"), "p_msx");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("ZX Spectrum (15)"), "p_zx");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("EGA (16)"), "p_ega");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("VIC 20 (16)"), "p_vic20");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("C64 1 (16)"), "p_c64");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("C64 2 (16)"), "p_c64_2");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Risc OS 3 (16)"), "p_riscos");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Macintosh (16)"), "p_mac16");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Windows (16)"), "p_win_16col");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Amstrad CPC (27)"), "p_amstrad");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Windows 95 (256)"), "p_win95");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Macintosh (256)"), "p_mac256");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("MSX 2 (256)"), "p_msx2");
    ui->predefinedPaletteCombo->insertSeparator(9999);
    ui->predefinedPaletteCombo->addItem(tr("Applications"));
    ui->predefinedPaletteCombo->setItemData(ui->predefinedPaletteCombo->count() - 1, false, Qt::UserRole -1);
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("MagicWB Amiga (8)"), "p_magicwb");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Degas Elite (16)"), "p_degas_elite");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("MS Paint Windows 7 (20)"), "p_mspaint_win7");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("MS Paint XP (28)"), "p_mspaint");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("MS Paint Windows Vista (28)"), "p_mspaint_vista");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Deluxe Paint Amiga (32)"), "d_paint_32");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Micrografx Draw'n'Publish (58)"), "p_micrografx");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Deluxe Paint Amiga EHB (63)"), "p_dpaint_ehb");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Deluxe Paint PC/AGA (223)"), "d_paint_256");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Autodesk Animator (251)"), "p_aa");
    ui->predefinedPaletteCombo->insertSeparator(9999);
    ui->predefinedPaletteCombo->addItem(tr("Consoles"));
    ui->predefinedPaletteCombo->setItemData(ui->predefinedPaletteCombo->count() - 1, false, Qt::UserRole -1);
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Playdate (2)"), "p_playdate");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Gameboy (4)"), "p_gameboy");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Gameboy Pocket (4)"), "p_gameboy_pocket");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Fairchild Channel F (8)"), "p_fairchild");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("ColecoVision (15)"), "p_colecovision");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Pico-8 (16)"), "p_pico8");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Magnavox Odyssey 2 (16)"), "p_magnavox");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Mattel Intellivision (16)"), "p_intellivision");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("NES (55)"), "p_nes");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Sega Master System (64)"), "p_sms");
    ui->predefinedPaletteCombo->insertSeparator(9999);
    ui->predefinedPaletteCombo->addItem(tr("Other Palettes"));
    ui->predefinedPaletteCombo->setItemData(ui->predefinedPaletteCombo->count() - 1, false, Qt::UserRole -1);
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("RGB 4 bit (16)"), "p_rgb_4bit");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("RGB 6 bit (64)"), "p_rgb_6bit");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("RGB 9 bit (512)"), "p_rgb_9bit");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("RGB 6 levels (216)"), "p_6rgb");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("RGB 6-8-5 levels (240)"), "p_685rgb");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("RGB 6-7-6 levels (252)"), "p_676rgb");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("RGB 8-8-4 levels (256)"), "p_884rgb");
    ui->predefinedPaletteCombo->insertSeparator(9999);
    ui->predefinedPaletteCombo->addItem(tr("Gradients"));
    ui->predefinedPaletteCombo->setItemData(ui->predefinedPaletteCombo->count() - 1, false, Qt::UserRole -1);
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("black and white (2)"), "g_bw");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("grayscale (4)"), "g_grayscale");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Citrink by Inkpendude (8)"), "g_citrink");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("Paper 8 by Frosty Rabbid (8)"), "g_paper8");
    ui->predefinedPaletteCombo->addItem(QIcon(":/resources/img_spacer.svg"), tr("SPACE DUST 9 by Hana (9)"), "g_spacedust9");
}

void MainWindow::populateColorComparisonCombo() {
    /* adds entries to the color comparison combo box in the palette tab */
    ui->colorComparisonCombo->insertItem(1, tr("LAB '76"), (int)LAB76);
    ui->colorComparisonCombo->insertItem(2, tr("LAB '94"), (int)LAB94);
    ui->colorComparisonCombo->insertItem(3, tr("LAB '2000"), (int)LAB2000);
    ui->colorComparisonCombo->insertItem(4, tr("Luminance"), (int)LUMINANCE);
    ui->colorComparisonCombo->insertItem(5, tr("HSV"), (int)HSV);
    ui->colorComparisonCombo->insertItem(6, tr("sRGB"), (int)SRGB);
    ui->colorComparisonCombo->insertItem(7, tr("sRGB CCIR"), (int)SRGB_CCIR);
    ui->colorComparisonCombo->insertItem(8, tr("Linear"), (int)LINEAR);
    ui->colorComparisonCombo->insertItem(9, tr("Linear CCIR"), (int)LINEAR_CCIR);
    ui->colorComparisonCombo->insertItem(10, tr("Tetrapal"), (int)TETRAPAL);
    // populates the LAB source illuminant combo
    ui->srcIlluminantCombo->insertItem(1, tr("D93"));
    ui->srcIlluminantCombo->insertItem(2, tr("D75"));
    ui->srcIlluminantCombo->insertItem(3, tr("D65"));
    ui->srcIlluminantCombo->insertItem(4, tr("D55"));
    ui->srcIlluminantCombo->insertItem(5, tr("D50"));
    ui->srcIlluminantCombo->insertItem(6, tr("A"));
    ui->srcIlluminantCombo->insertItem(7, tr("B"));
    ui->srcIlluminantCombo->insertItem(8, tr("C"));
    ui->srcIlluminantCombo->insertItem(9, tr("E (base)"));
    ui->srcIlluminantCombo->insertItem(10, tr("F1"));
    ui->srcIlluminantCombo->insertItem(11, tr("F2"));
    ui->srcIlluminantCombo->insertItem(12, tr("F3"));
    ui->srcIlluminantCombo->insertItem(13, tr("F7"));
    ui->srcIlluminantCombo->insertItem(14, tr("F11"));
}

void MainWindow::populateDithererSelection() {
    /* populates the lists of color and mono ditherers */
    // mono ditherers - Allebach
    ui->treeWidgetMono->addTreeItem(ALL, ALL_ALL, tr("Allebach"));
    // mono ditherers - DBS
    ui->treeWidgetMono->addTreeItem(DBS, DBS_DBS, tr("DBS"));
    // mono ditherers - Dot diffusion
    ui->treeWidgetMono->addTreeItem(DOT, DOT_G08, tr("GuoLiu 8x8"));
    ui->treeWidgetMono->addTreeItem(DOT, DOT_G16, tr("GuoLiu 16x16"));
    ui->treeWidgetMono->addTreeItem(DOT, DOT_ISP, tr("Inverted Spiral"));
    ui->treeWidgetMono->addTreeItem(DOT, DOT_KNU, tr("Knuth"));
    ui->treeWidgetMono->addTreeItem(DOT, DOT_M08, tr("Mese 8x8"));
    ui->treeWidgetMono->addTreeItem(DOT, DOT_M16, tr("Mese 16x16"));
    ui->treeWidgetMono->addTreeItem(DOT, DOT_MKN, tr("Mini Knuth"));
    ui->treeWidgetMono->addTreeItem(DOT, DOT_OKN, tr("Optimized Knuth"));
    ui->treeWidgetMono->addTreeItem(DOT, DOT_SPR, tr("Spiral"));
    // mono ditherers - Error diffusion
    ui->treeWidgetMono->addTreeItem(ERR, ERR_ATK, tr("Atkinson"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_BRK, tr("Burkes"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_DIA, tr("Diagonal Dither"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_D1D, tr("Error Diffusion 1D"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_D2D, tr("Error Diffusion 2D"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_FFS, tr("Fake Floyd Steinberg"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_FYD, tr("Floyd Steinberg"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_JJN, tr("Jarvis-Judice-Ninke"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_RKI, tr("Robert Kist"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_SF1, tr("Shiau Fan 1"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_SF2, tr("Shiau Fan 2"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_SF3, tr("Shiau Fan 3"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_SI2, tr("Sierra 2-Row"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_SI3, tr("Sierra 3"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_SIL, tr("Sierra Lite"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_SSA, tr("Stevenson-Arce"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_SPG, tr("Steve Pigeon"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_STK, tr("Stucki"));
    ui->treeWidgetMono->addTreeItem(ERR, ERR_XOT, tr("Xot Diffusion"));
    // mono ditherers - dot diffusion
    ui->treeWidgetMono->addTreeItem(GRD, GRD_GRD, tr("Grid"));
    // mono ditherers - dot lippens
    ui->treeWidgetMono->addTreeItem(LIP, LIP_GUO, tr("GuoLiu 16x16"));
    ui->treeWidgetMono->addTreeItem(LIP, LIP_KNU, tr("Knuth"));
    ui->treeWidgetMono->addTreeItem(LIP, LIP_LI1, tr("Lippens 1"));
    ui->treeWidgetMono->addTreeItem(LIP, LIP_LI2, tr("Lippens 2"));
    ui->treeWidgetMono->addTreeItem(LIP, LIP_LI3, tr("Lippens 3"));
    ui->treeWidgetMono->addTreeItem(LIP, LIP_MSE, tr("Mese 16x16"));
    // mono ditherers - ordered
    ui->treeWidgetMono->addTreeItem(ORD, ORD_BA2, tr("Bayer 2x2"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_BA3, tr("Bayer 3x3"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_BA4, tr("Bayer 4x4"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_BA8, tr("Bayer 8x8"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_B16, tr("Bayer 16x16"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_B32, tr("Bayer 32x32"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_BLU, tr("Blue Noise"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_CD1, tr("Clustered Dot 1"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_CD2, tr("Clustered Dot 2"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_CD3, tr("Clustered Dot 3"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_CD4, tr("Clustered Dot 4"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_CD5, tr("Clustered Dot 5"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_CD6, tr("Clustered Dot 6"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_CD7, tr("Clustered Dot 7"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_CD8, tr("Clustered Dot 8"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_CD9, tr("Clustered Dot 9"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_CDA, tr("Clustered Dot 10"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_CDB, tr("Clustered Dot 11"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_DIO, tr("Diagonal Ordered"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_DD1, tr("Dispersed Dots 1"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_DD2, tr("Dispersed Dots 2"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_IGR, tr("Interleaved Gradient"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_M5C, tr("Magic 5x5 Circle"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_M6C, tr("Magic 6x6 Circle"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_M7C, tr("Magic 7x7 Circle"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_M44, tr("Magic 4x4 45"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_M64, tr("Magic 6x6 45"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_M84, tr("Magic 8x8 45"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_MA4, tr("Magic 4x4"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_MA6, tr("Magic 6x6"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_MA8, tr("Magic 8x8"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_NR1, tr("Non-Rectangular 1"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_NR2, tr("Non-Rectangular 2"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_NR3, tr("Non-Rectangular 3"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_NR4, tr("Non-Rectangular 4"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_ULY, tr("Ulichney"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_UB5, tr("Ulichney Bayer 5x5"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_UCD, tr("Ulichney Cluster Dot"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_VA2, tr("Variable 2x2"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_VA4, tr("Variable 4x4"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_VDD, tr("Void Dispersed Dots"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_CWP, tr("White-Point (Central)"));
    ui->treeWidgetMono->addTreeItem(ORD, ORD_BWP, tr("White-Point (Balanced)"));
    // mono ditherers - Pattern
    ui->treeWidgetMono->addTreeItem(PAT, PAT_22A, tr("2x2 Pattern"));
    ui->treeWidgetMono->addTreeItem(PAT, PAT_33A, tr("3x3 Pattern 1"));
    ui->treeWidgetMono->addTreeItem(PAT, PAT_33B, tr("3x3 Pattern 2"));
    ui->treeWidgetMono->addTreeItem(PAT, PAT_33C, tr("3x3 Pattern 3"));
    ui->treeWidgetMono->addTreeItem(PAT, PAT_44A, tr("4x4 Pattern"));
    ui->treeWidgetMono->addTreeItem(PAT, PAT_52A, tr("5x2 Pattern"));
    // mono ditherers - Rimersma
    ui->treeWidgetMono->addTreeItem(RIM, RIM_FS0, tr("Fass 0 Curve"));
    ui->treeWidgetMono->addTreeItem(RIM, RIM_FS1, tr("Fass 1 Curve"));
    ui->treeWidgetMono->addTreeItem(RIM, RIM_FS2, tr("Fass 2 Curve"));
    ui->treeWidgetMono->addTreeItem(RIM, RIM_FSS, tr("Fass Spiral"));
    ui->treeWidgetMono->addTreeItem(RIM, RIM_GOS, tr("Gosper Curve"));
    ui->treeWidgetMono->addTreeItem(RIM, RIM_HIL, tr("Hilbert Curve"));
    ui->treeWidgetMono->addTreeItem(RIM, RIM_HIM, tr("Hilbert Mod Curve"));
    ui->treeWidgetMono->addTreeItem(RIM, RIM_PEA, tr("Peano Curve"));
    // mono ditherers - Thresholding
    ui->treeWidgetMono->addTreeItem(THR, THR_THR, tr("Threshold Dither"));
    // mono ditherers - Variable error diffusion
    ui->treeWidgetMono->addTreeItem(VAR, VAR_OST, tr("Ostromoukhov"));
    ui->treeWidgetMono->addTreeItem(VAR, VAR_ZHF, tr("ZhouFang"));
    // color ditherers - Error diffusion
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_ATK_C, tr("Atkinson"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_BRK_C, tr("Burkes"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_DIA_C, tr("Diagonal Dither"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_D1D_C, tr("Error Diffusion 1D"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_D2D_C, tr("Error Diffusion 2D"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_FFS_C, tr("Fake Floyd Steinberg"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_FYD_C, tr("Floyd Steinberg"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_JJN_C, tr("Jarvis-Judice-Ninke"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_RKI_C, tr("Robert Kist"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_SF1_C, tr("Shiau Fan 1"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_SF2_C, tr("Shiau Fan 2"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_SF3_C, tr("Shiau Fan 3"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_SI2_C, tr("Sierra 2-Row"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_SI3_C, tr("Sierra 3"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_SIL_C, tr("Sierra Lite"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_SSA_C, tr("Stevenson-Arce"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_SPG_C, tr("Steve Pigeon"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_STK_C, tr("Stucki"));
    ui->treeWidgetColor->addTreeItem(ERR_C, ERR_XOT_C, tr("Xot Diffusion"));
    // color ditherers - Ordered dithering
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_BLU_C, tr("Blue Noise"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_BA2_C, tr("Bayer 2x2"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_BA3_C, tr("Bayer 3x3"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_BA4_C, tr("Bayer 4x4"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_BA8_C, tr("Bayer 8x8"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_B16_C, tr("Bayer 16x16"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_B32_C, tr("Bayer 32x32"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_CD1_C, tr("Clustered Dot 1"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_CD2_C, tr("Clustered Dot 2"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_CD3_C, tr("Clustered Dot 3"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_CD4_C, tr("Clustered Dot 4"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_CD5_C, tr("Clustered Dot 5"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_CD6_C, tr("Clustered Dot 6"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_CD7_C, tr("Clustered Dot 7"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_CD8_C, tr("Clustered Dot 8"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_CD9_C, tr("Clustered Dot 9"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_CDA_C, tr("Clustered Dot 10"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_CDB_C, tr("Clustered Dot 11"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_DIO_C, tr("Diagonal Ordered"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_DD1_C, tr("Dispersed Dots 1"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_DD2_C, tr("Dispersed Dots 2"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_IGR_C, tr("Interleaved Gradient"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_M5C_C, tr("Magic 5x5 Circle"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_M6C_C, tr("Magic 6x6 Circle"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_M7C_C, tr("Magic 7x7 Circle"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_M44_C, tr("Magic 4x4 45"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_M64_C, tr("Magic 6x6 45"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_M84_C, tr("Magic 8x8 45"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_MA4_C, tr("Magic 4x4"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_MA6_C, tr("Magic 6x6"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_MA8_C, tr("Magic 8x8"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_NR1_C, tr("Non-Rectangular 1"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_NR2_C, tr("Non-Rectangular 2"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_NR3_C, tr("Non-Rectangular 3"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_NR4_C, tr("Non-Rectangular 4"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_ULY_C, tr("Ulichney"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_UB5_C, tr("Ulichney Bayer 5x5"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_UCD_C, tr("Ulichney Cluster Dot"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_VA2_C, tr("Variable 2x2"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_VA4_C, tr("Variable 4x4"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_VDD_C, tr("Void Dispersed Dots"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_CWP_C, tr("White-Point (Central)"));
    ui->treeWidgetColor->addTreeItem(ORD_C, ORD_BWP_C, tr("White-Point (Balanced)"));
}
