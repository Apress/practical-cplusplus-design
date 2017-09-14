CONFIG(debug, debug|release): DEFINES += DEBUG
CONFIG(release, debug|release): DEFINES += RELEASE
VERSION = 1.0.0
VERSION_STR = '\\"$${VERSION}\\"'
DEFINES += PDCALC_VERSION=\"$${VERSION_STR}\"
CONFIG += c++14

unix:DEFINES += POSIX
win32:DEFINES += WIN32

# to avoid declspec nonsense
win32:QMAKE_LFLAGS +=  -Wl,--no-undefined --enable-runtime-pseudo-reloc
