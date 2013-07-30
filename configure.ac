# Process this file with autoconf to produce a configure script.

# initialize package
AC_PREREQ([2.69])
AC_INIT([scorepress-gtk], [0.1.0])
AC_CONFIG_SRCDIR([src/score_widget.cpp])
AC_CONFIG_HEADERS([src/config.hh:src/config.hh.ac])
AC_LANG([C++])

AC_SUBST([PACKAGE_VERSION_MAIN], [0])
AC_DEFINE([SCOREPRESS_PACKAGE_NAME], ["AC_PACKAGE_NAME"])
AC_DEFINE([SCOREPRESS_VERSION_STRING], ["AC_PACKAGE_VERSION"])

# save user flags
CFLAGS_ORI="$CFLAGS"
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
AC_ARG_WITH([libxml],
            [AS_HELP_STRING([--with-libxml], [location of the libxml2 headers [/usr/include/libxml2]])],
            [with_libxml="$withval"],
            [with_libxml="auto"])

AS_IF([test "x$with_debug" != "xno"],
    AC_DEFINE([SCOREPRESS_DEBUG], [1]),
    AC_DEFINE([SCOREPRESS_DEBUG], [0]))
AC_SUBST([MODE], [RELEASE])

AS_IF([test "x$with_profile" != "xno"],
    AC_SUBST([MODE], [PROFILE]))

AS_IF([test "x$with_debug" != "xno"],
    AC_SUBST([MODE], [DEBUG]))

# check for programs
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PATH_PROG([AR], [ar])
AC_PATH_PROG([LDCONFIG], [ldconfig])
AC_PATH_PROG([PKGCONFIG], [pkg-config])

# check libraries
#   libscorepress
if test "x$with_libscorepress" != "xauto"; then
    AC_SUBST([LIBSCOREPRESS_CFLAGS], [-I$with_libscorepress])
    AC_SUBST([LIBSCOREPRESS_LIBS], [-lscorepress])
elif test -n "$PKGCONFIG" -a `$PKGCONFIG --exists $scorepress_pkg; echo $?` -eq 0; then
    AC_SUBST([LIBSCOREPRESS_CFLAGS], [`$PKGCONFIG $scorepress_pkg --cflags`])
    AC_SUBST([LIBSCOREPRESS_LIBS], [`$PKGCONFIG $scorepress_pkg --libs`])
else
    AC_SUBST([LIBSCOREPRESS_CFLAGS], [-I/usr/include/libscorepress])
    AC_SUBST([LIBSCOREPRESS_LIBS], [-lscorepress])
fi
CFLAGS="$LIBSCOREPRESS_CFLAGS $CFLAGS_ORI"
AC_CHECK_LIB([scorepress], [main],, AC_MSG_ERROR([cannot find library '$scorepress_pkg']))

#   libgtkmm
AS_IF([test -n "$PKGCONFIG" -a `$PKGCONFIG --exists $gtkmm_pkg; echo $?` -ne 0],
    AC_MSG_ERROR([pkg-config cannot find library '$gtkmm_pkg']))
AC_SUBST([LIBGTKMM_CFLAGS], [`$PKGCONFIG $gtkmm_pkg --cflags`])
AC_SUBST([LIBGTKMM_LIBS], [`$PKGCONFIG $gtkmm_pkg --libs`])
CFLAGS="$LIBGTKMM_CFLAGS $CFLAGS_ORI"
AC_CHECK_LIB([$gtkmm_pkg], [_init],, AC_MSG_ERROR([cannot find library '$gtkmm_pkg']))

#   libcairomm
AS_IF([test -n "$PKGCONFIG" -a `$PKGCONFIG --exists $cairomm_pkg; echo $?` -ne 0],
    AC_MSG_ERROR([pkg-config cannot find library '$cairomm_pkg']))
AC_SUBST([LIBCAIROMM_CFLAGS], [`$PKGCONFIG $cairomm_pkg --cflags`])
AC_SUBST([LIBCAIROMM_LIBS], [`$PKGCONFIG $cairomm_pkg --libs`])
CFLAGS="$LIBCAIROMM_CFLAGS $CFLAGS_ORI"
AC_CHECK_LIB([$cairomm_pkg], [_init],, AC_MSG_ERROR([cannot find library '$cairomm_pkg']))

#   libpango
AS_IF([test -n "$PKGCONFIG" -a `$PKGCONFIG --exists $pango_pkg; echo $?` -ne 0],
    AC_MSG_ERROR([pkg-config cannot find library '$pango_pkg']))
