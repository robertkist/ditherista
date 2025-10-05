#include "mainwindow.h"
#include "consts.h"
#include "ui_elements/signalblocker.h"
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QtConcurrent>
#include <QMessageBox>

/* This file contains:
 * - color palette related methods
 */

/************************************
 * LAB COLOR PARAMETER HANDLING     *
 ************************************/

void MainWindow::srcIlluminantComboChangedSlot(int index) {
    /* user changed the LAB illuminant combo */
    switch (index) {
        case 1: FloatColor_from_FloatColor(&srcIlluminant, &D93_XYZ); break;
        case 2: FloatColor_from_FloatColor(&srcIlluminant, &D75_XYZ); break;
        case 3: FloatColor_from_FloatColor(&srcIlluminant, &D65_XYZ); break;
        case 4: FloatColor_from_FloatColor(&srcIlluminant, &D55_XYZ); break;
        case 5: FloatColor_from_FloatColor(&srcIlluminant, &D50_XYZ); break;
        case 6: FloatColor_from_FloatColor(&srcIlluminant, &A_XYZ); break;
        case 7: FloatColor_from_FloatColor(&srcIlluminant, &B_XYZ); break;
        case 8: FloatColor_from_FloatColor(&srcIlluminant, &C_XYZ); break;
        case 9: FloatColor_from_FloatColor(&srcIlluminant, &E_XYZ); break;
        case 10: FloatColor_from_FloatColor(&srcIlluminant, &F1_XYZ); break;
        case 11: FloatColor_from_FloatColor(&srcIlluminant, &F2_XYZ); break;
        case 12: FloatColor_from_FloatColor(&srcIlluminant, &F3_XYZ); break;
        case 13: FloatColor_from_FloatColor(&srcIlluminant, &F7_XYZ); break;
        case 14: FloatColor_from_FloatColor(&srcIlluminant, &F11_XYZ); break;
    }
    generateCachedPalette(true, false, false);
}

void MainWindow::resetLabHCVspinBoxes() {
    /* resets LAB weights to their default values */
    whileBlocking(ui->spinBoxValue)->setValue(cachedPalette->lab_weights.v);
    whileBlocking(ui->spinBoxChroma)->setValue(cachedPalette->lab_weights.c);
    whileBlocking(ui->spinBoxHue)->setValue(cachedPalette->lab_weights.h);
}

void MainWindow::labHCVChanged() {
    /* refresh/re-dither the current image after user changed LAB weight settings */
    CachedPalette_free_cache(cachedPalette); // no need the re-gen entire palette if LAB weights change
    imageHashColor.clearAllDitheredImages();
    ui->treeWidgetColor->clearAllDitherFlags();
    reDither(true);
}

void MainWindow::spinBoxValueChangedSlot(double value) {
    /* user changed LAB color Value weight spin-box */
    cachedPalette->lab_weights.v = value;
    labHCVChanged();
}

void MainWindow::resetValueWeightButtonClickedSlot() {
    /* user pressed the LAB color Value reset button */
    whileBlocking(ui->spinBoxValue)->setValue(LAB_W_VALUE);
    spinBoxValueChangedSlot(LAB_W_VALUE);
}

void MainWindow::spinBoxHueChangedSlot(double hue) {
    /* user changed LAB color Hue weight spin-box */
    cachedPalette->lab_weights.h = hue;
    labHCVChanged();
}

void MainWindow::resetHueWeightButtonClickedSlot() {
    /* user pressed the LAB color Hue reset button */
    whileBlocking(ui->spinBoxHue)->setValue(LAB_W_HUE);
    spinBoxHueChangedSlot(LAB_W_HUE);
}

void MainWindow::spinBoxChromaChangedSlot(double chroma) {
    /* user changed LAB color Chroma weight spin-box */
    cachedPalette->lab_weights.c = chroma;
    labHCVChanged();
}

void MainWindow::resetChromaWeightButtonClickedSlot() {
    /* user pressed the LAB color Chroma reset button */
    whileBlocking(ui->spinBoxChroma)->setValue(LAB_W_CHROMA);
    spinBoxChromaChangedSlot(LAB_W_CHROMA);
}

/************************************
 * MONO-PALETTE / MONO DITHERING    *
 ************************************/

