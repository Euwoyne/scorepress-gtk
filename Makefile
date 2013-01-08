
#
#  ScorePress - Music Engraving Software  (scorepress-gtk)
#  Copyright (C) 2012 Dominik Lehmann
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

CXX := g++
CMAKE := make
INSTALL := install

SRCDIR   := src
BUILDDIR := obj
BINDIR   := ..

BINFILE  := score

MODE := DEBUG


INC_GTK   := `pkg-config gtkmm-3.0 --cflags`
INC_CAIRO := `pkg-config cairomm-1.0 --cflags`
INC_PANGO := `pkg-config pango --cflags`
INC_RSVG  := -I/home/dominik/sources/cpp/score/librsvg/include/librsvg-2.0 \
             `pkg-config librsvg-2.0 --cflags`
INC_XML   := `pkg-config libxml-2.0 --cflags`

LIB := `pkg-config gtkmm-3.0 --libs`   \
	   `pkg-config cairomm-1.0 --libs` \
	   `pkg-config pango --libs`       \
	   -L/home/dominik/sources/cpp/score/librsvg/lib \
	   `pkg-config librsvg-2.0 --libs` \
	   `pkg-config libxml-2.0 --libs`  \
	   -L../libscorepress -lscorepress


WARNINGS := -W                      \
            -Wall                   \
            -Wextra                 \
            -Wpointer-arith         \
            -Wcast-qual             \
            -Wcast-align            \
            -Wundef                 \
            -Wwrite-strings         \
            -Wmissing-declarations  \
            -Winit-self             \
            -Wno-ignored-qualifiers \
            -Wno-long-long
            
CTRLFLAGS := -ansi                 \
             -pedantic             \
             -fno-nonansi-builtins

DEBUG   := -ggdb -O0
RELEASE := -O2 -fomit-frame-pointer

FLAGS := $(WARNINGS) $(CTRLFLAGS) $($(MODE)) -I`pwd`/$(SRCDIR)  -I`pwd`/../libscorepress/$(SRCDIR)

FILES := $(BUILDDIR)/main.o \
	     $(BUILDDIR)/application.o \
	     $(BUILDDIR)/controller.o \
	     $(BUILDDIR)/configuration.o \
	     $(BUILDDIR)/cmdline_options.o \
	     $(BUILDDIR)/version.o \
	     \
	     $(BUILDDIR)/rsvg_renderer.o \
	     $(BUILDDIR)/key_map.o \
	     $(BUILDDIR)/key_listener.o \
	     $(BUILDDIR)/mainwnd.o \
	     $(BUILDDIR)/score_widget.o \
	     $(BUILDDIR)/icon_manager.o


#
# MAIN TARGETS
#

build: $(BUILDDIR) $(BUILDDIR)/$(BINFILE)

$(BUILDDIR):
	-mkdir $(BUILDDIR)

install:
	$(INSTALL) -m 0775 $(BUILDDIR)/$(BINFILE) $(BINDIR)

dist-clean:
	-rm -r $(BUILDDIR)

uninstall:
	-rm -f $(BINDIR)/$(BINFILE)

clean: dist-clean uninstall
clear: dist-clean uninstall


#
# TARGET FILES
#

$(BUILDDIR)/$(BINFILE):	$(FILES)
						$(CXX) $(FILES) -o $(BUILDDIR)/$(BINFILE) $(LIB) $(FLAGS)

#
# MAIN PROGRAM
#

$(BUILDDIR)/main.o:				$(SRCDIR)/main.cpp
								$(CXX) -c $(SRCDIR)/main.cpp -o $(BUILDDIR)/main.o $(INC_GTK) $(INC_RSVG) $(FLAGS)

$(BUILDDIR)/application.o:		$(SRCDIR)/application.cpp
								$(CXX) -c $(SRCDIR)/application.cpp -o $(BUILDDIR)/application.o $(INC_GTK) $(INC_RSVG) $(FLAGS)

$(BUILDDIR)/controller.o:		$(SRCDIR)/controller.cpp
								$(CXX) -c $(SRCDIR)/controller.cpp -o $(BUILDDIR)/controller.o $(INC_GTK) $(INC_RSVG) $(FLAGS)

$(BUILDDIR)/configuration.o:	$(SRCDIR)/configuration.cpp
								$(CXX) -c $(SRCDIR)/configuration.cpp -o $(BUILDDIR)/configuration.o $(FLAGS)

$(BUILDDIR)/cmdline_options.o:	$(SRCDIR)/cmdline_options.cpp
								$(CXX) -c $(SRCDIR)/cmdline_options.cpp -o $(BUILDDIR)/cmdline_options.o $(FLAGS)

$(BUILDDIR)/version.o:			$(SRCDIR)/version.cpp
								$(CXX) -c $(SRCDIR)/version.cpp -o $(BUILDDIR)/version.o $(INC_GTK) $(INC_RSVG) $(INC_XML) $(FLAGS)


#
# GRAPHICAL USER INTERFACE
#

$(BUILDDIR)/rsvg_renderer.o:	$(SRCDIR)/rsvg_renderer.cpp
								$(CXX) -c $(SRCDIR)/rsvg_renderer.cpp -o $(BUILDDIR)/rsvg_renderer.o $(INC_XML) $(INC_RSVG) $(INC_PANGO) $(FLAGS)

$(BUILDDIR)/key_map.o:			$(SRCDIR)/key_map.cpp
								$(CXX) -c $(SRCDIR)/key_map.cpp -o $(BUILDDIR)/key_map.o $(FLAGS)

$(BUILDDIR)/key_listener.o:		$(SRCDIR)/key_listener.cpp
								$(CXX) -c $(SRCDIR)/key_listener.cpp -o $(BUILDDIR)/key_listener.o $(FLAGS)

$(BUILDDIR)/mainwnd.o:			$(SRCDIR)/mainwnd.cpp
								$(CXX) -c $(SRCDIR)/mainwnd.cpp -o $(BUILDDIR)/mainwnd.o $(INC_GTK) $(INC_RSVG) $(FLAGS)

$(BUILDDIR)/score_widget.o:		$(SRCDIR)/score_widget.cpp
								$(CXX) -c $(SRCDIR)/score_widget.cpp -o $(BUILDDIR)/score_widget.o $(INC_GTK) $(INC_RSVG) $(FLAGS)

$(BUILDDIR)/icon_manager.o:		$(SRCDIR)/icon_manager.cpp
								$(CXX) -c $(SRCDIR)/icon_manager.cpp -o $(BUILDDIR)/icon_manager.o $(INC_GTK) $(FLAGS)

