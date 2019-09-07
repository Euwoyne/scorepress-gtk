
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

#ifndef MAINWND_HH
#define MAINWND_HH

#define GTK_DISABLE_DEPRECATED 1
#include <gtkmm.h>
#include "key_map.hh"
#include "icon_manager.hh"
#include "about_dialog.hh"
#include <scorepress/edit_cursor.hh>
#include <scorepress/log.hh>

class View;
class MainWnd : public Gtk::Window, public ScorePress::Logging
{
 private:
    friend class ValueButton;
    friend class ValueSelector;
    
    // type to map view widgets to their interface instances
    typedef std::map<Gtk::Widget*, View*> ViewSet;
     
    // zoom slider widget
    class ZoomScale : public Gtk::HScale
    {
     protected:
        virtual Glib::ustring on_format_value(double value); // value formatter
        
     public:
        ZoomScale(); // constructor
    };
    
    // note value button
    class ValueButton : public Gtk::Button
    {
     private:
        const KeyMap::ActionKey key;
        Gtk::Image              icon;
        
     public:
        ValueButton(KeyMap::ActionKey);
        
        KeyMap::ActionKey get_key() const;
        Gtk::Image&       get_icon();
    };
    
    // note value selector
    class ValueSelector : public Gtk::Popover
    {
     private:
        ViewSet::iterator& view;
        Gtk::Grid          grid;
        ValueButton        valueButtons[11];
        Gtk::Image         icon;
        
     public:
        ValueSelector(Gtk::ToggleToolButton&, ViewSet::iterator&);
        void set_icon(int exp);
        void on_selected(const ValueButton&);
    };
    
 private:
    // menu and toolbar manager
    Glib::RefPtr<Gtk::UIManager>   uiManager;   // UI manager
    Glib::RefPtr<Gtk::ActionGroup> actionGrp;   // action groups
    
    // widgets
    Gtk::Menu*             mainMnu;     // main menu
    Gtk::Toolbar*          mainBar;     // toolbar "Main"
    Gtk::Toolbar*          viewBar;     // toolbar "View"
    Gtk::Toolbar*          noteBar;     // toolbar "Note Properties"
    
    Gtk::ToggleToolButton* valueBtn;    // value button
         ValueSelector*    valueSelect;
    
         ZoomScale*        zoomScl;     // "Zoom" slider
    Gtk::ToolItem*         zoomItm;     // toolbar item "Zoom"
    Gtk::VBox*             mainBox;     // main box (Menu/Toolbar/ScoreBox)
    Gtk::HBox*             toolBox;     // toolbar box
    Gtk::HBox*             scoreBox;    // score box (SideMenu/ScoreTabs)
    Gtk::Notebook*         scoreTabs;   // document notebook (open files tabs)
    Gtk::Statusbar*        statusBar;   // status bar
         AboutDialog*      aboutDlg;    // about dialog
    
    // view management
    ViewSet                views;       // view set
    ViewSet::iterator      view;        // current view
    
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
    
    // toolbar handlers
    void tool_valueselect();
    
 public:
    // main interface
    MainWnd();                  // constructor
    ~MainWnd();                 // destructor
    
    void setup(const Icon&, AboutDialog&);  // setup window and controls
    
 public:
    // view interface
    void   register_view(View& view, bool select); // add new tab
    void   close_view(View& view);                 // close tab
    bool   update_title(View& view);               // update title and tab label
    bool   select_view(View& view);                // show tab with given View
    bool   select_view(int idx);                   // show tab with given index
    size_t get_view_count();                       // return count of views
    
    // menu interface
    void update_menus();        // update state-dependant menus
    
    bool get_linebounds();      // "Show line boundaries" menu status
    bool get_attachbounds();    // "Show attachment boundaries" menu status
    bool get_notebounds();      // "Show note boundaries" menu status
    bool get_eovbounds();       // "Show end-of-voice indicators" menu status
    
    // status interface
    void select_value(unsigned char exp);
    void select_note (ScorePress::EditCursor::NoteName name);
};

inline KeyMap::ActionKey MainWnd::ValueButton::get_key() const {
    return key;
}

inline Gtk::Image& MainWnd::ValueButton::get_icon() {
    return icon;
}

inline size_t MainWnd::get_view_count() {
    return views.size();
}

inline bool   MainWnd::get_linebounds()   {
    return static_cast<Gtk::CheckMenuItem*>(
            uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/LineboundsMnu")
        )->get_active();
}
inline bool   MainWnd::get_attachbounds() {
    return static_cast<Gtk::CheckMenuItem*>(
            uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/AttachboundsMnu")
        )->get_active();
}
inline bool   MainWnd::get_notebounds()   {
    return static_cast<Gtk::CheckMenuItem*>(
            uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/NoteboundsMnu")
        )->get_active();
}
inline bool   MainWnd::get_eovbounds()    {
    return static_cast<Gtk::CheckMenuItem*>(
            uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/EOVboundsMnu")
        )->get_active();
}

#endif
