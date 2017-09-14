HOME = ../../..
include ($$HOME/common.pri)
TEMPLATE = lib
TARGET = pdCalcCli
DEPENDPATH += .
INCLUDEPATH += . $$HOME/src
unix:DESTDIR = $$HOME/lib
win32:DESTDIR = $$HOME/bin


# Input
HEADERS += Cli.h
SOURCES += Cli.cpp

win32:LIBS += -L$$HOME/bin -lpdCalcUtilities1 -lpdCalcBackend1
