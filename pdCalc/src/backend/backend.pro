HOME = ../..
include ($$HOME/common.pri)
TEMPLATE = lib
TARGET = pdCalcBackend
DEPENDPATH += .
INCLUDEPATH += . $$HOME/src
unix:DESTDIR = $$HOME/lib
win32:DESTDIR = $$HOME/bin


win32:DEFINES += _USE_MATH_DEFINES

# Input
HEADERS += Stack.h \
    StackPluginInterface.h \
    Command.h \
    CommandManager.h \
    CommandRepository.h \
    CommandDispatcher.h \
    CoreCommands.h \
    StoredProcedure.h \
    PluginLoader.h \
    DynamicLoader.h \
    Plugin.h \
    PlatformFactory.h \
    StackPluginInterface.h \
    AppObservers.h

unix:HEADERS += PosixDynamicLoader.h \
    PosixFactory.h

win32:HEADERS += WindowsDynamicLoader.h \
                 WindowsFactory.h

SOURCES += Stack.cpp \
    StackPluginInterface.cpp \
    CommandManager.cpp \
    CommandRepository.cpp \
    CommandDispatcher.cpp \
    Command.cpp \
    CoreCommands.cpp \
    StoredProcedure.cpp \
    PluginLoader.cpp \
    DynamicLoader.cpp \
    PlatformFactory.cpp \
    AppObservers.cpp

unix:SOURCES += PosixDynamicLoader.cpp \
                PosixFactory.cpp

win32:SOURCES += WindowsDynamicLoader.cpp \
                 WindowsFactory.cpp

unix:LIBS += -ldl
win32:LIBS += -L$$HOME/bin -lpdCalcUtilities1