AC_SUBST([LIBPANGO_CFLAGS], [`$PKGCONFIG $pango_pkg --cflags`])
AC_SUBST([LIBPANGO_LIBS], [`$PKGCONFIG $pango_pkg --libs`])
CFLAGS="$LIBPANGO_CFLAGS $CFLAGS_ORI"
AC_CHECK_LIB([pango-1.0], [pango_context_new],, AC_MSG_ERROR([cannot find library 'pango-1.0']))
AC_CHECK_LIB([pangocairo-1.0], [pango_cairo_create_context],, AC_MSG_ERROR([cannot find library 'pangocairo']))

#   librsvg
AS_IF([test -n "$PKGCONFIG" -a `$PKGCONFIG --exists $rsvg_pkg; echo $?` -ne 0],
    AC_MSG_ERROR([pkg-config cannot find library '$rsvg_pkg']))
AC_SUBST([LIBRSVG_CFLAGS], [`$PKGCONFIG $rsvg_pkg --cflags`])
AC_SUBST([LIBRSVG_LIBS], [`$PKGCONFIG $rsvg_pkg --libs`])
CFLAGS="$LIBRSVG_CFLAGS $CFLAGS_ORI"
AC_CHECK_LIB([rsvg-2], [rsvg_handle_new],, AC_MSG_ERROR([cannot find library '$rsvg_pkg']))

#   libxml
if test "x$with_libxml" != "xauto"; then
    AC_SUBST([LIBXML_CFLAGS], [-I$with_libxml])
    AC_SUBST([LIBXML_LIBS], [-lxml2])
elif test -n "$PKGCONFIG" -a `$PKGCONFIG --exists $libxml_pkg; echo $?` -eq 0; then
    AC_SUBST([LIBXML_CFLAGS], [`$PKGCONFIG $libxml_pkg --cflags`])
    AC_SUBST([LIBXML_LIBS], [`$PKGCONFIG $libxml_pkg --libs`])
else
    AC_SUBST([LIBXML_CFLAGS], [-I/usr/include/libxml2])
    AC_SUBST([LIBXML_LIBS], [-lxml2])
fi
CFLAGS="$LIBXML_CFLAGS $CFLAGS_ORI"
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

CPPFLAGS="$LIBSCOREPRESS_CFLAGS $CFLAGS_ORI"
CFLAGS="$LIBSCOREPRESS_CFLAGS $CFLAGS_ORI"
AC_CHECK_HEADER([scorepress/engine.hh],, AC_MSG_ERROR([Failed to find <scorepress/engine.hh>]))
AC_CHECK_HEADER([scorepress/engraver.hh],, AC_MSG_ERROR([Failed to find <scorepress/engraver.hh>]))

CPPFLAGS="$LIBPANGO_CFLAGS $LIBCAIROMM_CFLAGS $CFLAGS_ORI"
CFLAGS="$LIBPANGO_CFLAGS $LIBCAIROMM_CFLAGS $CFLAGS_ORI"
AC_CHECK_HEADER([pango/pangocairo.h],, AC_MSG_ERROR([Failed to find <pango/pangocairo.h>]))

CPPFLAGS="$LIBRSVG_CFLAGS $CFLAGS_ORI"
CFLAGS="$LIBRSVG_CFLAGS $CFLAGS_ORI"
AC_CHECK_HEADER([librsvg/rsvg.h],,      AC_MSG_ERROR([Failed to find <librsvg/rsvg.h>]))
AC_CHECK_HEADER([librsvg/rsvg-cairo.h],,AC_MSG_ERROR([Failed to find <librsvg/rsvg-cairo.h>]), [#include <librsvg/rsvg.h>])

CPPFLAGS="$LIBXML_CFLAGS $CFLAGS_ORI"
CFLAGS="$LIBXML_CFLAGS $CFLAGS_ORI"
AC_CHECK_HEADER([libxml/xmlreader.h],, AC_MSG_ERROR([Failed to find <libxml/xmlreader.h>]))

# check types
AC_TYPE_SIZE_T

# check functions
CFLAGS="$LIBRSVG_CFLAGS $CFLAGS_ORI"
AC_CHECK_FUNC([rsvg_handle_get_position_sub],, AC_MSG_ERROR([library '$rsvg_pkg' cannot get subpath positions]))
AC_CHECK_FUNC([rsvg_handle_get_dimensions_sub],, AC_MSG_ERROR([library '$rsvg_pkg' cannot get subpath dimensions]))

CPPFLAGS="$LIBXML_CFLAGS $CFLAGS_ORI"
AC_CHECK_FUNC([xmlReaderForFile],, AC_MSG_ERROR([library 'libxml2' does not provide the 'xmlReader' interface]))
AC_CHECK_FUNC([xmlReaderForMemory],, AC_MSG_ERROR([library 'libxml2' does not provide the 'xmlReader' interface]))

# generate output
AC_SUBST([CFLAGS],[$CFLAGS_ORI])
AC_SUBST([LIBS],[$LIBS_ORI])
AC_CONFIG_FILES([Makefile:Makefile.ac])
AC_OUTPUT()

