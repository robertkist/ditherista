#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "libdither.h"
#include "about/aboutwindow.h"
#include "help/helpwindow.h"
#include "enums.h"
#include "imagehash/imagehashmono.h"
#include "imagehash/imagehashcolor.h"
#include "ui_elements/notificationlabel.h"
#include "filemanager.h"
#include "ui_elements/mouseeventfilter.h"
#include "updatecheck.h"
#include "batch/batchditherdialog.h"
#include <QTreeWidgetItem>
#include <QFuture>
#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>

// main tabWidget indexes
#define TAB_INDEX_MONO 0
#define TAB_INDEX_COLOR 1
#define TAB_INDEX_PALETTE 2

// palette source combobox index
#define PALETTE_BUILT_IN 0
#define PALETTE_FROM_FILE 1
#define PALETTE_REDUCED 2
#define PALETTE_CUSTOM 3

// paint.NET palette loading error codes
#define OK_PALETTE_LOAD 0
#define ERROR_PALETTE_LOAD_IO 1
#define ERROR_PALETTE_LOAD_PARSE 2
#define ERROR_PALETTE_LOAD_LOW_COLORS 3
#define ERROR_PALETTE_LOAD_MAX_COLORS 4

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// dither type mapping to stacked-widget (ui->ditherSettings) index pages (dithertype : index_page)
const QHash<int, int> ditherPage = {
        {DOT, 1}, {LIP, 1},  {PAT, 1}, {ORD_C, 1}, // generic dither settings page = 1
        {DBS, 2}, {ERR, 3}, {ORD, 4}, {RIM, 5}, {THR, 6}, {VAR, 7}, {ALL, 8},
        {ORD_IGR, 9}, {ORD_VA2, 10}, {ORD_VA4, 10}, {GRD, 11},
        {ERR_C, 12}, {ORD_VA2_C, 13}, {ORD_VA4_C, 13}, {ORD_IGR_C, 14}};

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    /* methods */
    explicit MainWindow(QWidget* parent = nullptr);
    void resizeEvent(QResizeEvent* event) override;
//    void keyPressEvent(QKeyEvent* event) override;
    ~MainWindow() override;
private:
    /* attributes */
    // GUI windows and program components
    Ui::MainWindow* ui{};                   // Qt UI for main window
    AboutWindow* aboutWindow;               // about window
    HelpWindow* helpWindow;                 // help window
    UpdateCheck* updateCheck;               // update check dialog
    FileManager fileManager;                // file open/save dialog
    BatchDitherDialog* batchDitherDialog;   // batch dither dialog
    NotificationLabel* notification;        // notification area inside the viewport
    MouseEventFilter eventFilter;           // filters out undesired mouse events
    QNetworkAccessManager* networkAccessManager{};  // manager http update checks
    // program state
    bool isDithering = false;               // true while dithering in progress
    bool firstLoad = true;                  // true if no image has been loaded yet
    int lastTabIndex = TAB_INDEX_MONO;      // last active index (mono or color tab; palette tab is ignored)
    QString currentDirectory;               // directory from where the current input image has been loaded from
    QString lastLoadedPalette;              // file name of the last palette loaded from external file
    QString lastBuiltInPalette;             // resource name of the last loaded built-in palette
    TreeWidget* activeTreeWidget = nullptr; // either colorTreeWidget or monoTreeWidget (based on active tab)
    // dithering
    QFuture<void> fthread;                 // thread for dithering
    ImageHashMono imageHashMono;           // caching for already dithered images Mono
    ImageHashColor imageHashColor;         // caching for already dithered images Color
    int current_dither_number = 0;         // number of current ditherer (lower numbers=mono, higher numbers=color)
    DitherType current_dither_type;        // current dither type:     e.g. error diffusion
    SubDitherType current_sub_dither_type; // current sub-dither type: e.g. floyd steinberg error diffusion
    // color palette related
    enum QuantizationMethod colorReductionMode;
    enum ColorComparisonMode colorComparisonMode;
    BytePalette* reducedPalette = nullptr;    // reduced palette sRGB
    CachedPalette* cachedPalette = nullptr;   // reduced palette in a particular color format (e.g. LAB94, HSV, etc.)
    BytePalette* loadFromFilePalette = nullptr; // caches the last palette that has been loaded externally from file
    FloatColor srcIlluminant;                 // currently chosen illuminant for LAB color


    QString lastSavedPalette;

    BytePalette* customPalette = nullptr;

    /* methods */
    void setDitherImageMono();      // sets ditherimage in graphicsview and applies user chosen light/dark colors
    // UI related and helpers
    void setMouseBusy(bool isBusy);
    void enableGui(bool enable);
    QPixmap* loadSvg(const QString& fileName); // load SVG as QPixmap from (resource) file
    void setResetIcon(QPushButton* button, const QIcon* icon);
    void uiSetup();                       // ui initialization (main)
    void setDithererDefaults();           // ui initialization
    void populateDithererSelection();     // ui initialization
    void addPredefinedPalettes();         // ui initialization
    void populateColorComparisonCombo();  // ui initialization
    void connectSignals();                // ui initialization
    void setApplicationDefaults();        // initialization - set default application values
    void expandColorComparisonArea(bool expand);
    // dithering (general)
    void runDitherThread();
    void reDither(bool force);
    // dithering mono
    void ALL_dither(uint8_t* out_buf);
    void GRD_dither(uint8_t* out_buf);
    void DBS_dither(uint8_t* out_buf);
    void THR_dither(uint8_t* out_buf);
    void DOT_dither(uint8_t* out_buf, SubDitherType n);
    void ERR_dither(uint8_t* out_buf, SubDitherType n);
    void LIP_dither(uint8_t* out_buf, SubDitherType n);
    void ORD_dither(uint8_t* out_buf, SubDitherType n);
    void PAT_dither(uint8_t* out_buf, SubDitherType n);
    void RIM_dither(uint8_t* out_buf, SubDitherType n);
    void VAR_dither(uint8_t* out_buf, SubDitherType n);
    // dithering color
    void ERR_C_dither(int* out_buf, const SubDitherType n);
    void ORD_C_dither(int* out_buf, const SubDitherType n);
    // batch dithering
    BatchDitherResult ditherSingleImage(const QString& inFileName, const QString& outFileName);
    // palette and color handling
    BytePalette* loadPaintNetPalette(QString fileName, int* errorCode);
    void savePaintNetPalette(bool fileDialog, QString fileName);
    bool loadPalette(QString fileName);
    void updatePaletteColorSwatches(BytePalette* palette);
    void generateCachedPalette(bool dither, bool resetLab, bool updateSwatches);
    void resetLabHCVspinBoxes();
    void updatePaletteFromImage();
    void updateCachedPalette(BytePalette* pal);
    // file I/O
    void saveFile(const QString &fileName);
    void loadImage(const QImage* image);
    void fileSaveSlotImpl(bool saveAs);
    // image adjustments
    void adjustImageMono();
    void adjustImageColor();


    void refreshUiColorDitherStatus(bool resetLab, bool updateSwatches);
    void labHCVChanged();
    bool loadPaintNetPaletteWrapper(QString fileName);

