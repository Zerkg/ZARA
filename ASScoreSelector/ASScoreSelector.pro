QT += gui widgets

TEMPLATE = lib                   #Comment this line and uncomment #18 to build project as independent component
DEFINES += \
    ASSCORESELECTOR_LIBRARY \
    QT_DEPRECATED_WARNINGS

CONFIG += c++17

INCLUDEPATH += \
    "..\ASFunctions"

HEADERS += \
     ASScoreSelector.h \
     ASScoreSelector_global.h \

SOURCES += \
#    main.cpp \
    ASScoreSelector.cpp

FORMS += \
    ASScoreSelector.ui

RESOURCES += \
    ../ASFunctions/res.qrc

#====================:: Configuration settings for ::====================
#====================:: release and debug version  ::====================

SUFFIX = ""

CONFIG(debug, debug|release) {
    message("debug mode")
    SUFFIX = "_d"
}else {
    message("release mode")
}

TARGET = ASScoreSelector$${SUFFIX}

LIBS += -L../.dll/ -lASFunctions$${SUFFIX} \

DLLDESTDIR = ../.dll

QMAKE_CXXFLAGS_RELEASE += -O3

