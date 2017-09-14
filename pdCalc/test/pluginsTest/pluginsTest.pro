HOME = ../..
include ($$HOME/common.pri)
TEMPLATE = lib
TARGET = pluginsTest
DEPENDPATH += $$HOME/src/shared $$HOME/src/backend
INCLUDEPATH += . $$HOME $$HOME/src
unix:DESTDIR = $$HOME/lib
win32:DESTDIR = $$HOME/bin

DEFINES += PLUGIN_TEST_DIR=\\\"$$PWD\\\"
unix:DEFINES += PLUGIN_TEST_FILE=\\\"plugins.unix.pdp\\\"
win32:DEFINES += PLUGIN_TEST_FILE=\\\"plugins.win.pdp\\\"

QT += testlib

# Input
HEADERS += HyperbolicLnPluginTest.h
SOURCES += HyperbolicLnPluginTest.cpp
unix:LIBS += -L$$HOME/lib -lpdCalcUtilities -lpdCalcBackend
win32:LIBS += -L$$HOME/bin -lpdCalcUtilities1 -lpdCalcBackend1
