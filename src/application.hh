
/*
  ScorePress - Music Engraving Software  (scorepress-gtk)
  Copyright (C) 2014 Dominik Lehmann
  
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

#ifndef APPLICATION_HH
#define APPLICATION_HH

#include <gtkmm.h>
#include "icon_manager.hh"
#include "key_listener.hh"
#include "mainwnd.hh"

class Controller;
class ScorePressApp : public Gtk::Application
{
 private:
    IconManager              icon_manager;
    KeyListener              key_listener;
    std::vector<Controller*> controllers;
    ScorePress::Log          log;
    
 private:
    std::string get_next_unnamed() const;
    
 protected:
    virtual void on_activate();
    virtual int  on_command_line(const Glib::RefPtr<Gio::ApplicationCommandLine>& command_line);
    virtual void on_open(const std::vector< Glib::RefPtr<Gio::File> >& files, const Glib::ustring& hint);
    virtual void on_window_hide(Gtk::Window* window);
    
 public:
    ScorePressApp();
    virtual ~ScorePressApp();
    
    bool add_tab(bool select = false);
    void add_tab(MainWnd& wnd, bool select = false);
    void add_window();
};

#endif

