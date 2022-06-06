QT += gui widgets

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ZARAMainWindow.cpp \
    main.cpp \

HEADERS += \
    ZARAMainWindow.h

INCLUDEPATH += \
    "..\ASLeaderboardAnalyzer" \
    "..\ASScoreAnalyzer"

RESOURCES += \
    res.qrc

FORMS += \
    ZARAMainWindow.ui

win32: RC_ICONS = $$PWD/Icon/icon.ico

#====================:: Configuration settings for ::====================
#====================:: release and debug version  ::====================

SUFFIX = ".dll"

CONFIG(debug, debug|release) {
    message("debug mode")
    SUFFIX = "_d.dll"
}else {
    message("release mode")
    DLLDESTDIR = ../.dll
}

LIBS += \
    -L../.dll/ -ASLeaderboardAnalyzer$${SUFFIX} \
    ../.dll/ASLeaderboardAnalyzer$${SUFFIX} \
    -L../.dll/ -ASScoreAnalyzer$${SUFFIX} \
    ../.dll/ASScoreAnalyzer$${SUFFIX}

QMAKE_CXXFLAGS_RELEASE += -O3
