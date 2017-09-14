HOME = ../../..
include ($$HOME/common.pri)
TEMPLATE = lib
TARGET = hyperbolicLnPlugin
DEPENDPATH += .
INCLUDEPATH += . $$HOME/src
unix:DESTDIR = $$HOME/lib
win32:DESTDIR = $$HOME/bin
QT += widgets

# Input
HEADERS += HyperbolicLnPlugin.h
SOURCES += HyperbolicLnPlugin.cpp

unix:QMAKE_PRE_LINK+=$(COPY_FILE) $$PWD/../plugins.pdp.unix $$HOME/bin/plugins.pdp
win32:QMAKE_PRE_LINK+=$(COPY_FILE) $$shell_path($$PWD/../plugins.pdp.win) $$shell_path($$HOME/bin/plugins.pdp)

win32:LIBS += -L$$HOME/bin -lpdCalcUtilities1 -lpdCalcBackend1
