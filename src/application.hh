
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

#ifndef APPLICATION_HH
#define APPLICATION_HH

#define GTK_DISABLE_DEPRECATED 1
#include <gtkmm.h>
#include <scorepress/log.hh>
#include "controller.hh"
#include "key_listener.hh"
#include "icon_manager.hh"
#include "about_dialog.hh"

class Application : public Gtk::Application
{
 private:
    // commandline options
    struct CmdlineOptions
    {
        // print/log flags
        struct Flags
        {
            bool silent;        // no output at all
            bool debug;         // emit debug messages
            bool verbose;       // emit verbose messages
            Flags() : silent(false), debug(false), verbose(false) {};
        };
        
        // log file flags
        struct Log : public Flags
        {
            std::string file;   // log file path
        };
        
        Log   log;              // log flags
        Flags stdout;           // print flags
        
        std::vector<std::string> files; // files to open
    };
    
    // global state
    std::list<Controller*> controllers;     // controllers (one for each open document; owned by the Application)
    KeyListener            key_listener;    // key listener (globally the same)
    IconManager            icon_manager;    // icon manager
    AboutDialog            about_dialog;    // about dialog
    ScorePress::Log        log;             // application log
    
 private:
    std::string get_next_unnamed() const;   // return "Unsaved Score #"
    
 public:
    Application();      // default constructor
    ~Application();     // default destructor
    void add_window();  // add new window to the application
    
 protected:
    virtual void on_startup();
    virtual int  on_command_line(const Glib::RefPtr<Gio::ApplicationCommandLine>& command_line);
    virtual void on_open(const std::vector< Glib::RefPtr<Gio::File> >& files, const Glib::ustring& hint);
    virtual void on_window_hide(Gtk::Window* window);
};

#endif

