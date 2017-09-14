HOME = ../..
include ($$HOME/common.pri)
TEMPLATE = lib
TARGET = pdCalcUtilities
DEPENDPATH += .
INCLUDEPATH += . $$HOME/src
unix:DESTDIR = $$HOME/lib
win32:DESTDIR = $$HOME/bin

DEFINES += BUILDING_UTILITIES

# Input
HEADERS += Exception.h \
           Observer.h \
           Publisher.h \
           Tokenizer.h \
           UserInterface.h

SOURCES += Observer.cpp \
           Publisher.cpp \
           Tokenizer.cpp \
           UserInterface.cpp

OTHER_FILES += \
    Publisher.o \
    Observer.o \
    Makefile
