HOME = ../../..
include ($$HOME/common.pri)
TEMPLATE = app
TARGET = pdCalc
DEPENDPATH += .
INCLUDEPATH += $$HOME/src
DESTDIR = $$HOME/bin

QT += widgets

win32:CONFIG += console

SOURCES += main.cpp

unix:LIBS += -L$$HOME/lib -lpdCalcGui -lpdCalcCli -lpdCalcBackend -lpdCalcUtilities
win32:LIBS += -L$$HOME/bin -lpdCalcGui1 -lpdCalcCli1 -lpdCalcBackend1 -lpdCalcUtilities1
