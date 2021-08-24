QT += gui widgets

TEMPLATE = lib
DEFINES += ASLEADERBOARDSELECTOR_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    "..\ASFunctions"

LIBS += \
    "C:\Qt\Projects\build-ASLeaderboardSelector-Desktop_Qt_5_15_2_MinGW_32_bit-Debug\debug\ASFunctions.dll"
#    "C:\Qt\Projects\build-ASLeaderboardSelector-Desktop_Qt_5_15_2_MinGW_32_bit-Release\release\ASFunctions.dll"

SOURCES += \
#    main.cpp \
    ASLeaderboardSelector.cpp

HEADERS += \
    ASLeaderboardSelector.h \
    ASLeaderboardSelector_global.h

FORMS += \
    ASLeaderboardSelector.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QMAKE_CXXFLAGS_RELEASE += -O3
