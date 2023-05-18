#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "libdither.h"
#include "aboutwindow.h"
#include "helpwindow.h"
#include "graphicspixmapitem.h"
#include "enums.h"
#include "imagehash.h"
#include "notificationlabel.h"
#include "filemanager.h"
#include "mouseeventfilter.h"
#include <QFuture>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// dither type mapping to stacked-widget (ui->ditherSettings) index pages
const QHash<int, int> ditherPage = {
        {GRD, 11},
        {ALL, 10}, {DBS, 2}, {DOT, 1}, {ERR, 3},
        {LIP, 1}, {ORD, 4}, {PAT, 1}, {RIM, 5},
        {THR, 6}, {VAR, 7}, {ORD_IGR, 8}, {ORD_VA2, 9}, {ORD_VA4, 9}};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow* ui;
    AboutWindow* aboutWindow;
    HelpWindow* helpWindow;
    QFuture<void> fthread;          // thread for dithering
    ImageHash imageHash;            // caching for already dithered images
    FileManager fileManager;
    bool isDithering = false;       // true while dithering in progress
    bool firstLoad = true;          // true if no image has been loaded yet
    NotificationLabel *notification;
    MouseEventFilter eventFilter;
    int current_dither_number = 0;
    enum DitherType current_dither_type;
    enum SubDitherType current_sub_dither_type;

    void enableUi(bool enable);
    void clearImageCache();
    void runDitherThread();
    void reDither(bool force);
    void saveFile(QString fileName);
    void ALL_dither(uint8_t* out_buf);
    void GRD_dither(uint8_t* out_buf);
    void DBS_dither(uint8_t* out_buf);
    void THR_dither(uint8_t* out_buf);
    void DOT_dither(uint8_t* out_buf, enum SubDitherType n);
    void ERR_dither(uint8_t* out_buf, enum SubDitherType n);
    void LIP_dither(uint8_t* out_buf, enum SubDitherType n);
    void ORD_dither(uint8_t* out_buf, enum SubDitherType n);
    void PAT_dither(uint8_t* out_buf, enum SubDitherType n);
    void RIM_dither(uint8_t* out_buf, enum SubDitherType n);
    void VAR_dither(uint8_t* out_buf, enum SubDitherType n);
    void loadImage(const QImage* image);
    QPixmap* loadSvg(QString fileName);

private slots:
    void loadImageFromFileSlot(QString fileName);
    void copySlot();
    void pasteSlot();
    void fileSaveSlot();
    void fileSaveAsSlot();
    void fileOpenSlot();
    void editMenuAboutToShowSlot();
    void adjustImageSlot(int);
    void showOriginalButtonPressedSlot(int checked);
    void treeWidgetItemChangedSlot(QTreeWidgetItem* item);
    void forceReDitherSlot() { reDither(true); };
    void resetContrastButtonPressedSlot() { ui->contrastSlider->setValue(0.0); };
    void resetBrightnessButtonPressedSlot() { ui->brightnessSlider->setValue(0.0); };
    void resetGammaButtonPressedSlot() { ui->gammaSlider->setValue(0.0); };

    void DBS_setFormulaSlot(int formula);
    void RIM_modRiemersmaToggledSlot(bool value);
    void THR_thresholdValueChangedSlot(double threshold);
    void THR_toggleAutoThresholdSlot(bool autoThreshold);
    void ORD_stepValueChangedSlot(int step);
    void ORD_IGR_aValueChangedSlot(double a);
    void ORD_IGR_bValueChangedSlot(double b);
    void ORD_IGR_cValueChangedSlot(double c);

    void ALL_randomizeToggleSlot(bool value);
    void GRD_altAlgorithmToggleSlot(bool value);
    void GRD_widthValueChangedSlot(int value) ;
    void GRD_heightValueChangedSlot(int value);
    void GRD_minPixelsValueChangedSlot(int value);

    void serpentineToggledSlot(bool serpentine);
    void jitterValueChangedSlot(double jitter);
};

#endif // MAINWINDOW_H
