HOME = ../../..
include ($$HOME/common.pri)
TEMPLATE = lib
TARGET = pdCalcGui
DEPENDPATH += .
INCLUDEPATH += . $$HOME/src
unix:DESTDIR = $$HOME/lib
win32:DESTDIR = $$HOME/bin

QT += widgets

# Input
HEADERS += MainWindow.h \
    Display.h \
    LookAndFeel.h \
    InputWidget.h \
    CommandButton.h \
    StoredProcedureDialog.h \
    GuiModel.h

SOURCES += MainWindow.cpp \
    Display.cpp \
    LookAndFeel.cpp \
    InputWidget.cpp \
    CommandButton.cpp \
    StoredProcedureDialog.cpp \
    GuiModel.cpp

win32:LIBS += -L$$HOME/bin -lpdCalcUtilities1 -lpdCalcBackend1
