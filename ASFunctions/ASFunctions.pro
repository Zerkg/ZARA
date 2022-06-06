QT       += core network
TEMPLATE = lib

DEFINES += ASFUNCTIONS_LIBARARY
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17

SOURCES += \
    ASAfterrideBonus.cpp \
    ASCharacter.cpp \
    ASFunctions.cpp \
    ASLeaderboard.cpp \
    ASPhpQuery.cpp \
    TableModel.cpp

HEADERS += \
    ASAfterrideBonus.h \
    ASCharacter.h \
    ASFunctions.h \
    ASFunctions_global.h \
    ASLeaderboard.h \
    ASPhpQuery.h \
    ASTypes.h \
    TableModel.h

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

