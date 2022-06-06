QT += gui widgets

TEMPLATE = lib
DEFINES += \
    ASLEADERBOARDSELECTOR_LIBRARY \
    QT_DEPRECATED_WARNINGS

CONFIG += c++17

INCLUDEPATH += \
    "..\ASFunctions"

SOURCES += \
#    main.cpp \
    ASLeaderboardSelector.cpp

HEADERS += \
    ASLeaderboardSelector.h \
    ASLeaderboardSelector_global.h

FORMS += \
    ASLeaderboardSelector.ui

#====================:: Configuration settings for ::====================
#====================:: release and debug version  ::====================

SUFFIX = ".dll"

CONFIG(debug, debug|release) {
    message("debug mode")
    TARGET = ASLeaderboardSelector_d
    SUFFIX = "_d.dll"
}else {
    message("release mode")
    TARGET = ASLeaderboardSelector
}

LIBS += \
    -L../.dll/ -ASFunctions$${SUFFIX} \
    ../.dll/ASFunctions$${SUFFIX}

DLLDESTDIR = ../.dll

QMAKE_CXXFLAGS_RELEASE += -O3
