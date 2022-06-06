QT += gui widgets

TEMPLATE = lib
DEFINES += \
    ASSCOREANALYZER_LIBRARY \
    QT_DEPRECATED_WARNINGS

CONFIG += c++17

INCLUDEPATH += \
    "..\ASFunctions" \
    "..\ASLeaderboardSearcher" \
    "..\ASLeaderboardSelector"

SOURCES += \
#    main.cpp \
    ASScoreAnalyzer.cpp \
#    AnalyzerCore.cpp

HEADERS += \
    ASScoreAnalyzer.h \
    ASScoreAnalyzer_global.h \
#    AnalyzerCore.h

FORMS += \
    ASScoreAnalyzer.ui

#====================:: Configuration settings for ::====================
#====================:: release and debug version  ::====================

SUFFIX = ".dll"

CONFIG(debug, debug|release) {
    message("debug mode")
    TARGET = ASScoreAnalyzer_d
    SUFFIX = "_d.dll"
}else {
    message("release mode")
    TARGET = ASScoreAnalyzer
}

LIBS += \
    -L../.dll/ -ASFunctions$${SUFFIX} \
    ../.dll/ASFunctions$${SUFFIX} \
    -L../.dll/ -ASLeaderboardSelector$${SUFFIX} \
    ../.dll/ASLeaderboardSelector$${SUFFIX} \
    -L../.dll/ -ASLeaderboardSearcher$${SUFFIX} \
    ../.dll/ASLeaderboardSearcher$${SUFFIX}

DLLDESTDIR = ../.dll

QMAKE_CXXFLAGS_RELEASE += -O3
