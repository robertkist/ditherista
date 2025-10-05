#include "mainwindow.h"
#include "ui_elements/signalblocker.h"
#include "consts.h"

/* This file contains:
 * - method for adjusting mono and color source images
 */

/*********************************
 * MONO IMAGE ADJUSTMENTS        *
 *********************************/

void MainWindow::resetContrastButtonMonoClickedSlot() {
    /* user resets the source image's contrast via reset button */
    whileBlocking(ui->contrastSliderMono)->setValue(DEFAULT_MONO_CONTRAST_ADJUST * 100.0);
    whileBlocking(ui->contrastEditMono)->setValue(DEFAULT_MONO_CONTRAST_ADJUST);
    contrastEditMonoEditingFinishedSlot();
};

void MainWindow::resetBrightnessButtonMonoClickedSlot() {
    /* user resets the source image's brightness via reset button */
    whileBlocking(ui->brightnessSliderMono)->setValue(DEFAULT_MONO_BRIGHTNESS_ADJUST * 100.0);
    whileBlocking(ui->brightnessEditMono)->setValue(DEFAULT_MONO_BRIGHTNESS_ADJUST);
    brightnessEditMonoEditingFinishedSlot();
};

void MainWindow::resetGammaButtonMonoClickedSlot() {
    /* user resets the source image's gamma via reset button */
    whileBlocking(ui->gammaSliderMono)->setValue(DEFAULT_MONO_GAMMA_ADJUST * 100.0);
    whileBlocking(ui->gammaEditMono)->setValue(DEFAULT_MONO_GAMMA_ADJUST);
    gammaEditMonoEditingFinishedSlot();
};

void MainWindow::contrastSliderMonoValueChangedSlot(int value) {
    /* user adjusts the contrast slider */
    whileBlocking(ui->contrastEditMono)->setValue(value / 100.0);
    contrastEditMonoEditingFinishedSlot();
}

void MainWindow::brightnessSliderMonoValueChangedSlot(int value) {
    /* user adjusts the contrast slider */
    whileBlocking(ui->brightnessEditMono)->setValue(value / 100.0);
    brightnessEditMonoEditingFinishedSlot();
}

void MainWindow::gammaSliderMonoValueChangedSlot(int value) {
    /* user adjusts the contrast slider */
    whileBlocking(ui->gammaEditMono)->setValue(value / 100.0);
    gammaEditMonoEditingFinishedSlot();
}

void MainWindow::brightnessEditMonoEditingFinishedSlot(double) {
    /* triggered when user edited the numerical brightness input */
    int iValue = (int)(ui->brightnessEditMono->value() * 100);
    if (imageHashMono.brightness != iValue) {
        imageHashMono.brightness = iValue;
        whileBlocking(ui->brightnessSliderMono)->setValue(iValue);
        adjustImageMono();
    }
}

void MainWindow::gammaEditMonoEditingFinishedSlot(double) {
    /* triggered when user edited the numerical gamma input */
    int iValue = (int)(ui->gammaEditMono->value() * 100);
    if (imageHashMono.gamma != iValue) {
        imageHashMono.gamma = iValue;
        whileBlocking(ui->gammaSliderMono)->setValue(iValue);
        adjustImageMono();
    }
}

void MainWindow::contrastEditMonoEditingFinishedSlot(double) {
    /* triggered when user edited the numerical contrast input */
    int iValue = (int)(ui->contrastEditMono->value() * 100);
    if (imageHashMono.contrast != iValue) {
        imageHashMono.contrast = iValue;
        whileBlocking(ui->contrastSliderMono)->setValue(iValue);
        adjustImageMono();
    }
}

void MainWindow::adjustImageMono() {
    /* applies the current image adjustments to the source image - Mono */
    imageHashMono.adjustSource();
    imageHashMono.clearAllDitheredImages();
    ui->treeWidgetMono->clearAllDitherFlags();
    ui->graphicsView->setSourceImageMono(imageHashMono.getSourceQImage());
    reDither(true);
}

void MainWindow::showOriginalMonoButtonClickedSlot(const int checked) {
    /* toggles between showing the source image and the dithered image */
    if (checked == Qt::Checked) {
        ui->graphicsView->showSourceMono(true);
    } else {
        ui->graphicsView->showSourceMono(false);
        reDither(false);
    }
}

/*********************************
 * COLOR IMAGE ADJUSTMENTS       *
 *********************************/

// reset color image adjustment button slots

void MainWindow::resetContrastButtonColorClickedSlot() {
    /* user resets the source image's contrast via reset button */
    whileBlocking(ui->contrastSliderColor)->setValue(DEFAULT_COLOR_CONTRAST_ADJUST * 100.0);
    whileBlocking(ui->contrastEditColor)->setValue(DEFAULT_COLOR_CONTRAST_ADJUST);
    contrastEditColorEditingFinishedSlot();
}