void MainWindow::monoColorOneChangedSlot(QColor color) {
    /* user changed one of the mono colors */
    ui->monoColorOneLabel->setColor(color);
    ui->monoColorOneButton->setColor(color);
    ui->resetMonoColors->setEnabled(color != DEFAULT_DARK_MONO_COLOR || ui->monoColorTwoButton->getColor() != DEFAULT_LIGHT_MONO_COLOR);
    setDitherImageMono();
}

void MainWindow::monoColorTwoChangedSlot(QColor color) {
    /* user changed one of the mono colors */
    ui->monoColorTwoLabel->setColor(color);
    ui->monoColorTwoButton->setColor(color);
    ui->resetMonoColors->setEnabled(ui->monoColorOneButton->getColor() != DEFAULT_DARK_MONO_COLOR || color != DEFAULT_LIGHT_MONO_COLOR);
    setDitherImageMono();
}

void MainWindow::resetMonoColorsClickedSlot() {
    /* use clicked the mono palette color's reset button */
    ui->monoColorOneLabel->setColor(DEFAULT_DARK_MONO_COLOR);
    ui->monoColorOneButton->setColor(DEFAULT_DARK_MONO_COLOR);
    ui->monoColorTwoLabel->setColor(DEFAULT_LIGHT_MONO_COLOR);
    ui->monoColorTwoButton->setColor(DEFAULT_LIGHT_MONO_COLOR);
    ui->resetMonoColors->setEnabled(false);
    setDitherImageMono();
}

/************************************
 * PAINT.NET PALETTE FILE I/O       *
 ************************************/

void MainWindow::savePaletteButtonClickedSlot() {
    /* saves the palette to a Paint.NET file */
    savePaintNetPalette(true, lastSavedPalette);
}

void MainWindow::savePaintNetPalette(bool fileDialog, QString fileName) {
    if (fileDialog) {
        const QString filter = tr("Palettes") + " (" + PALETTE_FILE_FILTERS.join(" ") + ")";\
        fileName = QFileDialog::getSaveFileName(this, tr("Save Palette"), lastSavedPalette, filter);
        if (fileName.isEmpty()) { // user cancelled
            notification->showText(tr("WARNING: palette not saved"), 3000);
            return;
        }
    }
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream<<";paint.net Palette File\n";
        stream<<";Palette Name: Ditherista palette ("<<customPalette->size<<")\n";
        stream<<";Colors: "<<customPalette->size<<"\n";
        for (size_t i = 0; i < customPalette->size; i++) {
            ByteColor* color = BytePalette_get(customPalette, i);
            stream<<QString("%1").arg(color->a, 2, 16, QLatin1Char( '0' )).toUpper();
            stream<<QString("%1").arg(color->r, 2, 16, QLatin1Char( '0' ));
            stream<<QString("%1").arg(color->g, 2, 16, QLatin1Char( '0' ));
            stream<<QString("%1").arg(color->b, 2, 16, QLatin1Char( '0' ))<<"\n";
        }
        file.close();
    } else {
        notification->showText("<font color=#ec6a5e>" + tr("ERROR") + "</font>\n" +
                               tr("palette file write error"), 3000);
        return;
    }
    notification->showText(tr("palette saved"), 2000);
    lastSavedPalette = fileName;
}

BytePalette* MainWindow::loadPaintNetPalette(QString fileName, int* errorCode) {
    /* loads a Paint.NET compatible palette file, such as found on lospec.com.
     * Transparency is currently not supported.
     * Color entries can be 32 or 24 bit (i.e. including alpha or without)
     * Comments starting with ; are supported */
    *errorCode = OK_PALETTE_LOAD;
    QFile file = QFile(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"ERROR: loading Paint .NET palette: "<<file.errorString();
        *errorCode = ERROR_PALETTE_LOAD_IO;
        return nullptr;
    }
    QTextStream in(&file);
    QList<ByteColor> palette;
    while(!in.atEnd()) {
        QString line = in.readLine().trimmed().toLower();
        int lineLength = line.length() == 8 ? 4 : (line.length() == 6 ? 3 : -1);
        if(!line.startsWith(";") && lineLength != -1) {
            int channel[4];
            for (int i = 0; i < lineLength; i++) {
                bool ok;
                channel[lineLength - 1 - i] = QString("0x%1").arg(line.sliced(2 * i, 2)).toInt(&ok, 16);
                if (!ok) {
                    qDebug()<<"ERROR: parsing palette: "<<line;
                    *errorCode = ERROR_PALETTE_LOAD_PARSE;
                    return nullptr;
                }
            }
            if (palette.size() > USER_FILE_PALETTE_MAX_COLORS) {
                qDebug()<<"WARNING: palette has more than 256 colors (palette truncated)";
                *errorCode = ERROR_PALETTE_LOAD_MAX_COLORS;
                break;
            }
            ByteColor color;
            color.b = channel[0];
            color.g = channel[1];
            color.r = channel[2];
            color.a = 255; // channel[3]; // TODO - we are currently not supporting transparent palette colors
            palette.append(color);
        }
    }
    file.close();
    if (palette.size() < 2) {
        qDebug()<<"ERROR: palette has less than 2 colors";
        *errorCode = ERROR_PALETTE_LOAD_LOW_COLORS;
        return nullptr; // not enough colors
    }
    BytePalette* pal = BytePalette_new(palette.size());
    for(qsizetype i = 0; i < palette.size(); i ++) {
        BytePalette_set(pal, (size_t)i, &palette.at(i));
    }
    return pal;
}

