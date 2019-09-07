
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

#include <gtkmm/icontheme.h>

#include "icon_manager.hh"
#include "i18n.hh"
#include "config.hh"    // icondir, appicondir

// throwing function (general error)
static void mythrow(const char* trns, const std::string& arg)
{
    char* msg = new char[strlen(trns) + arg.size() + 1];    // allocate memory
    sprintf(msg, trns, arg.c_str());                        // assemble message
    std::string s(msg);                                     // convert to string
    delete[] msg;                                           // delete buffer
    throw IconManager::Error(s);                            // throw message
}

// load icon into memory
void IconManager::load(const std::string iconname, const std::string alias)
{
    // prepare search paths
    if (!theme)
    {
        theme = Gtk::IconTheme::get_default();                          // use theme for default screen
        theme->append_search_path(scorepress_gtk_config.icondir);       // add icon directory
        theme->rescan_if_needed();                                      // scan new directories
    };
    
    // check for icon existence
    if (!theme->has_icon(iconname))
    {
        mythrow(_("Unable to load icon \"%s\"."), iconname);
    };
    
    // load all icon sizes into memory
    Icon& icon = icons[alias] = Icon();
    std::vector<int> sizes = theme->get_icon_sizes(iconname);
    for (std::vector<int>::const_iterator i = sizes.begin(); i != sizes.end(); ++i)
    {
        try // for each available size
        {   //     load icon into memory
            icon.push_back(theme->load_icon(iconname, *i));
        }
        catch(Gtk::IconThemeError& e)
        {
            throw Error(e.what());
        };
    };

}

// get icon (throws, if not loaded)
const Icon& IconManager::get(const std::string alias) const
{
    std::map<std::string, Icon>::const_iterator i = icons.find(alias);
    if (i == icons.end()) mythrow(_("Icon \"%s\" has not been loaded."), alias);
    return i->second;
}

