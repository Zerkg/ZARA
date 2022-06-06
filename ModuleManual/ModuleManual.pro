QT      += gui widgets
CONFIG  += c++17
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = lib

SOURCES += \
#    main.cpp \
    ModuleManual.cpp

HEADERS += \
    ModuleManual.h

FORMS += \
    ModuleManual.ui

RESOURCES += \
    res.qrc

#====================:: Configuration settings for ::====================
#====================:: release and debug version  ::====================

SUFFIX = ""

CONFIG(debug, debug|release) {
    message("debug mode")
    SUFFIX = "_d"
}else {
    message("release mode")
}

TARGET = ModuleManual$${SUFFIX}
DLLDESTDIR = ../.dll

QMAKE_CXXFLAGS_RELEASE += -O3
