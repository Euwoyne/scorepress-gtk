
/*
  ScorePress - Music Engraving Software  (scorepress-gtk)
  Copyright (C) 2014 Dominik Lehmann
  
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

#include "about_dialog.hh"
#include "config.hh"
#include "i18n.hh"

// about dialog content setup (authors, documenters and artists should be filled before)
void AboutDialog::setup()
{
    ///  About Dialog
    /// ==============
    ///
    /// program description
    static const Glib::ustring comment     = _("ScorePress is a WYSIWYG music typesetting program.");
    /// GitHub link
    static const Glib::ustring github_link = _("GitHub Repository");
    /// your name (as displayed in the about dialog)
    static const Glib::ustring translator  = _("[name of translator]");
    
    if (this->licence.empty())
    {
        Glib::RefPtr<Glib::IOChannel> eupl = Glib::IOChannel::create_from_file(std::string(scorepress_gtk_config.datadir) += "/EUPL.txt", "r");
        eupl->read_to_end(this->licence);
        this->logo = Gdk::Pixbuf::create_from_file(std::string(scorepress_gtk_config.datadir) += "/logo.png");
    };
    
    this->set_program_name(SCOREPRESS_TITLE);
    this->set_version(SCOREPRESS_VERSION_STRING);
    this->set_copyright(SCOREPRESS_COPYRIGHT_UTF);
    this->set_comments(comment);
    this->set_license(licence);
    this->set_license_type(Gtk::LICENSE_CUSTOM);
    this->set_website("https://github.com/Euwoyne/scorepress-gtk");
    this->set_website_label(github_link);
    this->set_authors(authors);
    if (!documenters.empty())
        this->set_documenters(documenters);
    this->set_artists(artists);
    if (translator != "[name of translator]")
        this->set_translator_credits(translator);
    this->set_logo(logo);
    this->set_wrap_license(true);
}

