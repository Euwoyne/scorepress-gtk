
/*
  ScorePress - Music Engraving Software  (scorepress-gtk)
  Copyright (C) 2016 Dominik Lehmann
  
  Licensed under the EUPL, Version 1.1 or - as soon they
  will be approved by the European Commission - subsequent
  versions of the EUPL (the "Licence");
  You may not use this work except in compliance with the
  Licence. You may obtain a copy of the Licence at
  <http://ec.europa.eu/idabc/eupl/>.
  
  Unless required by applicable law or agreed to in
  writing, software distributed under the Licence is
  distributed on an "AS IS" basis, WITHOUT WARRANTIES OR
  CONDITIONS OF ANY KIND, either expressed or implied.
  See the Licence for the specific language governing
  permissions and limitations under the Licence.
*/

#ifndef ICON_MANAGER_HH
#define ICON_MANAGER_HH

#define GTK_DISABLE_DEPRECATED 1
#include <string>
#include <vector>
#include <map>
#include <gdkmm/pixbuf.h>

typedef std::vector< Glib::RefPtr<Gdk::Pixbuf> > Icon;

class IconManager
{
 public:
    class Error : public std::string {public: Error(const std::string& msg) : std::string(msg) {};};
    
 private:
    std::map<std::string, Icon>  icons;     // icon cache
    Glib::RefPtr<Gtk::IconTheme> theme;     // used icon theme (i.e. default theme)
    
 public:
    void        load(const std::string icon, const std::string alias); // load icon into memory
    void        load(const std::string icon);                          // load icon into memory
    const Icon& get (const std::string alias) const;                   // get icon (throws, if not loaded)
};

// inline method implementation
inline void IconManager::load(const std::string iconname) {load(iconname, iconname);}

#endif