void MainWindow::resetBrightnessButtonColorClickedSlot() {
    /* user resets the source image's brightness via reset button */
    whileBlocking(ui->brightnessSliderColor)->setValue(DEFAULT_COLOR_BRIGHTNESS_ADJUST * 100.0);
    whileBlocking(ui->brightnessEditColor)->setValue(DEFAULT_COLOR_BRIGHTNESS_ADJUST);
    brightnessEditColorEditingFinishedSlot();
}

void MainWindow::resetGammaButtonColorClickedSlot() {
    /* user resets the source image's gamma via reset button */
    whileBlocking(ui->gammaSliderColor)->setValue(DEFAULT_COLOR_GAMMA_ADJUST * 100.0);
    whileBlocking(ui->gammaEditColor)->setValue(DEFAULT_COLOR_GAMMA_ADJUST);
    gammaEditColorEditingFinishedSlot();
}

void MainWindow::resetSaturationButtonColorClickedSlot() {
    /* resets the source image's brightness */
    whileBlocking(ui->saturationSliderColor)->setValue(DEFAULT_COLOR_SATURATION_ADJUST * 100.0);
    whileBlocking(ui->saturationEditColor)->setValue(DEFAULT_COLOR_SATURATION_ADJUST);
    saturationEditColorEditingFinishedSlot();
}

// color image adjustment slider slots

void MainWindow::contrastSliderColorValueChangedSlot(int value) {
    /* user adjusts the contrast slider */
    whileBlocking(ui->contrastEditColor)->setValue(value / 100.0);
    contrastEditColorEditingFinishedSlot();
}

void MainWindow::brightnessSliderColorValueChangedSlot(int value) {
    /* user adjusts the brightness slider */
    whileBlocking(ui->brightnessEditColor)->setValue(value / 100.0);
    brightnessEditColorEditingFinishedSlot();
}

void MainWindow::gammaSliderColorValueChangedSlot(int value) {
    /* user adjusts the gamma slider */
    whileBlocking(ui->gammaEditColor)->setValue(value / 100.0);
    gammaEditColorEditingFinishedSlot();
}

void MainWindow::saturationSliderColorValueChangedSlot(int value) {
    /* user adjusts the saturation slider */
    whileBlocking(ui->saturationEditColor)->setValue(value / 100.0);
    saturationEditColorEditingFinishedSlot();
}

// color image adjustment spin box slots

void MainWindow::brightnessEditColorEditingFinishedSlot(double) {
    /* triggered when user edited the numerical brightness input */
    int iValue = (int)(ui->brightnessEditColor->value() * 100);
    if (imageHashColor.brightness != iValue) {
        imageHashColor.brightness = iValue;
        whileBlocking(ui->brightnessSliderColor)->setValue(iValue);
        adjustImageColor();
    }
}

void MainWindow::gammaEditColorEditingFinishedSlot(double) {
    /* triggered when user edited the numerical gamma input */
    int iValue = (int)(ui->gammaEditColor->value() * 100);
    if (imageHashColor.gamma != iValue) {
        imageHashColor.gamma = iValue;
        whileBlocking(ui->gammaSliderColor)->setValue(iValue);
        adjustImageColor();
    }
}

void MainWindow::contrastEditColorEditingFinishedSlot(double) {
    /* triggered when user edited the numerical contrast input */
    int iValue = (int)(ui->contrastEditColor->value() * 100);
    if (imageHashColor.contrast != iValue) {
        imageHashColor.contrast = iValue;
        whileBlocking(ui->contrastSliderColor)->setValue(iValue);
        adjustImageColor();
    }
}

void MainWindow::saturationEditColorEditingFinishedSlot(double) {
    /* triggered when user edited the numerical saturation input */
    int iValue = (int)(ui->saturationEditColor->value() * 100);
    if (imageHashColor.saturation != iValue) {
        imageHashColor.saturation = iValue;
        whileBlocking(ui->saturationSliderColor)->setValue(iValue);
        adjustImageColor();
    }
}

// apply & preview color image adjustments

void MainWindow::adjustImageColor() {
    /* applies the current image adjustments to the source image - Color */
    imageHashColor.adjustSource();
    generateCachedPalette(true, false, true);
    refreshUiColorDitherStatus(false, false);
    ui->graphicsView->setSourceImageColor(imageHashColor.getSourceQImage());
    reDither(true);
}

void MainWindow::showOriginalColorButtonClickedSlot(const int checked) {
    /* toggles between showing the source image and the dithered image */
    if (checked == Qt::Checked) {
        ui->graphicsView->showSourceColor(true);
    } else {
        ui->graphicsView->showSourceColor(false);
        reDither(false);
    }
}
