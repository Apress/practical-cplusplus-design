HOME = ../..
include ($$HOME/common.pri)
TEMPLATE = lib
TARGET = pdCalcCliTest
INCLUDEPATH += . $$HOME
unix:DESTDIR = $$HOME/lib
win32:DESTDIR = $$HOME/bin
DEFINES += CLI_TEST_DIR=\\\"$$PWD\\\"

# Input
HEADERS += CliTest.h
SOURCES += CliTest.cpp

unix:LIBS += -L$$HOME/lib -lpdCalcCli -lpdCalcBackend -lpdCalcUtilities
win32:LIBS += -L$$HOME/bin -lpdCalcCli1 -lpdCalcBackend1 -lpdCalcUtilities1
QT -= gui core
QT += testlib
