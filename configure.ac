# Process this file with autoconf to produce a configure script.
#
#  ScorePress - Music Engraving Software  (scorepress-gtk)
#  Copyright (C) 2014 Dominik Lehmann
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

# initialize package
AC_PREREQ([2.69])
AC_INIT([scorepress-gtk], [0.1.1])
AC_CONFIG_SRCDIR([src/score_widget.cpp])
AC_CONFIG_HEADERS([src/config.hh:src/config.hh.ac])
AC_CONFIG_FILES([Makefile:Makefile.ac])
AC_LANG([C++])

AC_SUBST([PACKAGE_VERSION_MAIN], [0])

AC_DEFINE([SCOREPRESS_PACKAGE_NAME], ["AC_PACKAGE_NAME"])
AC_DEFINE([SCOREPRESS_VERSION_STRING], ["AC_PACKAGE_VERSION"])
AC_DEFINE([SCOREPRESS_VERSION_MAJOR], [0])
AC_DEFINE([SCOREPRESS_VERSION_MINOR], [1])
AC_DEFINE([SCOREPRESS_VERSION_PATCH], [1])

# save user flags
CFLAGS_ORI="$CFLAGS"
LDFLAGS_ORI="$LDFLAGS"
LIBS_ORI="$LIBS"

# package names (for use with pkg-config)
scorepress_pkg="libscorepress"
gtkmm_pkg="gtkmm-3.0"
cairomm_pkg="cairomm-1.0"
pango_pkg="pango"
rsvg_pkg="librsvg-2.0"
libxml_pkg="libxml-2.0"

# check commandline arguments
#AC_PRESERVE_HELP_ORDER
AC_ARG_ENABLE([debug],
            [AS_HELP_STRING([--enable-debug], [enable debugging with GDB])],
            [with_debug="$withval"],
            [with_debug="no"])
AC_ARG_ENABLE([profile],
            [AS_HELP_STRING([--enable-profile], [enable profiling with GProf])],
            [with_profile="$withval"],
            [with_profile="no"])
AC_ARG_WITH([libscorepress],
            [AS_HELP_STRING([--with-libscorepress], [location of the libscorepress headers [/usr/include/libscorepress]])],
            [with_libscorepress="$withval"],
            [with_libscorepress="auto"])
AC_ARG_WITH([libxml2],
            [AS_HELP_STRING([--with-libxml2], [location of the libxml2 headers [/usr/include/libxml2]])],
            [with_libxml2="$withval"],
            [with_libxml2="auto"])
AC_ARG_WITH([rpath],
            [AS_HELP_STRING([--with-rpath], [add the given path to the runtime library search path])],
            [with_rpath="$withval"],
            [with_rpath=""])

AC_SUBST([MODE], [RELEASE])

AS_IF([test "x$with_debug" != "xno"],
      [AC_DEFINE([SCOREPRESS_DEBUG], [1])],
      [AC_DEFINE([SCOREPRESS_DEBUG], [0])])
AS_IF([test "x$with_profile" != "xno"],
      [AC_SUBST([MODE], [PROFILE])])
AS_IF([test "x$with_debug" != "xno"],
      [AC_SUBST([MODE], [DEBUG])])

AC_SUBST([USER_RPATH], [$with_rpath])

# check shell
AS_IF([test "x$SHELL" != "x"],
    [AC_SUBST([SH], [$SHELL])],
    [AC_PATH_PROG([SH], [/bin/sh])])

# check compiler
AC_PROG_CXX

