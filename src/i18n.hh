
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

#ifndef I18N_HH
#define I18N_HH

#ifdef I18N
#   include <libintl.h>
#   define _(String) gettext(String)
#   define gettext_noop(String) String
#   define N_(String) gettext_noop(String)
#else
#   define _(String) (String)
#   define N_(String) String
#   define textdomain(Domain)
#   define bindtextdomain(Package, Directory)
#endif

#endif

