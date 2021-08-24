QT += gui widgets

TEMPLATE = lib
DEFINES += ASLEADERBOARDSEARCHER_LIBRARY

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
    "..\ASFunctions"

LIBS += \
#    "C:\Qt\Projects\build-ASLeaderboardSelector-Desktop_Qt_5_15_2_MinGW_32_bit-Release\release\ASFunctions.dll"
    "C:\Qt\Projects\build-ASLeaderboardSearcher-Desktop_Qt_5_15_2_MinGW_32_bit-Debug\debug\ASFunctions.dll"


SOURCES += \
    ASLeaderboardSearcher.cpp \
    SearchHistoryWidget.cpp \
#    main.cpp

HEADERS += \
    ASLeaderboardSearcher.h \
    ASLeaderboardSearcher_global.h \
    SearchHistoryWidget.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    ASLeaderboardSearcher.ui \
    SearchHistoryWidget.ui

RESOURCES += \
    res.qrc

QMAKE_CXXFLAGS_RELEASE += -O3