/************************************
 * COLOR-PALETTE LOADING FROM FILE  *
 ************************************/

void MainWindow::paletteBrowseButtonClickedSlot() {
    /* user pressed "Browse" button to load a custom palette */
    QString fileIoLocation = lastLoadedPalette;
    const QString filter = tr("Palettes") + " (" + PALETTE_FILTERS.join(" ") + ")";
    QString fileName = QFileDialog::getOpenFileName((QWidget *) parent(),
                                                    tr("Open Palette File"), fileIoLocation,
                                                    filter);
    if (fileName.isEmpty() || fileName.isNull() || !QFile::exists(fileName)) {
        return; // cancelled or file does not exist
    }
    if (loadPaintNetPaletteWrapper(fileName)) {
        whileBlocking(ui->palettePathEdit)->setText(fileName);
    }
}

void MainWindow::palettePathEditEditingFinishedSlot() {
    /* user entered a path in the palette path text-edit field */
    QString newPalette = ui->palettePathEdit->text();
    if (lastLoadedPalette != newPalette) {
        if (!loadPaintNetPaletteWrapper(newPalette)) {
            whileBlocking(ui->palettePathEdit)->setText(lastLoadedPalette); // restore old filename
        }
    }
}

bool MainWindow::loadPaintNetPaletteWrapper(QString fileName) {
    /* wrapper, with error handling for loading a Paint.NET palette */
    int errorCode;
    if (loadFromFilePalette != nullptr) {
        BytePalette_free(loadFromFilePalette);
    }
    loadFromFilePalette = loadPaintNetPalette(fileName, &errorCode);
    if (errorCode == OK_PALETTE_LOAD || errorCode == ERROR_PALETTE_LOAD_MAX_COLORS) {
        generateCachedPalette(true, false, true);
        if (errorCode == ERROR_PALETTE_LOAD_MAX_COLORS) {
            notification->showText(tr("WARNING\nmore than 256 colors in palette"), 3000);
        }
        lastLoadedPalette = fileName;
    } else {
        switch(errorCode) {
            case ERROR_PALETTE_LOAD_IO:
                notification->showText("<font color=#ec6a5e>" + tr("ERROR") + "</font>\n" +
                                       tr("palette file read error"), 3000);
                break;
            case ERROR_PALETTE_LOAD_PARSE:
                notification->showText("<font color=#ec6a5e>" + tr("ERROR") + "</font>\n" +
                                       tr("palette file parsing error"), 3000);
                break;
            case ERROR_PALETTE_LOAD_LOW_COLORS:
                notification->showText("<font color=#ec6a5e>" + tr("ERROR") + "</font>\n" +
                                       tr("palette has too few colors"), 3000);
                break;
            default: // we should never get this error
                notification->showText("<font color=#ec6a5e>" + tr("ERROR") + "</font>\n" +
                                       tr("palette loading error"), 3000);
                break;
        }
        return false;
    }
    return true;
}

/*************************************************
 * COLOR-COMPARISON MODE                         *
 *************************************************/

