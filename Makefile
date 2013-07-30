
#
#  ScorePress - Music Engraving Software  (scorepress-gtk)
#  Copyright (C) 2013 Dominik Lehmann
#  
#  Licensed under the EUPL, Version 1.1 or - as soon they
#  will be approved by the European Commission - subsequent
#  versions of the EUPL (the "Licence");
#  You may not use this work except in compliance with the
#  Licence.
#  
#  Unless required by applicable law or agreed to in
#  writing, software distributed under the Licence is
#  distributed on an "AS IS" basis, WITHOUT WARRANTIES OR
#  CONDITIONS OF ANY KIND, either expressed or implied.
#  See the Licence for the specific language governing
#  permissions and limitations under the Licence.
#

#
# MAKEFILE
#

# programs
SHELL     := /bin/sh
CXX       := g++
INSTALL   := /usr/bin/install -c
INSTALLU  := ${INSTALL} -m 644
PKGCONFIG := /usr/bin/pkg-config

# target filenames
PACKAGE_TARNAME := scorepress-gtk
binfile         := scorepress

# directories
prefix      := /home/dominik/sources/cpp/score/build
exec_prefix := ${prefix}
datarootdir := ${prefix}/share
datadir     := ${datarootdir}/${PACKAGE_TARNAME}
distdir     := scorepress-gtk-0.1.0

srcdir      := ./src
datasrc     := ./data
iconsrc     := ${datasrc}/icons
appiconsrc  := ${datasrc}/icons
builddir    := ./build

bindir   := ${exec_prefix}/bin
mandir   := ${datarootdir}/man/man3
docdir   := ${datarootdir}/doc/${PACKAGE_TARNAME}
htmldir  := ${docdir}/html
dvidir   := ${docdir}
pdfdir   := ${docdir}
psdir    := ${docdir}

# icons
icontheme    := hicolor
iconctx      := apps
icondir      := ${datarootdir}/icons/${icontheme}
iconfiles    := scorepress.png scorepress-sm.png
iconsizes    := 16 24 32 48 64 128
appicondir   := ${datadir}/icons/${icontheme}
appiconfiles := 
appiconsizes := 

# compiler flags
SCOREPRESSFLAGS := -I/home/dominik/sources/cpp/score/build/include/libscorepress-0.2.5
SCOREPRESSLIBS  := -lscorepress
GTKFLAGS        := -pthread -I/usr/include/gtkmm-3.0 -I/usr/lib/i386-linux-gnu/gtkmm-3.0/include -I/usr/include/atkmm-1.6 -I/usr/include/giomm-2.4 -I/usr/lib/i386-linux-gnu/giomm-2.4/include -I/usr/include/pangomm-1.4 -I/usr/lib/i386-linux-gnu/pangomm-1.4/include -I/usr/include/gtk-3.0 -I/usr/include/cairomm-1.0 -I/usr/lib/i386-linux-gnu/cairomm-1.0/include -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/gtk-3.0/unix-print -I/usr/include/gdkmm-3.0 -I/usr/lib/i386-linux-gnu/gdkmm-3.0/include -I/usr/include/atk-1.0 -I/usr/include/glibmm-2.4 -I/usr/lib/i386-linux-gnu/glibmm-2.4/include -I/usr/include/glib-2.0 -I/usr/lib/i386-linux-gnu/glib-2.0/include -I/usr/include/sigc++-2.0 -I/usr/lib/i386-linux-gnu/sigc++-2.0/include -I/usr/include/pango-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/harfbuzz  
GTKLIBS         := -lgtkmm-3.0 -latkmm-1.6 -lgdkmm-3.0 -lgiomm-2.4 -lpangomm-1.4 -lgtk-3 -lglibmm-2.4 -lcairomm-1.0 -lgdk-3 -latk-1.0 -lgio-2.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo-gobject -lpango-1.0 -lcairo -lsigc-2.0 -lgobject-2.0 -lglib-2.0  
CAIROFLAGS      := -I/usr/include/cairomm-1.0 -I/usr/lib/i386-linux-gnu/cairomm-1.0/include -I/usr/include/cairo -I/usr/include/sigc++-2.0 -I/usr/lib/i386-linux-gnu/sigc++-2.0/include -I/usr/include/glib-2.0 -I/usr/lib/i386-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12  
CAIROLIBS       := -lcairomm-1.0 -lcairo -lsigc-2.0  
PANGOFLAGS      := -pthread -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/i386-linux-gnu/glib-2.0/include  
PANGOLIBS       := -lpango-1.0 -lgobject-2.0 -lglib-2.0  
RSVGFLAGS       := -pthread -I/usr/include/librsvg-2.0 -I/usr/include/glib-2.0 -I/usr/lib/i386-linux-gnu/glib-2.0/include -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/cairo -I/usr/include/libpng12 -I/usr/include/pixman-1 -I/usr/include/freetype2  
RSVGLIBS        := -lrsvg-2 -lm -lgio-2.0 -lgdk_pixbuf-2.0 -lcairo -lgobject-2.0 -lglib-2.0  
XMLFLAGS        := -I/usr/include/libxml2  
XMLLIBS         := -lxml2  

