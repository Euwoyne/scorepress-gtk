
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

#ifndef ABOUT_DIALOG_HH
#define ABOUT_DIALOG_HH

#include <gtkmm/aboutdialog.h>

class AboutDialog : public Gtk::AboutDialog
{
 private:
    Glib::ustring              licence;     // licence text (<data_path>/EUPL.txt)
    Glib::RefPtr<Gdk::Pixbuf>  logo;        // logo         (<data_path>/logo.png)
    
 public:
    std::vector<Glib::ustring> authors;     // authors list
    std::vector<Glib::ustring> documenters; // documenters list
    std::vector<Glib::ustring> artists;     // artists list
    
 public:
    void setup();                           // content setup
    int  run(Gtk::Window& parent);          // show dialog
};

#endif
