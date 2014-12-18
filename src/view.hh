
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

#ifndef VIEW_HH
#define VIEW_HH

#define GTK_DISABLE_DEPRECATED 1
#include <gtkmm/scrolledwindow.h>   // Gtk::ScrolledWindow
#include "score_widget.hh"          // ScoreWidget
#include "mainwnd.hh"               // MainWnd

class Controller;
class MainWnd;

class View
{
 public:
    Controller&         controller;         // document's controller
    
 private:
    MainWnd*            window;             // parent window
    Gtk::ScrolledWindow scrollWnd;          // scrolled window container
    ScoreWidget         widget;             // displaying score-widget
    
 public:
    // constructor/destructor
    View(Controller&, MainWnd&);            // constructor
    View(const View&);                      // copy constructor
    ~View();                                // destructor
    
    // setup and access
    void     set_window(MainWnd&);          // move to different window
    MainWnd& get_window();                  // get current window
    
          Gtk::Widget* get_widget();        // get widget
    const Gtk::Widget* get_widget() const;  // get widget (constant version)
    
    operator const Gtk::Widget&() const;    // cast to widget
    operator       Gtk::Widget&();          // cast to widget (constant version)
    
    // signals from control
    void on_resize();                       // document size changed  (reallocate and redraw cache)
    void rerender();                        // document data changed  (redraw cache)
    void redraw();                          // view changed           (refresh widget)
    
    void on_title_changed();                // title and/or filename changed (refresh title and tab label)
    void on_status_changed();               // status bar content changed    (refresh status bar)
    
    // signals from view
    void on_size_changed(Gtk::Allocation&); // resize signal handler (like a score-resize)
    void grab_focus();
};

// inline methods
inline void     View::set_window(MainWnd& wnd) {window = &wnd;}
inline MainWnd& View::get_window()             {return *window;}

inline       Gtk::Widget* View::get_widget()       {return &scrollWnd;}
inline const Gtk::Widget* View::get_widget() const {return &scrollWnd;}

inline View::operator const Gtk::Widget&() const {return scrollWnd;}
inline View::operator       Gtk::Widget&()       {return scrollWnd;}

inline void View::on_title_changed() {window->update_title(*this);}
inline void View::grab_focus()       {widget.grab_focus();}

#endif