WARNINGS := -W                      \
            -Wall                   \
            -Wextra                 \
            -Wpointer-arith         \
            -Wcast-qual             \
            -Wcast-align            \
            -Wundef                 \
            -Wwrite-strings         \
            -Winit-self             \
            -Wno-ignored-qualifiers \
            -Wno-long-long
            
CTRLFLAGS := -ansi                  \
             -pedantic              \
             -fno-nonansi-builtins

MODE    := RELEASE
DEBUG   := -ggdb -O0
PROFILE := -pg -O2
RELEASE := -O2 -fomit-frame-pointer

CONFIGFLAGS := -DDATADIR="\"${datadir}\"" -DICONDIR="\"${datarootdir}/icons\"" -DAPPICONDIR="\"${datadir}/icons\""
USERFLAGS   := 
USERLIBS    := -L/home/dominik/sources/cpp/score/build/lib 

FLAGS := ${WARNINGS} ${CTRLFLAGS} ${${MODE}} -I${srcdir} ${SCOREPRESSFLAGS} ${USERFLAGS}
LIBS  := ${SCOREPRESSLIBS} ${GTKLIBS} ${CAIROLIBS} ${PANGOLIBS} ${RSVGLIBS} ${XMLLIBS} ${USERLIBS}

# file lists
ofiles := ${builddir}/main.o            \
          ${builddir}/application.o     \
          ${builddir}/controller.o      \
          ${builddir}/cmdline_options.o \
          ${builddir}/config.o          \
          ${builddir}/rsvg_renderer.o   \
          ${builddir}/key_map.o         \
          ${builddir}/key_listener.o    \
          ${builddir}/mainwnd.o         \
          ${builddir}/score_widget.o    \
          ${builddir}/icon_manager.o

distfiles := ./AUTHORS     \
             ./configure   \
             ./install-sh  \
             ./LICENCE.pdf \
             ./Makefile.ac \
             ./README      \
             ./data        \
             ./src

#
# MAIN TARGETS
#

.PHONY: all                                               \
        install install-bin install-data                  \
        install-icons install-appicons update-icon-cache  \
        uninstall uninstall-bin uninstall-data            \
        clean distclean help                              \
        dist dist-gzip dist-bzip2 dist-zip deb

# compile
all: ${builddir}
	@echo "Making:         ${binfile}"
	@${MAKE} -s ${builddir}/${binfile}

# install
install: install-bin install-data

install-bin: ${bindir}
	@echo "Installing:     ${binfile}"
	@${INSTALL} ${builddir}/${binfile} ${bindir}

