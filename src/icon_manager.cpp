
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

#include <gtkmm/icontheme.h>

#include "icon_manager.hh"
#include "i18n.hh"
#include "config.hh"

#include <iostream>

void IconManager::load(const std::string iconname, const std::string alias) throw(Error)
{
    if (!theme)
    {
        theme = Gtk::IconTheme::get_default();
        theme->append_search_path(scorepress_icondir);
        theme->append_search_path(scorepress_appicondir);
        theme->rescan_if_needed();
    };
    
    if (!theme->has_icon(iconname))
    {
        throw Error(_("Unable to load icon \"" + iconname + "\"."));
    };
    
    Icon& icon = icons[alias] = Icon();
    std::vector<int> sizes = theme->get_icon_sizes(iconname);
    for (std::vector<int>::const_iterator i = sizes.begin(); i != sizes.end(); ++i)
    {
        try
        {
            icon.push_back(theme->load_icon(iconname, *i));
        }
        catch(Gtk::IconThemeError& e)
        {
            throw IconManager::Error(e.what());
        };
    };

}

const Icon& IconManager::get(const std::string alias) const throw(Error)
{
    std::map<std::string, Icon>::const_iterator i = icons.find(alias);
    if (i == icons.end()) throw Error(_("Icon \"" + alias + "\" has not been loaded."));
    return i->second;
}
