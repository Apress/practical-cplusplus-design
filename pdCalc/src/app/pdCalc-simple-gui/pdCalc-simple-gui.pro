HOME = ../../..
include ($$HOME/common.pri)
TEMPLATE = app
TARGET = pdCalc-simple-gui
DEPENDPATH += .
INCLUDEPATH += $$HOME/src
DESTDIR = $$HOME/bin

QT += widgets

SOURCES += main.cpp

unix:LIBS += -L$$HOME/lib -lpdCalcGui -lpdCalcBackend -lpdCalcUtilities
win32:LIBS += -L$$HOME/bin -lpdCalcGui1 -lpdCalcBackend1 -lpdCalcUtilities1
