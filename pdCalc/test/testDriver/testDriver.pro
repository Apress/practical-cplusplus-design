HOME = ../..
include ($$HOME/common.pri)
QMAKE_CXXFLAGS += -Wno-deprecated-declarations
TEMPLATE = app
TARGET = testPdCalc
INCLUDEPATH += $$HOME $$HOME/src
DESTDIR = $$HOME/bin

QT += testlib

# Input
SOURCES += main.cpp

unix:LIBS += -L$$HOME/lib -lpdCalcUtilities -lpdCalcUtilitiesTest \
        -lpdCalcBackend -lpdCalcBackendTest \
        -lpdCalcCliTest -lpdCalcCli \
        -lpdCalcGuiTest -lpdCalcGui \
        -lpluginsTest

win32:LIBS += -L$$HOME/bin -lpdCalcUtilities1 -lpdCalcUtilitiesTest1 \
        -lpdCalcBackend1 -lpdCalcBackendTest1 \
        -lpdCalcCliTest1 -lpdCalcCli1 \
        -lpdCalcGuiTest1 -lpdCalcGui1 \
        -lpluginsTest1