void MainWindow::colorComparisonComboChangedSlot(int index) {
    /* user changed the combobox for setting the color comparison mode */
    switch(index) {
        case 3: colorComparisonMode = LUMINANCE; break;
        case 5: colorComparisonMode = SRGB; break;
        case 7: colorComparisonMode = LINEAR; break;
        case 4: colorComparisonMode = HSV; break;
        case 0: colorComparisonMode = LAB76; break;
        case 1: colorComparisonMode = LAB94; break;
        case 2: colorComparisonMode = LAB2000; break;
        case 6: colorComparisonMode = SRGB_CCIR; break;
        case 8: colorComparisonMode = LINEAR_CCIR; break;
        case 9: colorComparisonMode = TETRAPAL; break;
    }
    bool expand = colorComparisonMode == LAB76 || colorComparisonMode == LAB94 || colorComparisonMode == LAB2000;
    expandColorComparisonArea(expand);

    // TODO instead of disabling (=temporary solution), we should rework the .ui file instead and not display the spinners at all
    if (expand) {
        ui->spinBoxHue->setEnabled(colorComparisonMode != LAB76);
        ui->spinBoxChroma->setEnabled(colorComparisonMode != LAB76);
        ui->spinBoxValue->setEnabled(colorComparisonMode != LAB76);
        ui->resetHueWeightButton->setEnabled(colorComparisonMode != LAB76);
        ui->resetChromaWeightButton->setEnabled(colorComparisonMode != LAB76);
        ui->resetValueWeightButton->setEnabled(colorComparisonMode != LAB76);
    }

    generateCachedPalette(true, false, true);
}

/*******************************************************
 * COLOR-PALETTE SELECTION / CACHED-PALETTE GENERATION *
 *******************************************************/

void MainWindow::paletteSourceComboChangedSlot(int index) {
    /* user changed the combobox for setting the palette source (built-in, from file, custom) */
    ui->paletteSourceWidget->setCurrentIndex(index);
    generateCachedPalette(true, false, true);
}

void MainWindow::updatePaletteFromImage() {
    /* creates a reduced palette from the currently loaded image */
    ColorImage* image = imageHashColor.getSourceImage();
    if (image == nullptr) {
        qDebug() << "WARNING: image from imageHash at generateCachedPalette() is null!";
    } else {
        bool ok = false;
        int colors = QVariant(ui->paletteColorsEdit->text()).toInt(&ok);
        if (ok) {
            CachedPalette_free(cachedPalette);
            cachedPalette = CachedPalette_new();
            fthread = QtConcurrent::run(CachedPalette_from_image, cachedPalette, image, colors,
                                        colorReductionMode,
                                        ui->palGenUniqueColorsCheck->isChecked(),
                                        ui->palGenBWCheck->isChecked(),
                                        ui->palGenRGBCheck->isChecked(),
                                        ui->palGenCMYCheck->isChecked());
            runDitherThread();
            CachedPalette_update_cache(cachedPalette, colorComparisonMode, &srcIlluminant);
            CachedPalette_set_shift(cachedPalette, DEFAULT_BIT_SHIFT.r, DEFAULT_BIT_SHIFT.g, DEFAULT_BIT_SHIFT.b);
        }
    }
}

void MainWindow::updateCachedPalette(BytePalette* pal) {
    /* loads a palette from file or .qrc resource */
    CachedPalette_free(cachedPalette);
    cachedPalette = CachedPalette_new();
    BytePalette_free(reducedPalette);
    reducedPalette = BytePalette_copy(pal);
    CachedPalette_from_BytePalette(cachedPalette, reducedPalette);
    CachedPalette_update_cache(cachedPalette, colorComparisonMode, &srcIlluminant);
    CachedPalette_set_shift(cachedPalette, DEFAULT_BIT_SHIFT.r, DEFAULT_BIT_SHIFT.g, DEFAULT_BIT_SHIFT.b);
}

void MainWindow::generateCachedPalette(bool dither, bool resetLab, bool updateSwatches) {
    /* generates a cache for speeding up palette lookups during dithering */
    setMouseBusy(true);
    // use file based palette
    if (ui->paletteSourceWidget->currentIndex() == PALETTE_FROM_FILE) {
        if (loadFromFilePalette != nullptr) {
            updateCachedPalette(loadFromFilePalette);
        } else { // no file based palette loaded -> do nothing
            setMouseBusy(false);
            return;
        }
    // use built-in palette from resource file
    } else if (ui->paletteSourceWidget->currentIndex() == PALETTE_BUILT_IN) {
        int errorCode = 0;
        BytePalette* pal = loadPaintNetPalette(lastBuiltInPalette, &errorCode);
        if (pal != nullptr) {
            updateCachedPalette(pal);
            BytePalette_free(pal);
        } else {
            qDebug()<<"WARNING: failed to load palette from ressource:"<<lastBuiltInPalette<<errorCode;
        }
    // quantify colors form image
    } else if (ui->paletteSourceWidget->currentIndex() == PALETTE_CUSTOM) {
        updateCachedPalette(customPalette);
    } else {
        updatePaletteFromImage();
    }
    updatePaletteColorSwatches(cachedPalette->target_palette);
    refreshUiColorDitherStatus(resetLab, updateSwatches);
    if (!resetLab) {
        cachedPalette->lab_weights.v = ui->spinBoxValue->value();
        cachedPalette->lab_weights.c = ui->spinBoxChroma->value();
        cachedPalette->lab_weights.h = ui->spinBoxHue->value();
    }
    setMouseBusy(false);
    if (dither) {
        reDither(true);
    }
}