# set compiler specific flags
if test "x$CXX" = "xgcc" || test "x$CXX" = "xg++"; then
    AC_CHECK_PROG([DEBUG], [gdb], [-ggdb], [-g])
    AC_SUBST([DEBUG],      ["$DEBUG -fno-inline -Werror"])
    AC_SUBST([PROFILE],    ["-O2 -fno-omit-frame-pointer"])
    AC_SUBST([RELEASE],    ["-O2"])
    AS_IF([test "x$with_rpath" == "xno"],
        [AC_SUBST([RPATHFLAG],  [])],
        [AC_SUBST([RPATHFLAG],  ["-Wl,-rpath,"])])
    AC_SUBST([WARNINGS],   ["-W                   \\
             -Wall                \\
             -Wextra              \\
             -Wcast-qual          \\
             -Wcast-align         \\
             -Wconversion         \\
             -Wfloat-equal        \\
             -Winit-self          \\
             -Wno-long-long       \\
             -Wpointer-arith      \\
             -Wshadow             \\
             -Wundef              \\
             -Wunreachable-code   \\
             -Wwrite-strings"])
    AC_SUBST([CTRLFLAGS],  ["-ansi                \\
             -pedantic            \\
             -fno-nonansi-builtins"])
    
    # append runtime library search path option to the linker flags
    if test -n "$with_rpath"; then
        LDFLAGS_ORI="$LDFLAGS_ORI -Wl,-R,$with_rpath"
    fi
    
    # set compiler-version specific flags
    AS_IF([gcc_version=`$CXX -dumpversion 2> /dev/null`],[
            if test `echo -n $gcc_version | cut -d'.' -f1` -eq 4 &&
               test `echo -n $gcc_version | cut -d'.' -f2` -ge 8 ||
               test `echo -n $gcc_version | cut -d'.' -f1` -gt 4
            then
                AC_SUBST([DEBUG], ["$DEBUG -Og"])
            else
                AC_SUBST([DEBUG], ["$DEBUG -O0"])
            fi ])
fi

# check for programs
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PATH_PROG([AR], [ar])
AC_PATH_PROG([LDCONFIG], [ldconfig])
AC_PATH_PROG([SED], [sed])
AC_PATH_PROG([PKGCONFIG], [pkg-config])

# check libraries
#   libscorepress
if test "x$with_libscorepress" != "xauto"; then
    AC_SUBST([LIBSCOREPRESS_CFLAGS], [-I$with_libscorepress])
    AC_SUBST([LIBSCOREPRESS_LIBS], [-lscorepress])
    AC_SUBST([USER_LIBSCOREPRESS], [$with_libscorepress])
elif test -n "$PKGCONFIG" -a `$PKGCONFIG --exists $scorepress_pkg; echo $?` -eq 0; then
    AC_SUBST([LIBSCOREPRESS_CFLAGS], [`$PKGCONFIG $scorepress_pkg --cflags`])
    AC_SUBST([LIBSCOREPRESS_LIBS], [`$PKGCONFIG $scorepress_pkg --libs`])
    AC_SUBST([USER_LIBSCOREPRESS], [])
else
    AC_SUBST([LIBSCOREPRESS_CFLAGS], [-I/usr/include/libscorepress])
    AC_SUBST([LIBSCOREPRESS_LIBS], [-lscorepress])
    AC_SUBST([USER_LIBSCOREPRESS], [])
fi

CFLAGS="$CFLAGS_ORI $LIBSCOREPRESS_CFLAGS"
LDFLAGS="$LDFLAGS_ORI $LIBSCOREPRESS_LIBS"
LIBS="$LIBS_ORI $LIBSCOREPRESS_LIBS"
AC_CHECK_LIB([scorepress], [main],, AC_MSG_ERROR([cannot find library '$scorepress_pkg']))

#   libgtkmm
AS_IF([test -n "$PKGCONFIG" -a `$PKGCONFIG --exists $gtkmm_pkg; echo $?` -ne 0],
    AC_MSG_ERROR([pkg-config cannot find library '$gtkmm_pkg']))
AC_SUBST([LIBGTKMM_CFLAGS], [`$PKGCONFIG $gtkmm_pkg --cflags`])
AC_SUBST([LIBGTKMM_LIBS], [`$PKGCONFIG $gtkmm_pkg --libs`])

CFLAGS="$CFLAGS_ORI $LIBGTKMM_CFLAGS"
LDFLAGS="$LDFLAGS_ORI $LIBGTKMM_LIBS"
LIBS="$LIBS_ORI $LIBGTKMM_LIBS"
AC_CHECK_LIB([$gtkmm_pkg], [_init],, AC_MSG_ERROR([cannot find library '$gtkmm_pkg']))

#   libcairomm
AS_IF([test -n "$PKGCONFIG" -a `$PKGCONFIG --exists $cairomm_pkg; echo $?` -ne 0],
    AC_MSG_ERROR([pkg-config cannot find library '$cairomm_pkg']))
AC_SUBST([LIBCAIROMM_CFLAGS], [`$PKGCONFIG $cairomm_pkg --cflags`])
AC_SUBST([LIBCAIROMM_LIBS], [`$PKGCONFIG $cairomm_pkg --libs`])

CFLAGS="$CFLAGS_ORI $LIBCAIROMM_CFLAGS"
LDFLAGS="$LDFLAGS_ORI $LIBCAIROMM_LIBS"
LIBS="$LIBS_ORI $LIBCAIROMM_LIBS"
AC_CHECK_LIB([$cairomm_pkg], [_init],, AC_MSG_ERROR([cannot find library '$cairomm_pkg']))

#   libpango
AS_IF([test -n "$PKGCONFIG" -a `$PKGCONFIG --exists $pango_pkg; echo $?` -ne 0],
    AC_MSG_ERROR([pkg-config cannot find library '$pango_pkg']))
AC_SUBST([LIBPANGO_CFLAGS], [`$PKGCONFIG $pango_pkg --cflags`])
AC_SUBST([LIBPANGO_LIBS], [`$PKGCONFIG $pango_pkg --libs`])

CFLAGS="$CFLAGS_ORI $LIBPANGO_CFLAGS"
LDFLAGS="$LDFLAGS_ORI $LIBPANGO_LIBS"
LIBS="$LIBS_ORI $LIBPANGO_LIBS"
AC_CHECK_LIB([pango-1.0], [pango_context_new],, AC_MSG_ERROR([cannot find library 'pango-1.0']))
AC_CHECK_LIB([pangocairo-1.0], [pango_cairo_create_context],, AC_MSG_ERROR([cannot find library 'pangocairo']))

#   librsvg
AS_IF([test -n "$PKGCONFIG" -a `$PKGCONFIG --exists $rsvg_pkg; echo $?` -ne 0],
    AC_MSG_ERROR([pkg-config cannot find library '$rsvg_pkg']))
AC_SUBST([LIBRSVG_CFLAGS], [`$PKGCONFIG $rsvg_pkg --cflags`])
AC_SUBST([LIBRSVG_LIBS], [`$PKGCONFIG $rsvg_pkg --libs`])

CFLAGS="$CFLAGS_ORI $LIBRSVG_CFLAGS"
LDFLAGS="$LDFLAGS_ORI $LIBRSVG_LIBS"
LIBS="$LIBS_ORI $LIBRSVG_LIBS"
AC_CHECK_LIB([rsvg-2], [rsvg_handle_new],, AC_MSG_ERROR([cannot find library '$rsvg_pkg']))

#   libxml
if test "x$with_libxml2" != "xauto"; then
    AC_SUBST([LIBXML_CFLAGS], [-I$with_libxml2])
    AC_SUBST([LIBXML_LIBS], [-lxml2])
    AC_SUBST([USER_LIBXML], [$with_libxml2])
elif test -n "$PKGCONFIG" -a `$PKGCONFIG --exists $libxml_pkg; echo $?` -eq 0; then
    AC_SUBST([LIBXML_CFLAGS], [`$PKGCONFIG $libxml_pkg --cflags`])
    AC_SUBST([LIBXML_LIBS], [`$PKGCONFIG $libxml_pkg --libs`])
    AC_SUBST([USER_LIBXML], [])
else
    AC_SUBST([LIBXML_CFLAGS], [-I/usr/include/libxml2])
    AC_SUBST([LIBXML_LIBS], [-lxml2])
    AC_SUBST([USER_LIBXML], [])
fi

CFLAGS="$CFLAGS_ORI $LIBXML_CFLAGS"
LDFLAGS="$LDFLAGS_ORI $LIBXML_LIBS"
LIBS="$LIBS_ORI $LIBXML_LIBS"
AC_CHECK_LIB([xml2], [xmlReaderForFile],, AC_MSG_ERROR([cannot find library 'libxml2']))

# check headers
AC_CHECK_HEADER([cstdlib],,  AC_MSG_ERROR([Failed to find <cstdlib> header.]))
AC_CHECK_HEADER([cstddef],,  AC_MSG_ERROR([Failed to find <cstddef> header.]))
AC_CHECK_HEADER([cmath],,    AC_MSG_ERROR([Failed to find <cmath> header.]))
AC_CHECK_HEADER([climits],,  AC_MSG_ERROR([Failed to find <climits> header.]))
AC_CHECK_HEADER([cstring],,  AC_MSG_ERROR([Failed to find <cstring> header.]))
AC_CHECK_HEADER([iostream],, AC_MSG_ERROR([Failed to find <iostream> header.]))
AC_CHECK_HEADER([string],,   AC_MSG_ERROR([Failed to find <string> header.]))
AC_CHECK_HEADER([vector],,   AC_MSG_ERROR([Failed to find <vector> header.]))
AC_CHECK_HEADER([list],,     AC_MSG_ERROR([Failed to find <list> header.]))
AC_CHECK_HEADER([deque],,    AC_MSG_ERROR([Failed to find <deque> header.]))
AC_CHECK_HEADER([map],,      AC_MSG_ERROR([Failed to find <map> header.]))
AC_CHECK_HEADER([set],,      AC_MSG_ERROR([Failed to find <set> header.]))

CPPFLAGS="$CFLAGS_ORI $LIBSCOREPRESS_CFLAGS"
CFLAGS="$CFLAGS_ORI $LIBSCOREPRESS_CFLAGS"
AC_CHECK_HEADER([scorepress/engine.hh],, AC_MSG_ERROR([Failed to find <scorepress/engine.hh>]))
AC_CHECK_HEADER([scorepress/engraver.hh],, AC_MSG_ERROR([Failed to find <scorepress/engraver.hh>]))

CPPFLAGS="$CFLAGS_ORI $LIBPANGO_CFLAGS $LIBCAIROMM_CFLAGS"
CFLAGS="$CFLAGS_ORI $LIBPANGO_CFLAGS $LIBCAIROMM_CFLAGS"
AC_CHECK_HEADER([pango/pangocairo.h],, AC_MSG_ERROR([Failed to find <pango/pangocairo.h>]))

CPPFLAGS="$CFLAGS_ORI $LIBRSVG_CFLAGS"
CFLAGS="$CFLAGS_ORI $LIBRSVG_CFLAGS"
AC_CHECK_HEADER([librsvg/rsvg.h],,      AC_MSG_ERROR([Failed to find <librsvg/rsvg.h>]))
AC_CHECK_HEADER([librsvg/rsvg-cairo.h],,AC_MSG_ERROR([Failed to find <librsvg/rsvg-cairo.h>]), [#include <librsvg/rsvg.h>])

CPPFLAGS="$CFLAGS_ORI $LIBXML_CFLAGS"
CFLAGS="$CFLAGS_ORI $LIBXML_CFLAGS"
AC_CHECK_HEADER([libxml/xmlreader.h],, AC_MSG_ERROR([Failed to find <libxml/xmlreader.h>]))

# check types
AC_TYPE_SIZE_T

# check functions
CFLAGS="$CFLAGS_ORI $LIBRSVG_CFLAGS"
LDFLAGS="$LDFLAGS_ORI $LIBRSVG_LIBS"
LIBS="$LIBS_ORI $LIBRSVG_LIBS"
AC_CHECK_FUNC([rsvg_handle_get_position_sub],, AC_MSG_ERROR([library '$rsvg_pkg' cannot get subpath positions]))
AC_CHECK_FUNC([rsvg_handle_get_dimensions_sub],, AC_MSG_ERROR([library '$rsvg_pkg' cannot get subpath dimensions]))

CFLAGS="$CFLAGS_ORI $LIBXML_CFLAGS"
LDFLAGS="$LDFLAGS_ORI $LIBXML_LIBS"
LIBS="$LIBS_ORI $LIBXML_LIBS"
AC_CHECK_FUNC([xmlReaderForFile],, AC_MSG_ERROR([library 'libxml2' does not provide the 'xmlReader' interface]))
AC_CHECK_FUNC([xmlReaderForMemory],, AC_MSG_ERROR([library 'libxml2' does not provide the 'xmlReader' interface]))

# create rpath
RPATH=
if test -n "$RPATHFLAG"; then
    buffer=$LIBSCOREPRESS_LIBS
    for path in $buffer; do
        path=$(expr "$path" : "-L\(.*\)")
        if test -n "$path"; then
            LIBSCOREPRESS_LIBS="$LIBSCOREPRESS_LIBS $RPATHFLAG$path"
        fi
    done
    AC_SUBST([LIBSCOREPRESS_LIBS], [$LIBSCOREPRESS_LIBS])
    
    buffer=$LIBGTKMM_LIBS
    for path in $buffer; do
        path=$(expr "$path" : "-L\(.*\)")
        if test -n "$path"; then
            LIBGTKMM_LIBS="$LIBGTKMM_LIBS $RPATHFLAG$path"
        fi
    done
    AC_SUBST([LIBGTKMM_LIBS], [$LIBGTKMM_LIBS])
    
    buffer=$LIBCAIROMM_LIBS
    for path in $buffer; do
        path=$(expr "$path" : "-L\(.*\)")
        if test -n "$path"; then
            LIBCAIROMM_LIBS="$LIBCAIROMM_LIBS $RPATHFLAG$path"
        fi
    done
    AC_SUBST([LIBCAIROMM_LIBS], [$LIBCAIROMM_LIBS])
    
    buffer=$LIBPANGO_LIBS
    for path in $buffer; do
        path=$(expr "$path" : "-L\(.*\)")
        if test -n "$path"; then
            LIBPANGO_LIBS="$LIBPANGO_LIBS $RPATHFLAG$path"
        fi
    done
    AC_SUBST([LIBPANGO_LIBS], [$LIBPANGO_LIBS])
    
    buffer=$LIBRSVG_LIBS
    for path in $buffer; do
        path=$(expr "$path" : "-L\(.*\)")
        if test -n "$path"; then
            LIBRSVG_LIBS="$LIBRSVG_LIBS $RPATHFLAG$path"
        fi
    done
    AC_SUBST([LIBRSVG_LIBS], [$LIBRSVG_LIBS])
    
    buffer=$LIBXML_LIBS
    for path in $buffer $USER_LIBXML; do
        path=$(expr "$path" : "-L\(.*\)")
        if test -n "$path"; then
            LIBXML_LIBS="$LIBXML_LIBS $RPATHFLAG$path"
        fi
    done
    AC_SUBST([LIBXML_LIBS], [$LIBXML_LIBS])
    
    if test "x$with_rpath" != "xno" -a "x$with_rpath" != "xyes" -a "x$with_rpath" != "x"; then
        RPATH="$RPATHFLAG$with_rpath"
    fi
    
    for path in $LDFLAGS; do
        path=$(expr "$path" : "-L\(.*\)")
        if test -n "$path"; then
            RPATH="$RPATH $RPATHFLAG$path"
        fi
    done
fi
AC_SUBST([RPATH], [$RPATH])

# generate output
AC_SUBST([CFLAGS],[$CFLAGS_ORI])
AC_SUBST([LDFLAGS],[$LDFLAGS_ORI])
AC_SUBST([LIBS],[$LIBS_ORI])
AC_OUTPUT()

