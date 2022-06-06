QT += gui widgets

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ZARAMainWindow.cpp \
    main.cpp \

HEADERS += \
    ZARAMainWindow.h

INCLUDEPATH += \
    "..\ASLeaderboardAnalyzer"

RESOURCES += \
    res.qrc

FORMS += \
    ZARAMainWindow.ui

win32: RC_ICONS = $$PWD/Icon/icon.ico

#====================:: Configuration settings for ::====================
#====================:: release and debug version  ::====================

SUFFIX = ""

CONFIG(debug, debug|release) {
    message("debug mode")
    SUFFIX = "_d"
}else {
    message("release mode")
}

LIBS += \
    -L../.dll/ -lASLeaderboardAnalyzer$${SUFFIX}

QMAKE_CXXFLAGS_RELEASE += -O3
