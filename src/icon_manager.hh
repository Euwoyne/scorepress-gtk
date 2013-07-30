
/*
  ScorePress - Music Engraving Software  (scorepress-gtk)
  Copyright (C) 2013 Dominik Lehmann

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
    std::map<std::string, Icon> icons;
    Glib::RefPtr<Gtk::IconTheme> theme;

 public:
    void load(const std::string iconname, const std::string alias) throw(Error);
    void load(const std::string iconname) throw(Error) {load(iconname, iconname);};
    const Icon& get(const std::string alias) const throw(Error);
};

#endif

