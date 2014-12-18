
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

#ifndef MAINWND_HH
#define MAINWND_HH

#define GTK_DISABLE_DEPRECATED 1
#include <gtkmm.h>
#include "icon_manager.hh"
#include "about_dialog.hh"
#include <scorepress/log.hh>

class View;
class MainWnd : public Gtk::Window, public ScorePress::Logging
{
 private:
    // zoom slider widget
    class ZoomScale : public Gtk::HScale
    {
     protected:
        virtual Glib::ustring on_format_value(double value);    // value formatter
        
     public:
        ZoomScale();                                            // constructor
    };
    
 private:
    typedef std::map<Gtk::Widget*, View*> ViewSet;
    
    // menu and toolbar manager
    Glib::RefPtr<Gtk::UIManager>   uiManager;   // UI manager
    Glib::RefPtr<Gtk::ActionGroup> actionGrp;   // action groups 
    
    // widgets
    Gtk::Menu*           mainMnu;           // main menu
    //Gtk::HandleBox*      mainBarBox;        // toolbar "Main" handle
    //Gtk::Toolbar*        mainBar;           // toolbar "Main"
    //Gtk::HandleBox*      viewBarBox;        // toolbar "View" handle
    //Gtk::Toolbar*        viewBar;           // toolbar "View"
    
    Gtk::Toolbar*        noteBar;           // toolbar "Note Properties"
    Gtk::MenuButton*     noteValueBtn;      // note value button
    
         ZoomScale*      zoomScl;           // "Zoom" slider
    Gtk::ToolItem*       zoomItm;           // toolbar item "Zoom"
    Gtk::VBox*           mainBox;           // main box (Menu/Toolbar/ScoreBox)
    Gtk::HBox*           toolBox;           // toolbar box
    Gtk::HBox*           scoreBox;          // score box (SideMenu/ScoreTabs)
    Gtk::Notebook*       scoreTabs;         // document notebook (open files tabs)
    Gtk::Statusbar*      statusBar;         // status bar
         AboutDialog*    aboutDlg;          // about dialog
    
    // view management
    ViewSet              views;         // view set
    ViewSet::iterator    view;          // current view
    
    // private copy constructor
    MainWnd(MainWnd&);
    
    // setup methods
    void setup_action_groups();         // setup the "actionGrp" instance
    
 private:
    // signal handlers
    void on_switch_tab(Gtk::Widget* page, guint);   // tab switch handler
    void on_close_tab(Gtk::Widget* page, guint);    // tab close handler
    void on_zoom_changed();                         // zoom slider handler
    
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
    
    void menu_attachbounds();
    void menu_notebounds();
    void menu_linebounds();
    void menu_eovbounds();
    void menu_allbounds();
    void menu_nobounds();
    
    void menu_help();
    void menu_about();
    
 public:
    // main interface
    MainWnd();                  // constructor
    ~MainWnd();                 // destructor
    
    void setup(const Icon&, AboutDialog&);  // setup window and controls
    
 public:
    // view interface
    void register_view(View& view, bool select);    // add new tab to show the given view
    void close_view(View& view);                    // close tab
    bool update_title(View& view);                  // update title and tab label
    bool select_view(View& view);                   // show tab with given View
    bool select_view(int idx);                      // show tab with given index
    int  get_view_count();                          // return count of views
    
    // menu interface
    void update_menus();        // update state-dependant menus
    
    bool get_linebounds();      // return LineboundsMnu status   ("Show line boundaries")
    bool get_attachbounds();    // return AttachboundsMnu status ("Show attachment boundaries")
    bool get_notebounds();      // return NoteboundsMnu status   ("Show note boundaries")
    bool get_eovbounds();       // return EOVboundsMnu status    ("Show end-of-voice indicators")
};

inline int  MainWnd::get_view_count()   {return views.size();}

inline bool MainWnd::get_linebounds()   {return static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/LineboundsMnu"))->get_active();}
inline bool MainWnd::get_attachbounds() {return static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/AttachboundsMnu"))->get_active();}
inline bool MainWnd::get_notebounds()   {return static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/NoteboundsMnu"))->get_active();}
inline bool MainWnd::get_eovbounds()    {return static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/EOVboundsMnu"))->get_active();}

#endif

