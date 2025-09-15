#pragma once
#ifndef CONSTS_H
#define CONSTS_H

#include <QSize>
#include <QStringList>
#include <QColor>
#include "enums.h"
#include "libdither.h"

/* constants governing program behavior and overall GUI sizes/spacing.
 * constants regarding specific UI elements' looks are within their respective source files.
 */

// MainWindow settings
#define MIN_WINDOW_HEIGHT 624
#define MIN_WINDOW_WIDTH 770
#define MIN_SPLITTER_WIDTH 294

// palette source (built-in, from file, custom) (ui->paletteSourceWidget)
#define PALETTE_SOURCE_CUSTOM_INDEX 2     // custom palette from image
#define PALETTE_SOURCE_HEIGHT_DEFAULT 26  // for build-in, from file
#define PALETTE_SOURCE_HEIGHT_CUSTOM 138  // for custom color quantization

// height of color comparison settings area
#define COLOR_COMPARISON_HEIGHT 52      // for all other comparison modes
#define COLOR_COMPARISON_HEIGHT_MAX 156 // for LAB comparison

// 'Input Image Settings' group box at right bottom (ui->imageSettingsStackedWidget)
#define IMAGE_SETTINGS_STACKED_WIDGET_MONO_INDEX 0     // mono image adjustments
#define IMAGE_SETTINGS_STACKED_WIDGET_COLOR_INDEX 1    // color image adjustments
#define IMAGE_SETTINGS_STACKED_WIDGET_MONO_HEIGHT 130  // for mono image adjustments
#define IMAGE_SETTINGS_STACKED_WIDGET_COLOR_HEIGHT 156 // for color image adjustments

// Min/max colors for reduced palette
#define PALETTE_MIN_COLORS 2
#define PALETTE_MAX_COLORS 256

// Max colors for user loaded Paint.NET palettes to avoid loading overly large palettes
#define USER_FILE_PALETTE_MAX_COLORS 256

// default illuminant - assumes source image is lit with this illuminant when using LAB color comparison
inline constexpr const FloatColor& DEFAULT_ILLUMINANT = D65_XYZ;

// CachedPalette settings
inline constexpr ByteColor DEFAULT_BIT_SHIFT = { .r=1, .g=1, .b=1, .a=0 };

// default mono dithering colors
inline constexpr QColor DEFAULT_DARK_MONO_COLOR = QColor(0, 0, 0, 255);
inline constexpr QColor DEFAULT_LIGHT_MONO_COLOR = QColor(255, 255, 255, 255);

// reset buttons
inline const QSize resetButtonIconSize = QSize(16, 16);
inline const QSize resetButtonSize = QSize(20, 20);

// update check
inline const QString UPDATE_URL("https://gist.githubusercontent.com/robertkist/806563a8b5be80764ec926d9df459b2c/raw/dversion.txt?cachebust=");
inline const QString DOWNLOAD_URL("https://github.com/robertkist/ditherista/releases/tag/");
inline constexpr int HTTP_TIMEOUT = 3000;  // how long we should wait to hear back from the update server (in ms)

// dithering related
inline constexpr int THREAD_SLEEP_DELAY_MS = 80;  // delay for sleeping while dithering is in progress
inline constexpr int IMAGE_MAX_SIZE = 4096 * 4096;  // maximum supported input image size
inline constexpr int TIMEOUT_FOREVER = 1000 * 60 * 60 * 24 * 7;  // forever is a whole week!
inline constexpr int DBS_PROGRESS_TRIGGER_SIZE = 256 * 256;  // image size when to pop up a warning that DBS dither takes long

// supported file types
inline static QStringList PALETTE_FILE_FILTERS = {"*.txt", "*.pal"};
inline static QStringList FILE_FILTERS = {"*.jpg", "*.jpeg", "*.png", "*.bmp"};
inline static QStringList PALETTE_FILTERS = {"*.pal", "*.txt"};

// viewport/GraphicsView zoom
inline constexpr int MIN_ZOOM = 10;
inline constexpr int MAX_ZOOM = 500;
inline constexpr int ZOOM_STEP_KEYBOARD = 10;
inline constexpr int ZOOM_STEP_WHEEL = 10;

// default color dithering palette
inline static QString DEFAULT_PALETTE = "p_mac16";
inline constexpr int DEFAULT_COLOR_COMPARISON_MODE = (int)SRGB;
inline constexpr int DEFAULT_COLOR_REDUCTION_MODE = (int)MEDIAN_CUT;

#endif // CONSTS_H