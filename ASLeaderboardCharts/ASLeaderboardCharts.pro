QT       += core gui widgets

DEFINES += \
    ASLEADERBOARDCHARTS_LIBRARY \
    QT_DEPRECATED_WARNINGS      \
    QCUSTOMPLOT_USE_LIBRARY

CONFIG += c++17
TEMPLATE = lib

INCLUDEPATH += \
    "..\ASFunctions"

HEADERS += \
    ASLeaderboardCharts_global.h \
    StatsChart.h                 \
    TracerToolTip.h

SOURCES += \
    StatsChart.cpp    \
    TracerToolTip.cpp \
#    main.cpp

FORMS += \
    StatsChart.ui    \
    TracerToolTip.ui

RESOURCES += \
    res.qrc

#====================:: Configuration settings for ::====================
#====================:: release and debug version  ::====================

SUFFIX = ""

CONFIG(debug, debug|release) {
    message("debug mode")
    SUFFIX = "_d"
    QCPLIB = qcustomplotd2
}else {
    QCPLIB = qcustomplot2
    message("release mode")
}
TARGET = ASLeaderboardCharts$${SUFFIX}

LIBS += \
    -L../.dll/ -lASFunctions$${SUFFIX} \
    -L../.dll/ -l$$QCPLIB

DLLDESTDIR = ../.dll

QMAKE_CXXFLAGS_RELEASE += -O3
