QT += \
    core \
    gui \
    concurrent \
    svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += ../libdither/src/libdither
LIBS += -L../libdither/dist -ldither

CONFIG += c++11
QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
RC_ICONS = resources/appicon.ico

SOURCES += \
    modernredux/style.cpp \
    main.cpp \
    mainwindow.cpp \
    graphicsview.cpp \
    stackedwidget.cpp \
    aboutwindow.cpp \
    graphicspixmapitem.cpp \
    treewidget.cpp \
    imagehash.cpp \
    notificationlabel.cpp \
    elidelabel.cpp \
    filemanager.cpp \
    treewidgetdelegate.cpp \
    mouseeventfilter.cpp \
    helpwindow.cpp

HEADERS += \
    modernredux/style.h \
    mainwindow.h \
    graphicsview.h \
    stackedwidget.h \
    aboutwindow.h \
    graphicspixmapitem.h \
    treewidget.h \
    imagehash.h \
    notificationlabel.h \
    elidelabel.h \
    filemanager.h \
    treewidgetdelegate.h \
    mouseeventfilter.h \
    helpwindow.h

FORMS += \
    mainwindow.ui \
    aboutwindow.ui \
    helpwindow.ui

RESOURCES += \
    modernredux/modernredux.qrc \
    resources.qrc

TRANSLATIONS += \
    application_en_US.ts

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
