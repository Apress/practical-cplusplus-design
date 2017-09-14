HOME = ../../..
include ($$HOME/common.pri)
TEMPLATE = app
TARGET = pdCalc-simple-cli
DEPENDPATH += .
INCLUDEPATH += $$HOME/src
DESTDIR = $$HOME/bin

win32:CONFIG += console

SOURCES += main.cpp

unix:LIBS += -L$$HOME/lib -lpdCalcCli -lpdCalcBackend -lpdCalcUtilities
win32:LIBS += -L$$HOME/bin -lpdCalcCli1 -lpdCalcBackend1 -lpdCalcUtilities1
