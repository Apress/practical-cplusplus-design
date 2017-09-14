HOME = ../..
include ($$HOME/common.pri)
TEMPLATE = lib
TARGET = pdCalcGuiTest
INCLUDEPATH += . $$HOME
unix:DESTDIR = $$HOME/lib
win32:DESTDIR = $$HOME/bin

QT += widgets testlib

# Input
HEADERS += DisplayTest.h
SOURCES += DisplayTest.cpp

unix:LIBS += -L$$HOME/lib -lpdCalcBackend -lpdCalcUtilities -lpdCalcGui
win32:LIBS += -L$$HOME/bin -lpdCalcBackend1 -lpdCalcUtilities1 -lpdCalcGui1
