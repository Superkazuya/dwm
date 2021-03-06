# dwm version
VERSION = 6.1-custom

# Customize below to fit your system

# paths
PREFIX = /usr

X11INC = -I/usr/include/X11
X11LIB = -L/usr/lib/X11 -lX11

# Xinerama, comment if you don't want it
XINERAMALIB = -lXinerama

# Xft
XFTINC = -I/usr/include/freetype2
XFTLIB = -lXft

# includes and libs
INCS = ${X11INC} ${XFTINC}  `pkg-config --cflags xft pango pangoxft` 
LIBS = ${X11LIB} ${XFTLIB} ${XINERAMALIB} `pkg-config --libs xft pango pangoxft`

# flags
CFLAGS   = -std=c99 -Wall -pedantic -Os ${INCS} -DVERSION=\"${VERSION}\"
LDFLAGS  = -s ${LIBS}

# compiler and linker
CC = cc