install-data: ${datadir}
	@echo "Installing Data..."
	@echo "  Installing:   logo.png"
	@${INSTALLU} -t ${datadir} ${datasrc}/logo.png
	@echo "  Installing:   EUPL.txt"
	@${INSTALLU} -t ${datadir} ${datasrc}/EUPL.txt
	@echo "  Installing:   CC-BY-SA.pdf"
	@${INSTALLU} -t ${datadir} ${datasrc}/CC-BY-SA.pdf
	@${MAKE} -s install-icons
	@${MAKE} -s install-appicons

install-icons: ${icondir}
	@echo "  Installing:   Icons (systemwide)"
	@for size in ${iconsizes}; do                                       \
	    if test ! -e ${icondir}/$${size}x$${size}; then                 \
	        mkdir ${icondir}/$${size}x$${size};                         \
	        mkdir ${icondir}/$${size}x$${size}/${iconctx};              \
	    elif test ! -e ${icondir}/$${size}x$${size}/${iconctx}; then    \
	        mkdir ${icondir}/$${size}x$${size}/${iconctx};              \
	    fi;                                                             \
	    for file in ${iconfiles}; do                                    \
	        ${INSTALLU} -t ${icondir}/$${size}x$${size}/${iconctx} ${iconsrc}/$${size}x$${size}/$${file}; \
	    done;                                                           \
	 done

install-appicons: ${appicondir}
	@echo "  Installing:   Icons (application specific)"
	@for size in ${appiconsizes}; do                                    \
	    if test ! -e ${appicondir}/$${size}x$${size}; then              \
	        mkdir ${appicondir}/$${size}x$${size};                      \
	        mkdir ${appicondir}/$${size}x$${size}/${iconctx};           \
	    elif test ! -e ${appicondir}/$${size}x$${size}/${iconctx}; then \
	        mkdir ${appicondir}/$${size}x$${size}/${iconctx};           \
	    fi;                                                             \
	    for file in ${appiconfiles}; do                                 \
	        ${INSTALLU} -t ${appicondir}/$${size}x$${size}/${iconctx} ${iconsrc}/$${size}x$${size}/$${file}; \
	    done;                                                           \
	 done

update-icon-cache:
	gtk-update-icon-cache -f -t ${icondir}

# uninstall
uninstall: uninstall-bin uninstall-data

uninstall-bin:
	@echo "Removing:       ${binfile}"
	@-rm -f ${bindir}/${binfile}

uninstall-data:
	@echo "Removing Data..."
	@echo "  Removing:     Icons"
	@for size in ${iconsizes}; do                                   \
	    for file in ${iconfiles}; do                                \
	        rm -f ${icondir}/$${size}x$${size}/${iconctx}/$${file}; \
	    done;                                                       \
	 done
	@-rm -r ${datadir}/icons
	@echo "  Removing:     logo.png"
	@-rm -f ${datadir}/logo.png
	@echo "  Removing:     EUPL.txt"
	@-rm -f ${datadir}/EUPL.txt
	@echo "  Removing:     CC-BY-SA.pdf"
	@-rm -f ${datadir}/CC-BY-SA.pdf
	@-rmdir ${datadir}

# clean
clean:
	@echo "Cleaning \"${PACKAGE_TARNAME}\""
	@echo "  Deleting:     object-files..."
	@-rm -f  ${ofiles}
	@-rm -f  ${builddir}/${binfile}
	@echo "  Deleting:     build directory..."
	@-rmdir ${builddir}

distclean: clean
	@echo "  Deleting:     configuration..."
	@-rm -f ${srcdir}/config.hh
	@-rm -f ./config.log
	@-rm -f ./config.status
	@-rm -f ./Makefile

help:
	@echo "No Help Available"

# distributions
dist: ${distdir}
	cp -RP --preserve=timestamps,links ${distfiles} ${distdir}
	if test -f ${distdir}/src/config.hh; then rm -f ${distdir}/src/config.hh; fi

dist-gzip: dist
	tar -cf - ${distdir} | gzip -c > ${distdir}.tar.gz

dist-bzip2: dist
	tar -cf - ${distdir} | bzip2 -c > ${distdir}.tar.bz2