private slots:
    // image adjustment mono
    void resetContrastButtonMonoClickedSlot();
    void resetBrightnessButtonMonoClickedSlot();
    void resetGammaButtonMonoClickedSlot();
    void contrastSliderMonoValueChangedSlot(int value);
    void brightnessSliderMonoValueChangedSlot(int value);
    void gammaSliderMonoValueChangedSlot(int value);
    void contrastEditMonoEditingFinishedSlot(double value = 0.0);
    void brightnessEditMonoEditingFinishedSlot(double value = 0.0);
    void gammaEditMonoEditingFinishedSlot(double value = 0.0);
    void showOriginalMonoButtonClickedSlot(int checked);
    // image adjustment color
    void resetContrastButtonColorClickedSlot();
    void resetBrightnessButtonColorClickedSlot();
    void resetGammaButtonColorClickedSlot();
    void resetSaturationButtonColorClickedSlot();
    void contrastSliderColorValueChangedSlot(int);
    void brightnessSliderColorValueChangedSlot(int);
    void gammaSliderColorValueChangedSlot(int);
    void saturationSliderColorValueChangedSlot(int);
    void contrastEditColorEditingFinishedSlot(double value = 0.0);
    void brightnessEditColorEditingFinishedSlot(double value = 0.0);
    void gammaEditColorEditingFinishedSlot(double value = 0.0);
    void saturationEditColorEditingFinishedSlot(double value = 0.0);
    void showOriginalColorButtonClickedSlot(int checked);
    // UI helpers
    void paletteSourceWidgetIndexChangedSlot(int index);
    void imageSettingsStackedWidgetIndexChangedSlot(int index);
    // file I/O
    void loadImageFromFileSlot(const QString &fileName);
    void fileSaveSlot();
    void fileSaveAsSlot();
    void fileOpenSlot();
    // dither settings mono
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
    // dither settings color
    void serpentineColorToggledSlot(bool serpentine);
    void ORD_stepColorValueChangedSlot(int step);
    void ORD_IGR_aColorValueChangedSlot(double a);
    void ORD_IGR_bColorValueChangedSlot(double b);
    void ORD_IGR_cColorValueChangedSlot(double c);
    // dithering
    void forceReDitherSlot() { reDither(true); };
    // palette mono
    void monoColorOneChangedSlot(QColor color);
    void monoColorTwoChangedSlot(QColor color);
    void resetMonoColorsClickedSlot();
    // palette color
    void paletteSourceComboChangedSlot(int index);
    void predefinedPaletteComboChangedSlot(int index);
    void colorComparisonComboChangedSlot(int index);
    void paletteBrowseButtonClickedSlot();
    void palettePathEditEditingFinishedSlot();
    void paletteColorsEditEditingFinishedSlot();
    void colorReductionComboChangedSlot(int index);
    void paletteColorChangedSlot(int index, QColor color); // user changed a color in the palette
    void paletteIncludeExtremeColorsSlot(int); // include light/dark, CMY, RGB color
    // palette LAB colors
    void spinBoxValueChangedSlot(double value);
    void spinBoxHueChangedSlot(double value);
    void spinBoxChromaChangedSlot(double value);
    void srcIlluminantComboChangedSlot(int index);
    // misc UI
    void treeWidgetItemChangedSlot(QTreeWidgetItem* item);
    void tabWidgetChangedSlot(int index);
    void copySlot() const;
    void pasteSlot();
    void editMenuAboutToShowSlot();
    void keyEventSlot(QKeyEvent* event);
    // batch dithering
//    void batchDitherRequestedSlot(); // TODO temporary disabled
    // online update check
    void networkRequestFinishedSlot(QNetworkReply* reply) const;
    void updateCheckSlot();

    void savePaletteButtonClickedSlot();
    void resetHueWeightButtonClickedSlot();
    void resetChromaWeightButtonClickedSlot();
    void resetValueWeightButtonClickedSlot();

    void zoomLevelChangedSlot(int zoomLevel);
    void zoomLevelComboCurrentIndexChangedSlot(int index);
    void zoomLevelEditingFinishedSlot();
};

#endif // MAINWINDOW_H
