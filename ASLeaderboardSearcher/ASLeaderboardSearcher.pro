QT += gui widgets

TEMPLATE = lib                      #Comment this line and uncomment #26 to build the project as independed componet
DEFINES += \
    ASLEADERBOARDSEARCHER_LIBRARY \
    QT_DEPRECATED_WARNINGS

CONFIG += c++17

INCLUDEPATH += \
    ..\ASFunctions

RESOURCES += \
    res.qrc


# ==================================
# =========:: Main files ::=========
# ==================================
HEADERS += \
    ASLeaderboardSearcher.h        \
    ASLeaderboardSearcher_global.h \
    DataTypes.h \

SOURCES += \
    ASLeaderboardSearcher.cpp      \
#    main.cpp

FORMS += \
    ASLeaderboardSearcher.ui


# ===================================
# ======:: Common components ::======
# ===================================

PREFIX = Components/
HEADERS += \
    $${PREFIX}SearchResultsBox.h \
    $${PREFIX}DisplayFilter.h    \
    $${PREFIX}FilterEditor.h     \
    $${PREFIX}FilterSettings.h   \

SOURCES += \
    $${PREFIX}SearchResultsBox.cpp \
    $${PREFIX}FilterSettings.cpp   \
    $${PREFIX}DisplayFilter.cpp    \
    $${PREFIX}FilterEditor.cpp     \


FORMS += \
    $${PREFIX}SearchResultsBox.ui \
    $${PREFIX}FilterSettings.ui   \
    $${PREFIX}FilterEditor.ui     \


# ===================================
# ==========:: Searchers ::==========
# ===================================

PREFIX = Searchers/
HEADERS += \
    $${PREFIX}StandardSearcher.h \

SOURCES += \
    $${PREFIX}StandardSearcher.cpp \

FORMS += \
    $${PREFIX}StandardSearcher.ui \


# ===================================
# ===========:: History ::===========
# ===================================

PREFIX = History/
HEADERS += \
    $${PREFIX}HistoryModel.h     \
    $${PREFIX}HistoryView.h      \
    $${PREFIX}HistoryCompleter.h \

SOURCES += \
    $${PREFIX}HistoryModel.cpp     \
    $${PREFIX}HistoryView.cpp      \
    $${PREFIX}HistoryCompleter.cpp \

FORMS += \
    $${PREFIX}HistoryView.ui


#====================:: Configuration settings for ::====================
#====================:: release and debug version  ::====================

SUFFIX = ""

CONFIG(debug, debug|release) {
    message("Debug mode")
    SUFFIX = "_d"
}else {
    message("Release mode")
}

TARGET = ASLeaderboardSearcher$${SUFFIX}
LIBS += -L../.dll/ -lASFunctions$${SUFFIX}

DLLDESTDIR = ../.dll

QMAKE_CXXFLAGS_RELEASE += -O3

