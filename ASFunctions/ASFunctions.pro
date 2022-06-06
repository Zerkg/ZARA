QT      += core network
TEMPLATE = lib                   #Comment this line and uncomment #22 to build project as independent component

DEFINES += \
    ASFUNCTIONS_LIBARARY   \
    QT_DEPRECATED_WARNINGS

CONFIG += c++17

SOURCES += \
    ASAfterrideBonus.cpp \
    ASFunctions.cpp      \
    ASPhpQuery.cpp

HEADERS += \
    ASAfterrideBonus.h   \
    ASFunctions.h        \
    ASFunctions_global.h \
    ASPhpQuery.h         \
    ASTypes.h \
    PlayerStats.h

RESOURCES += res.qrc

#====================:: Configuration settings for ::====================
#====================:: release and debug version  ::====================

CONFIG(debug, debug|release) {
    message("debug mode")
    TARGET = ASFunctions_d
}else {
    message("release mode")
    TARGET = ASFunctions
}

DLLDESTDIR = ..\.dll

QMAKE_CXXFLAGS_RELEASE += -O3

