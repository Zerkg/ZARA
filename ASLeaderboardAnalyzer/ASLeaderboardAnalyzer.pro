QT += gui widgets

TEMPLATE = lib
DEFINES += \
    ASLEADERBOARDANALYZER_LIBRARY \
    QT_DEPRECATED_WARNINGS

CONFIG += c++17

INCLUDEPATH += \
    "..\ASFunctions" \
    "..\ASLeaderboardSearcher" \
    "..\ASLeaderboardSelector"

SOURCES += \
    ASLeaderboardAnalyzer.cpp \
    ASPlayerStatsController.cpp \
    ASPlayerStatsModel.cpp \
    ASPlayerStatsProxy.cpp \
    ASPlayerStatsView.cpp \
    CompareModeSettings.cpp \
#    main.cpp

HEADERS += \
    ASLeaderboardAnalyzer_global.h \
    ASLeaderboardAnalyzer.h \
    ASPlayerStatsController.h \
    ASPlayerStatsModel.h \
    ASPlayerStatsProxy.h \
    ASPlayerStatsView.h \
    CompareModeSettings.h

FORMS += \
    ASLeaderboardAnalyzer.ui \
    ASPlayerStatsView.ui \
    CompareModeSettings.ui \
    GuiController.ui

RESOURCES += res.qrc

win32: RC_ICONS = $$PWD/Images/LeaderboardAnalyzer.ico

#====================:: Configuration settings for ::====================
#====================:: release and debug version  ::====================

SUFFIX = ".dll"

CONFIG(debug, debug|release) {
    message("debug mode")
    TARGET = ASLeaderboardAnalyzer_d
    SUFFIX = "_d.dll"
}else {
    message("release mode")
    TARGET = ASLeaderboardAnalyzer
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
