QT += gui widgets

TEMPLATE = lib                   #Comment this line and uncomment #22 to build project as independent component
DEFINES += \
    ASLEADERBOARDANALYZER_LIBRARY \
    QT_DEPRECATED_WARNINGS

CONFIG += c++17

INCLUDEPATH +=                 \
    "..\ASFunctions"           \
    "..\ASLeaderboardSearcher" \
    "..\ASScoreSelector"       \
    "..\ASLeaderboardCharts"   \
    "..\ModuleManual"          \

SOURCES += \
    ASLeaderboardAnalyzer.cpp   \
    PlayerStatsController.cpp \
    PlayerStatsWidget.cpp \
    StatsAlias.cpp \
    StatsConverter.cpp \
    main.cpp

HEADERS += \
    ASLeaderboardAnalyzer_global.h \
    ASLeaderboardAnalyzer.h        \
    PlayerStatsController.h        \
    PlayerStatsWidget.h            \
    StatsAlias.h                   \
    StatsConverter.h

FORMS += \
    ASLeaderboardAnalyzer.ui   \
    PlayerStatsController.ui \
    PlayerStatsWidget.ui \
    PowerupsWidget.ui

RESOURCES += res.qrc

win32: RC_ICONS = $$PWD/Images/LeaderboardAnalyzer.ico

#====================:: Configuration settings for ::====================
#====================:: release and debug version  ::====================

SUFFIX = ""

CONFIG(debug, debug|release) {
    message("debug mode")
    SUFFIX = "_d"
}else {
    message("release mode")
}

TARGET = ASLeaderboardAnalyzer$${SUFFIX}
LIBS += \
    -L../.dll/ -lASFunctions$${SUFFIX}           \
    -L../.dll/ -lASScoreSelector$${SUFFIX}       \
    -L../.dll/ -lASLeaderboardSearcher$${SUFFIX} \
    -L../.dll/ -lASLeaderboardCharts$${SUFFIX}   \
    -L../.dll/ -lModuleManual$${SUFFIX}

DLLDESTDIR = ../.dll

QMAKE_CXXFLAGS_RELEASE += -O3
