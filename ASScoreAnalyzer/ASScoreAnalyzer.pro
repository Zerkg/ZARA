QT += gui widgets

TEMPLATE = lib
DEFINES += ASSCOREANALYZER_LIBRARY
CONFIG += c++17


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    "..\ASFunctions" \
    "..\ASLeaderboardSearcher" \
    "..\ASLeaderboardSelector"

LIBS += \
    "C:\Qt\Projects\build-ASLeaderboardAnalyzer-Desktop_Qt_5_15_2_MinGW_32_bit-Debug\debug\ASFunctions.dll" \
    "C:\Qt\Projects\build-ASLeaderboardAnalyzer-Desktop_Qt_5_15_2_MinGW_32_bit-Debug\debug\ASLeaderboardSelector.dll" \
    "C:\Qt\Projects\build-ASLeaderboardAnalyzer-Desktop_Qt_5_15_2_MinGW_32_bit-Debug\debug\ASLeaderboardSearcher.dll"
    #"C:\Qt\Projects\build-ASScoreAnalyzer-Desktop_Qt_5_15_2_MinGW_32_bit-Release\release\ASFunctions.dll" \
    #"C:\Qt\Projects\build-ASScoreAnalyzer-Desktop_Qt_5_15_2_MinGW_32_bit-Release\release\ASLeaderboardSelector.dll" \
    #"C:\Qt\Projects\build-ASScoreAnalyzer-Desktop_Qt_5_15_2_MinGW_32_bit-Release\release\ASLeaderboardSearcher.dll"

SOURCES += \
#    main.cpp \
    ASScoreAnalyzer.cpp \
    AnalyzerCore.cpp

HEADERS += \
    ASScoreAnalyzer.h \
    ASScoreAnalyzer_global.h \
    AnalyzerCore.h

FORMS += \
    ASScoreAnalyzer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: RC_ICONS = $$PWD/ScoreAnalyzer.ico

QMAKE_CXXFLAGS_RELEASE += -O3
