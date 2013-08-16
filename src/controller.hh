
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

#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <string>
#include <vector>
#include <scorepress/engine.hh>
#include <giomm/file.h>
#include "rsvg_renderer.hh"
#include "key_listener.hh"

class MainWnd;
class Controller : public ScorePress::Logging
{
 private:
    RSVGRenderer*       renderer;
    ScorePress::Engine* engine;
    KeyListener*        keylistener;
    MainWnd*            window;
    
    std::string  filename;
    std::string  filepath;
    bool         changed;
    
    void setup_engine();
    
    Controller(Controller&);
    
 public:
    // constructors
    Controller(MainWnd& window, KeyListener& keylistener);
    Controller(MainWnd& window, KeyListener& keylistener, const std::string& sprite_file);
    ~Controller();
    
    // data access
    RSVGRenderer&       get_renderer();
    ScorePress::Engine& get_engine();
    MainWnd&            get_window();
    const std::string&  get_filename() const;
    const std::string&  get_filepath() const;
    void                set_filename(const std::string& s);
    void                set_filepath(const std::string& s);
    bool                is_new() const;
    bool                is_changed() const;
    void                change(bool b = true);
    
    // open file
    bool open(const Glib::RefPtr<Gio::File>& file);
    
    // action listeners
    void mouse_on(double x, double y);
    void mouse_off(double x, double y);
    void key_press(const KeyMap::Key key);
    void key_release(const KeyMap::Key key);
    
    // view (i.e. press parameters)
    void set_scale(unsigned int scale);
    void set_linebounds(bool value);
    void set_attachbounds(bool value);
    void set_notebounds(bool value);
    void set_eovbounds(bool value);
    
    // logging control
    void log_set(ScorePress::Log& log);
    void log_unset();
};

inline RSVGRenderer&       Controller::get_renderer()         {return *renderer;}
inline ScorePress::Engine& Controller::get_engine()           {return *engine;}
inline MainWnd&            Controller::get_window()           {return *window;}

inline const std::string&  Controller::get_filename() const               {return filename;}
inline const std::string&  Controller::get_filepath() const               {return filepath;}
inline void                Controller::set_filepath(const std::string& s) {filepath = s;}
inline bool                Controller::is_new() const                     {return filepath.empty();}
inline bool                Controller::is_changed() const                 {return changed;}
inline void                Controller::change(bool b)                     {changed = b;}

inline void Controller::key_press(const KeyMap::Key key) {keylistener->press(key, engine->cursor);}
inline void Controller::key_release(const KeyMap::Key key) {keylistener->release(key);}

#endif