dist-zip: dist
	zip -qr ${distdir}.zip ${distdir}

deb: dist
	@echo "Debian Packaging not implemented yet."

# directories
${builddir} ${bindir} ${datadir} ${distdir} ${icondir} ${appicondir}:
	@echo "[[Creating directory:  $@]]"
	@-mkdir -p $@


#
# TARGET FILES
#

${builddir}/${binfile}:	${ofiles}
						echo "  Linking:      ${binfile}..."
						${CXX} ${ofiles} -o ${builddir}/${binfile} ${LIBS} ${FLAGS}

#
# MAIN PROGRAM
#

${builddir}/main.o:				${srcdir}/main.cpp
								echo "  Compiling:    main.cpp..."
								${CXX} -c ${srcdir}/main.cpp -o ${builddir}/main.o ${GTKFLAGS} ${RSVGFLAGS} ${FLAGS}

${builddir}/application.o:		${srcdir}/application.cpp
								echo "  Compiling:    application.cpp..."
								${CXX} -c ${srcdir}/application.cpp -o ${builddir}/application.o ${GTKFLAGS} ${RSVGFLAGS} ${FLAGS}

${builddir}/config.o:			${srcdir}/config.cpp
								echo "  Compiling:    config.cpp..."
								${CXX} -c ${srcdir}/config.cpp -o ${builddir}/config.o ${GTKFLAGS} ${PANGOFLAGS} ${RSVGFLAGS} ${XMLFLAGS} ${FLAGS} ${CONFIGFLAGS}

${builddir}/controller.o:		${srcdir}/controller.cpp
								echo "  Compiling:    controller.cpp..."
								${CXX} -c ${srcdir}/controller.cpp -o ${builddir}/controller.o ${GTKFLAGS} ${RSVGFLAGS} ${FLAGS}

${builddir}/cmdline_options.o:	${srcdir}/cmdline_options.cpp
								echo "  Compiling:    cmdline_options.cpp..."
								${CXX} -c ${srcdir}/cmdline_options.cpp -o ${builddir}/cmdline_options.o ${FLAGS}

#
# GRAPHICAL USER INTERFACE
#

${builddir}/rsvg_renderer.o:	${srcdir}/rsvg_renderer.cpp
								echo "  Compiling:    rsvg_renderer.cpp..."
								${CXX} -c ${srcdir}/rsvg_renderer.cpp -o ${builddir}/rsvg_renderer.o ${PANGOFLAGS} ${RSVGFLAGS} ${XMLFLAGS} ${FLAGS}

${builddir}/key_map.o:			${srcdir}/key_map.cpp
								echo "  Compiling:    key_map.cpp..."
								${CXX} -c ${srcdir}/key_map.cpp -o ${builddir}/key_map.o ${FLAGS}

${builddir}/key_listener.o:		${srcdir}/key_listener.cpp
								echo "  Compiling:    key_listener.cpp..."
								${CXX} -c ${srcdir}/key_listener.cpp -o ${builddir}/key_listener.o ${FLAGS}

${builddir}/mainwnd.o:			${srcdir}/mainwnd.cpp
								echo "  Compiling:    mainwnd.cpp..."
								${CXX} -c ${srcdir}/mainwnd.cpp -o ${builddir}/mainwnd.o ${GTKFLAGS} ${RSVGFLAGS} ${FLAGS}

${builddir}/score_widget.o:		${srcdir}/score_widget.cpp
								echo "  Compiling:    score_widget.cpp..."
								${CXX} -c ${srcdir}/score_widget.cpp -o ${builddir}/score_widget.o ${GTKFLAGS} ${RSVGFLAGS} ${FLAGS}

${builddir}/icon_manager.o:		${srcdir}/icon_manager.cpp
								echo "  Compiling:    icon_manager.cpp..."
								${CXX} -c ${srcdir}/icon_manager.cpp -o ${builddir}/icon_manager.o ${GTKFLAGS} ${FLAGS}

