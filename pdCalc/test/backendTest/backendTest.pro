HOME = ../..
include ($$HOME/common.pri)
TEMPLATE = lib
TARGET = pdCalcBackendTest
DEPENDPATH += $$HOME/src/shared
INCLUDEPATH += . $$HOME $$HOME/src
unix:DESTDIR = $$HOME/lib
win32:DESTDIR = $$HOME/bin
DEFINES += BACKEND_TEST_DIR=\\\"$$PWD\\\"
unix:DEFINES += PLUGIN_TEST_FILE=\\\"plugins.unix.pdp\\\"
win32:DEFINES += PLUGIN_TEST_FILE=\\\"plugins.win.pdp\\\"


# Input
HEADERS += StackTest.h \
    CommandManagerTest.h \
    CommandRepositoryTest.h \
    CoreCommandsTest.h \
    CommandDispatcherTest.h \
    StoredProcedureTest.h \
    PluginLoaderTest.h
SOURCES += StackTest.cpp \
    CommandManagerTest.cpp \
    CommandRepositoryTest.cpp \
    CoreCommandsTest.cpp \
    CommandDispatcherTest.cpp \
    StoredProcedureTest.cpp \
    PluginLoaderTest.cpp

unix:LIBS += -L$$HOME/lib -lpdCalcUtilities -lpdCalcBackend
win32:LIBS += -L$$HOME/bin -lpdCalcUtilities1 -lpdCalcBackend1

QT -= gui core
QT += testlib