/*************************************************
 * PRE-DEFINED (BUILT-IN) COLOR-PALETTE          *
 *************************************************/

void MainWindow::predefinedPaletteComboChangedSlot(int index) {
    /* user chooses a different pre-defined palette */
    QString palName = ui->predefinedPaletteCombo->itemData(index).toString();
    lastBuiltInPalette = QString(":/resources/palettes/%1.pal").arg(palName);
    generateCachedPalette(true, false, true);
}

/*************************************************
 * COLOR-PALETTE REDUCTION / QUANTIFICATION      *
 *************************************************/

void MainWindow::paletteIncludeExtremeColorsSlot(int) {
    /* triggered for custom palettes when user checks one of the checkboxes, e.g. to include RGB or CMY colors */
    generateCachedPalette(true, false, true);
}

void MainWindow::colorReductionComboChangedSlot(int index) {
    /* triggered for custom palettes when user changes the color reduction method */
    // TODO: a Qt data item should be used here, rather than relying on the combo box's index -> more robust
    colorReductionMode = (enum QuantizationMethod)(index + 1); // ensure index matches with QuantizationMethod enum
    generateCachedPalette(true, false, true);
}

/*************************************************
 * COLOR-PALETTE COLOR SWATCHES                  *
 *************************************************/

void MainWindow::paletteColorsEditEditingFinishedSlot() {
    /* triggered for custom palattes when user edits the input field for the number of colors */
    generateCachedPalette(true, false, true);
}

void MainWindow::updatePaletteColorSwatches(BytePalette* palette) {
    /* populates the color list with entries and color swatches, based on the given palette */
    ui->colorListWidget->clear();
    for (size_t i = 0; i < palette->size; i++) {
        ui->colorListWidget->addColorEntry((int)i, BytePalette_get(palette, i));
    }
    ui->colorListWidget->resizeColumnToContents(0);
    ui->colorListWidget->resizeColumnToContents(1);
}

void MainWindow::paletteColorChangedSlot(int index, QColor color) {
    /* user changed a color in the reduced palette, either by pasting a color or via the color dialog */
    BytePalette* newPalette = BytePalette_copy(cachedPalette->target_palette);
    ByteColor bc = { (uint8_t)color.red(), (uint8_t)color.green(), (uint8_t)color.blue(), (uint8_t)color.alpha() };
    BytePalette_set(newPalette, index, &bc);
    if (ui->paletteSourceCombo->count() == 3) { // no custom palette exists -> create a new one
        BytePalette_free(customPalette);
        customPalette = newPalette;
        ui->paletteSourceCombo->addItem(QString());
        ui->paletteSourceCombo->setItemText(3, tr("custom"));
        ui->paletteSourceCombo->setCurrentIndex(PALETTE_CUSTOM);
        whileBlocking(ui->paletteSourceWidget)->setCurrentIndex(3);
    } else if (ui->paletteSourceWidget->currentIndex() != 3) { // create new custom palette (user is not in custom palette mode)
        bool updatePalette = true;
        if (customPalette != nullptr) { // another custom palette already exists...
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("Custom Palette Exists"),
                                          tr("Changing the current palette will create a new custom palette.\n"
                                             "Do you want to save the existing palette?"),
                                          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (reply == QMessageBox::Yes) {
                savePaintNetPalette(true, lastSavedPalette);
            } else if (reply == QMessageBox::Cancel) {
                updatePalette = false; // cancel color edit
            }
        }
        if (updatePalette) {
            BytePalette_free(customPalette);
            customPalette = newPalette;
            ui->paletteSourceCombo->setCurrentIndex(PALETTE_CUSTOM);
            whileBlocking(ui->paletteSourceWidget)->setCurrentIndex(3);
        }
    } else { // user changes another color of the custom palette
        BytePalette_free(customPalette);
        customPalette = newPalette;
        generateCachedPalette(true, false, true);
    }
}