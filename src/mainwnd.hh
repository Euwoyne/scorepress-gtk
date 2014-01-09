
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

#ifndef MAINWND_HH
#define MAINWND_HH

#define GTK_DISABLE_DEPRECATED 1
#include <vector>
#include <gtkmm.h>
#include <scorepress/engine.hh>

#include "score_widget.hh"
#include "icon_manager.hh"
#include "rsvg_renderer.hh"

class Controller;
class MainWnd : public Gtk::Window, public ScorePress::Logging
{
 private:
    // view management
    class View          // class representing an open document
    {
     private:
        View(View&);        // private copy constructor
        
     public:
        Controller*          controller;    // document's controller
        ScoreWidget          widget;        // corresponding score-widget
        Gtk::ScrolledWindow  scrollWnd;     // scrolled window container
        
        View(Controller& ctrl);             // constructor
    };
    
    typedef std::map<Controller*, View*> ViewSet;
    
    // zoom slider widget
    class ZoomScale : public Gtk::HScale
    {
     protected:
        virtual Glib::ustring on_format_value(double value);    // value formatter
        
     public:
        ZoomScale();                                            // constructor
    };
    
 private:
    // menu and toolbar manager
    Glib::RefPtr<Gtk::UIManager>   uiManager;   // UI manager
    Glib::RefPtr<Gtk::ActionGroup> actionGrp;   // action groups 
    
    // widgets
    Gtk::Menu*           mainMnu;           // main menu
    Gtk::HandleBox*      mainBarBox;        // toolbar "Main" handle
    Gtk::Toolbar*        mainBar;           // toolbar "Main"
    Gtk::HandleBox*      viewBarBox;        // toolbar "View" handle
    Gtk::Toolbar*        viewBar;           // toolbar "View"
         ZoomScale*      zoomScl;           // "Zoom" slider
    Gtk::ToolItem*       zoomItm;           // toolbar item "Zoom"
    Gtk::VBox*           mainBox;           // main box (Menu/Toolbar/ScoreBox)
    Gtk::HBox*           toolBox;           // toolbar box
    Gtk::HBox*           scoreBox;          // score box (SideMenu/ScoreTabs)
    Gtk::Notebook*       scoreTabs;         // document notebook (open files tabs)
    Gtk::Statusbar*      statusBar;         // status bar
    
    // about dialog (and corresponding data)
    Gtk::AboutDialog           aboutDlg;    // about dialog
    Glib::ustring              licence;     // licence text (<data_path>/EUPL.txt)
    std::vector<Glib::ustring> authors;     // authors list (hardcoded)
    std::vector<Glib::ustring> documenters; // documenters list (hardcoded)
    std::vector<Glib::ustring> artists;     // artists list (hardcoded)
    Glib::RefPtr<Gdk::Pixbuf>  logo;        // logo (<data_path>/logo.png)
    
    // view management
    ViewSet              views;         // view set
    Controller*          controller;    // current controller
    ScorePress::Engine*  engine;        // current engine
    
    // private copy constructor
    MainWnd(MainWnd&);
    
    // setup methods
    void setup_action_groups();     // setup the "actionGrp" instance
    void setup_about_dialog();      // setup the "aboutDlg" instance
    
 private:
    // signal handlers
    void on_switch_tab(Gtk::Widget* page, guint);       // tab switch handler
    void on_close_tab(Gtk::Widget* page, guint);        // tab close handler
    void on_size_changed(Gtk::Allocation& allocation);  // window size change (center score)
    void on_zoom_changed();                             // zoom slider handler
    
    // menu handlers
    void menu_newEmpty();
    void menu_newCustom();
    void menu_open();
    void menu_save();
    void menu_quit();
    
    void menu_undo();
    void menu_redo();
    void menu_cut();
    void menu_copy();
    void menu_paste();
    
    void menu_linebounds();
    void menu_attachbounds();
    void menu_notebounds();
    void menu_eovbounds();
    void menu_allbounds();
    void menu_nobounds();
    void menu_refresh();
    
    void menu_help();
    void menu_about();
    
 public:
    // main interface
    MainWnd();                  // constructor
    ~MainWnd();                 // destructor
    
    void setup(const Icon&);    // setup window and controls
    
    void reengrave();           // reengrave the associated score
    void refresh();             // redraw the score widget
    
    bool get_linebounds();      // return LineboundsMnu status ("Show line boundaries")
    bool get_attachbounds();    // return AttachboundsMnu status ("Show attachment boundaries")
    bool get_notebounds();      // return NoteboundsMnu status ("Show note boundaries")
    bool get_eovbounds();       // return EOVboundsMnu status ("Show end-of-voice indicators")
    
 public:
    // view interface
    void add_view(Controller& ctrl, bool select = false);   // add new tab to show the given controller
    bool refresh_label(Controller& ctrl);                   // refresh tab label
    bool select_view(Controller& ctrl);                     // show tab with given Controller
    bool select_view(int idx);                              // show tab with given index
};


// inline method implementations
#include "controller.hh"

inline void MainWnd::reengrave()         {controller->reengrave();}
inline void MainWnd::refresh()           {views.find(controller)->second->widget.refresh();}

inline void MainWnd::menu_linebounds()   {controller->set_linebounds(get_linebounds());}
inline void MainWnd::menu_attachbounds() {controller->set_attachbounds(get_attachbounds());}
inline void MainWnd::menu_notebounds()   {controller->set_notebounds(get_notebounds());}
inline void MainWnd::menu_eovbounds()    {controller->set_eovbounds(get_eovbounds());}
inline void MainWnd::menu_refresh()      {reengrave(); refresh();}

inline void MainWnd::menu_about()        {aboutDlg.run(); aboutDlg.hide();}

inline bool MainWnd::get_linebounds()    {return static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/LineboundsMnu"))->get_active();}
inline bool MainWnd::get_attachbounds()  {return static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/AttachboundsMnu"))->get_active();}
inline bool MainWnd::get_notebounds()    {return static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/NoteboundsMnu"))->get_active();}
inline bool MainWnd::get_eovbounds()     {return static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/EOVboundsMnu"))->get_active();}

#endif

