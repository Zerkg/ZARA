QT += gui widgets

TEMPLATE = lib                      #Comment this line and uncomment #16 line to build the project as independed componet
DEFINES += \
    ASLEADERBOARDSEARCHER_LIBRARY \
    QT_DEPRECATED_WARNINGS

CONFIG += c++17

INCLUDEPATH += \
    "..\ASFunctions"

SOURCES += \
    ASLeaderboardSearcher.cpp \
    SearchHistoryWidget.cpp \
#    main.cpp

HEADERS += \
    ASLeaderboardSearcher.h \
    ASLeaderboardSearcher_global.h \
    SearchHistoryWidget.h

FORMS += \
    ASLeaderboardSearcher.ui \
    SearchHistoryWidget.ui

RESOURCES += res.qrc

#====================:: Configuration settings for ::====================
#====================:: release and debug version  ::====================

SUFFIX = ".dll"

CONFIG(debug, debug|release) {
    message("debug mode")
    TARGET = ASLeaderboardSearcher_d
    SUFFIX = "_d.dll"
}else {
    message("release mode")
    TARGET = ASLeaderboardSearcher
}

LIBS += \
    -L../.dll/ -ASFunctions$${SUFFIX} \
    ../.dll/ASFunctions$${SUFFIX}

DLLDESTDIR = ../.dll

QMAKE_CXXFLAGS_RELEASE += -O3

