QT += \
    core \
    gui \
    concurrent \
    svg \
    network \
    widgets

INCLUDEPATH += ../libdither/src/libdither
CONFIG += c++20 lrelease embed_translations
QMAKE_CXXFLAGS_RELEASE += -Wall -fno-common  -Os

!win32-msvc* {
    LIBS += -L../libdither/dist -ldither
    QMAKE_CXXFLAGS_RELEASE += -Wextra -Wstrict-overflow -Wformat=2 -Wundef -Wpedantic -Werror \
                              -Wno-c++20-attribute-extensions -Wno-c++20-extensions -O3 -pedantic -std=gnu++20
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
}

win32-msvc {
    LIBS += -L../libdither/dist/Release -llibdither
}

win32 {
    RC_ICONS = resources/appicon.ico
}

SOURCES += \
    modernredux/style.cpp \
    help/helpwindow.cpp \
    about/aboutwindow.cpp \
    viewport/graphicsview.cpp \
    viewport/graphicspixmapitem.cpp \
    batch/batchditherdialog.cpp \
    batch/batchditherprogress.cpp \
    batch/ditherresults.cpp \
    batch/fileexistsdialog.cpp \
    ui_elements/mouseeventfilter.cpp \
    ui_elements/stackedwidget.cpp \
    ui_elements/notificationlabel.cpp \
    ui_elements/elidelabel.cpp \
    ui_elements/messagebox.cpp \
    ui_elements/doublepushbutton.cpp \
    ui_elements/doublelabel.cpp \
    ui_elements/editcombobox.cpp \
    ui_elements/svg.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindow_ui.cpp \
    mainwindow_dither_mono.cpp \
    mainwindow_dither_color.cpp \
    mainwindow_update_check.cpp \
    mainwindow_img_adjust.cpp \
    mainwindow_palette.cpp \
    mainwindow_helpers.cpp \
    treewidget.cpp \
    imagehash/imagehash.cpp \
    imagehash/imagehashcolor.cpp \
    imagehash/imagehashmono.cpp \
    filemanager.cpp \
    treewidgetdelegate.cpp \
    updatecheck.cpp \
    colortree.cpp \
    colortreedelegate.cpp \
    ui_elements/colorcopymenu.cpp \
    ui_elements/sectioncombobox.cpp

HEADERS += \
    modernredux/style.h \
    help/helpwindow.h \
    about/aboutwindow.h \
    viewport/graphicsview.h \
    viewport/graphicspixmapitem.h \
    batch/batchditherdialog.h \
    batch/batchditherprogress.h \
    batch/ditherresults.h \
    batch/fileexistsdialog.h \
    ui_elements/stackedwidget.h \
    ui_elements/notificationlabel.h \
    ui_elements/elidelabel.h \
    ui_elements/mouseeventfilter.h \
    ui_elements/messagebox.h \
    ui_elements/doublepushbutton.h \
    ui_elements/doublelabel.h \
    ui_elements/editcombobox.h \
    ui_elements/svg.h \
    mainwindow.h \
    treewidget.h \
    imagehash/imagehash.h \
    imagehash/imagehashcolor.h \
    imagehash/imagehashmono.h \
    filemanager.h \
    treewidgetdelegate.h \
    updatecheck.h \
    enums.h \
    colortree.h \
    colortreedelegate.h \
    ui_elements/colorcopymenu.h \
    ui_elements/signalblocker.h \
    ui_elements/sectioncombobox.h

FORMS += \
    mainwindow.ui \
    about/aboutwindow.ui \
    help/helpwindow.ui \
    batch/batchditherdialog.ui \
    batch/batchditherprogress.ui \
    batch/ditherresults.ui \
    batch/fileexistsdialog.ui \
    updatecheck.ui

RESOURCES += \
    modernredux/modernredux.qrc \
    resources.qrc

TRANSLATIONS += \
    application_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
