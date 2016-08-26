
/*
  ScorePress - Music Engraving Software  (libscorepress-gtk)
  Copyright (C) 2016 Dominik Lehmann
  
  Licensed under the EUPL, Version 1.1 or - as soon they
  will be approved by the European Commission - subsequent
  versions of the EUPL (the "Licence");
  You may not use this work except in compliance with the
  Licence.
  
  Unless required by applicable law or agreed to in
  writing, software distributed under the Licence is
  distributed on an "AS IS" basis, WITHOUT WARRANTIES OR
  CONDITIONS OF ANY KIND, either expressed or implied.
  See the Licence for the specific language governing
  permissions and limitations under the Licence.
*/

#include <iostream>
#include <scorepress/config.hh>
#include <gtkmm.h>
#include <cairommconfig.h>
#include <pango/pango-features.h>
#include <librsvg/rsvg.h>
#ifndef LIBRSVG_FEATURES_H
#   include <librsvg/librsvg-features.h>
#endif
#include <libxml/xmlversion.h>

#include "config.hh"

const ScorePressGtk_Config scorepress_gtk_config =
{
    PREFIX,
    DATADIR,
    DESTDIR,
    ICONDIR,
    APPICONDIR
};

void print_library_info()
{
    g_print(" - libscorepress %i.%i.%i\n", LIBSCOREPRESS_VERSION_MAJOR, LIBSCOREPRESS_VERSION_MINOR, LIBSCOREPRESS_VERSION_PATCH);
    g_print(" - libgtkmm      %i.%i.%i\n", GTKMM_MAJOR_VERSION        , GTKMM_MINOR_VERSION        , GTKMM_MICRO_VERSION);
    g_print(" - libcairomm    %i.%i.%i\n", CAIROMM_MAJOR_VERSION      , CAIROMM_MINOR_VERSION      , CAIROMM_MICRO_VERSION);
    g_print(" - libpango      %i.%i.%i\n", PANGO_VERSION_MAJOR        , PANGO_VERSION_MINOR        , PANGO_VERSION_MICRO);
    g_print(" - librsvg       %i.%i.%i\n", LIBRSVG_MAJOR_VERSION      , LIBRSVG_MINOR_VERSION      , LIBRSVG_MICRO_VERSION);
    g_print(" - libxml        %s\n",       LIBXML_DOTTED_VERSION);
#ifndef CAIRO_HAS_IMAGE_SURFACE
    g_print("\nNOTE: libcairomm comes WITHOUT image surface support!\n");
#endif
}

void print_directory_info()
{
    g_print(" - DATADIR    = %s\n", scorepress_gtk_config.datadir);
    g_print(" - ICONDIR    = %s\n", scorepress_gtk_config.icondir);
    g_print(" - APPICONDIR = %s\n", scorepress_gtk_config.appicondir);
    g_print(" - DESTDIR    = %s\n", scorepress_gtk_config.destdir);
    g_print(" - PREFIX     = %s\n", scorepress_gtk_config.prefix);
}

