
/*
  ScorePress - Music Engraving Software  (scorepress-gtk)
  Copyright (C) 2011 Dominik Lehmann
  
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
#include <gtkmm.h>
#include <cairommconfig.h>
#include <pango/pango-features.h>
#include <librsvg/rsvg.h>
#ifndef LIBRSVG_FEATURES_H
#   include <librsvg/librsvg-features.h>
#endif
#include <libxml/xmlversion.h>

#include "version.hh"

void print_library_info()
{
    std::cout << " - libgtkmm " << GTKMM_MAJOR_VERSION << "." << GTKMM_MINOR_VERSION << "." << GTKMM_MICRO_VERSION << "\n";
    std::cout << " - libcairomm " << CAIROMM_MAJOR_VERSION << "." << CAIROMM_MINOR_VERSION << "." << CAIROMM_MICRO_VERSION << "\n";
    std::cout << " - libpango " << PANGO_VERSION_MAJOR<< "." << PANGO_VERSION_MINOR << "." << PANGO_VERSION_MICRO << "\n";
    std::cout << " - librsvg " << LIBRSVG_MAJOR_VERSION << "." << LIBRSVG_MINOR_VERSION << "." << LIBRSVG_MICRO_VERSION << "\n";
    std::cout << " - libxml " << LIBXML_DOTTED_VERSION << "\n";
#ifndef CAIRO_HAS_IMAGE_SURFACE
    std::cout << "\nNOTE: libcairomm comes WITHOUT image surface support!\n";
#endif
}

