
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

#include "mainwnd.hh"
#include "../mainwnd-ui.xml.hh"
#include "controller.hh"
#include "view.hh"
#include "config.hh"
#include "i18n.hh"
#include <vector>
#include <iostream>
#include <scorepress/engine.hh>

#define CATCH_ERRORS(NAME) \
    catch (ScorePress::Engine::Error& s) { \
        std::cerr << "[ERROR] MainWnd::" NAME "(): " << s << " (Engine Error)\n"; \
    } \
    catch (ScorePress::UserCursor::Error& s) { \
        std::cerr << "[ERROR] MainWnd::" NAME "(): " << s << " (User Cursor Error)\n"; \
    } \
    catch (ScorePress::Cursor::Error& s) { \
        std::cerr << "[ERROR] MainWnd::" NAME "(): " << s << " (Cursor Error)\n"; \
    } \
    catch (ScorePress::FileReader::Error& s) { \
        std::cerr << "[ERROR] MainWnd::" NAME "(): " << s << " (File Reader Error)\n"; \
    } \
    catch (ScorePress::Press::Error& s) { \
        std::cerr << "[ERROR] MainWnd::" NAME "(): " << s << " (Press Error)\n"; \
    } \
    catch (ScorePress::StaffContext::Error& s) { \
        std::cerr << "[ERROR] MainWnd::" NAME "(): " << s << " (Context Error)\n"; \
    } \
    catch (ScorePress::Error& s) { \
        std::cerr << "[ERROR] MainWnd::" NAME "(): " << s << "\n"; \
    } \
    catch (std::exception& e) { \
        std::cerr << "[ERROR] MainWnd::" NAME "(): " << e.what() << "\n"; \
    } \
    catch (...) { \
        std::cerr << "[ERROR] MainWnd::" NAME "(): Unknown Exception\n"; \
    }

// zoom slider widget: value formatter
Glib::ustring MainWnd::ZoomScale::on_format_value(double value)
{
    try
    {
        return Glib::ustring::compose("%1%% ", 5.0 * value);
    }
    catch (std::exception& e)
    {
        std::cerr << "std::exception caught: " << e.what() << "\n";
        return "ERR%";
    };
}

// zoom slider widget: constructor
MainWnd::ZoomScale::ZoomScale() : Gtk::HScale(2, 61, 1)
{
    this->set_value_pos(Gtk::POS_RIGHT);
    this->set_has_origin(false);
    this->set_slider_size_fixed(true);
    this->set_digits(0);
    this->set_increments(1, 2);
}

// setup the "actionGrp" instance (requires properly set up menus)
inline void MainWnd::setup_action_groups()
{
    actionGrp = Gtk::ActionGroup::create();
    
    ///  Window menus in main-window
    /// =============================
    actionGrp->add(Gtk::Action::create("FileMnu", _("_File")));
    actionGrp->add(Gtk::Action::create("NewMnu", _("_New..."), _("Create new score")));
    actionGrp->add(Gtk::Action::create("NewEmptyMnu", Gtk::Stock::NEW, _("_Empty Score"), _("Create empty score")),
                   sigc::mem_fun(*this, &MainWnd::menu_newEmpty));
    actionGrp->add(Gtk::Action::create("NewCustomMnu", _("_Customized..."), _("Create customized empty score")),
                   sigc::mem_fun(*this, &MainWnd::menu_newCustom));
    actionGrp->add(Gtk::Action::create("OpenMnu", Gtk::Stock::OPEN, _("_Open..."), _("Open score file")),
                   Gtk::AccelKey("<control>o"),
                   sigc::mem_fun(*this, &MainWnd::menu_open));
    actionGrp->add(Gtk::Action::create("SaveMnu", Gtk::Stock::SAVE, _("_Save..."), _("Save current score")),
                   Gtk::AccelKey("<control>s"),
                   sigc::mem_fun(*this, &MainWnd::menu_save));
    actionGrp->add(Gtk::Action::create("QuitMnu", Gtk::Stock::QUIT, _("_Quit"), _("Quit program")),
                   Gtk::AccelKey("<control>q"),
                   sigc::mem_fun(*this, &MainWnd::menu_quit));
    actionGrp->add(Gtk::Action::create("EditMnu", _("_Edit")));
    actionGrp->add(Gtk::Action::create("UndoMnu", Gtk::Stock::UNDO, _("_Undo"), _("Undo last action")),
                   Gtk::AccelKey("<control>z"),
                   sigc::mem_fun(*this, &MainWnd::menu_undo));
    actionGrp->add(Gtk::Action::create("RedoMnu", Gtk::Stock::REDO, _("_Redo"), _("Redo the last undone action")),
                   Gtk::AccelKey("<shift><control>z"),
                   sigc::mem_fun(*this, &MainWnd::menu_redo));
    actionGrp->add(Gtk::Action::create("CutMnu", Gtk::Stock::CUT, _("Cu_t"), _("Cut highlighted part")),
                   Gtk::AccelKey("<control>x"),
                   sigc::mem_fun(*this, &MainWnd::menu_cut));
    actionGrp->add(Gtk::Action::create("CopyMnu", Gtk::Stock::COPY, _("_Copy"), _("Copy highlighted part")),
                   Gtk::AccelKey("<control>c"),
                   sigc::mem_fun(*this, &MainWnd::menu_copy));
    actionGrp->add(Gtk::Action::create("PasteMnu", Gtk::Stock::PASTE, _("_Paste"), _("Paste clipboard contents")),
                   Gtk::AccelKey("<control>v"),
                   sigc::mem_fun(*this, &MainWnd::menu_paste));
    actionGrp->add(Gtk::Action::create("ViewMnu", _("_View")));
    actionGrp->add(Gtk::Action::create("BoundsMnu", _("_Boundaries")));
    actionGrp->add(Gtk::ToggleAction::create("LineboundsMnu", _("_Line"), _("Show line boundaries")),
                   sigc::mem_fun(*this, &MainWnd::menu_linebounds));
    actionGrp->add(Gtk::ToggleAction::create("AttachboundsMnu", _("_Attached"), _("Show attachment boundaries")),
                   sigc::mem_fun(*this, &MainWnd::menu_attachbounds));
    actionGrp->add(Gtk::ToggleAction::create("NoteboundsMnu", _("_Note"), _("Show note boundaries")),
                   sigc::mem_fun(*this, &MainWnd::menu_notebounds));
    actionGrp->add(Gtk::ToggleAction::create("EOVboundsMnu", _("_EOV"), _("Show end-of-voice indicators")),
                   sigc::mem_fun(*this, &MainWnd::menu_eovbounds));
    actionGrp->add(Gtk::Action::create("AllboundsMnu", _("_Show All"), _("Show all boundaries")),
                   sigc::mem_fun(*this, &MainWnd::menu_allbounds));
    actionGrp->add(Gtk::Action::create("NoboundsMnu", _("_Hide All"), _("Show no boundaries")),
                   sigc::mem_fun(*this, &MainWnd::menu_nobounds));
    actionGrp->add(Gtk::Action::create("HelpMnu", _("_Help")));
    actionGrp->add(Gtk::Action::create("HelpIdxMnu", Gtk::Stock::HELP, _("_Help..."), _("Help Index")),
                   Gtk::AccelKey("F1"),
                   sigc::mem_fun(*this, &MainWnd::menu_help));
    actionGrp->add(Gtk::Action::create("AboutMnu", Gtk::Stock::ABOUT, _("_About..."), _("Copyright and Credits")),
                   sigc::mem_fun(*this, &MainWnd::menu_about));
}

// tab switch
void MainWnd::on_switch_tab(Gtk::Widget* page, guint)
{
    try
    {
        // get corresponding view
        ViewSet::iterator view_it = views.find(page);
        if (view_it == views.end())
            throw ScorePress::Error("Unable to find requested Gtk::Notebook page.");
        view = view_it;
        
        // update window
        zoomScl->set_value(view->second->controller.get_engine().get_press_parameters().scale / 50.0);
        this->set_title(Glib::ustring(view->second->controller.get_document().meta.title) += " - ScorePress");
        update_menus();
        
        // set focus
        view->second->grab_focus();
    }
    CATCH_ERRORS("on_switch_tab");
}

// tab close
void MainWnd::on_close_tab(Gtk::Widget* page, guint)
{
    try
    {
        // get corresponding view
        ViewSet::iterator view_it = this->views.find(page);
        
        // TODO: ask for save
        
        // delete the view
        if (view_it == views.end())
            throw ScorePress::Error("Unable to find requested Gtk::Notebook page.");
        else
            views.erase(view_it);
    }
    CATCH_ERRORS("on_close_tab");
}

// signal handlers
void MainWnd::on_zoom_changed()
{
    if (view != views.end())
        view->second->controller.on_zoom_changed(static_cast<unsigned int>(50.0 * zoomScl->get_value()));
}

void MainWnd::menu_newEmpty()     {std::cout << "NEW (empty)\n";}
void MainWnd::menu_newCustom()    {std::cout << "NEW (wizard)\n";}
void MainWnd::menu_open()         {try {std::cout << "Open!\n"; view->second->controller.open(Glib::RefPtr<Gio::File>());} CATCH_ERRORS("menu_open");}
void MainWnd::menu_save()         {std::cout << "Save\n";}
void MainWnd::menu_quit()         {std::cout << "QUIT!\n"; exit(0);}

void MainWnd::menu_undo()         {std::cout << "Undo\n";}
void MainWnd::menu_redo()         {std::cout << "Redo\n";}
void MainWnd::menu_cut()          {std::cout << "CUT\n";}
void MainWnd::menu_copy()         {std::cout << "COPY\n";}
void MainWnd::menu_paste()        {std::cout << "PASTE\n";}

void MainWnd::menu_attachbounds() {view->second->controller.get_engine().get_press_parameters().draw_attachbounds = get_attachbounds();
                                   view->second->controller.rerender();}
void MainWnd::menu_notebounds()   {view->second->controller.get_engine().get_press_parameters().draw_notebounds   = get_notebounds();
                                   view->second->controller.rerender();}
void MainWnd::menu_linebounds()   {view->second->controller.get_engine().get_press_parameters().draw_linebounds   = get_linebounds();
                                   view->second->controller.rerender();}
void MainWnd::menu_eovbounds()    {view->second->controller.get_engine().get_press_parameters().draw_eov          = get_eovbounds();
                                   view->second->controller.rerender();}

void MainWnd::menu_allbounds()
{
    static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/LineboundsMnu"))->set_active();
    static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/AttachboundsMnu"))->set_active();
    static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/NoteboundsMnu"))->set_active();
    static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/EOVboundsMnu"))->set_active();
}

void MainWnd::menu_nobounds()
{
    static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/LineboundsMnu"))->set_active(false);
    static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/AttachboundsMnu"))->set_active(false);
    static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/NoteboundsMnu"))->set_active(false);
    static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/EOVboundsMnu"))->set_active(false);
}

void MainWnd::menu_help()         {std::cout << "Help (?)\n";}
void MainWnd::menu_about()        {aboutDlg->run(*this); aboutDlg->hide();}

// constructor
MainWnd::MainWnd() : view(views.end())
{
    //mainBarBox = NULL;
    //viewBarBox = NULL;
    //toolBox = NULL;
    noteValueBtn = NULL;
    zoomScl = NULL;
    zoomItm = NULL;
    mainBox = NULL;
    scoreBox = NULL;
    scoreTabs = NULL;
    statusBar = NULL;
}

// setup window and controls
void MainWnd::setup(const Icon& icon, AboutDialog& aboutdlg)
{
    // setup window
    log_debug("setup window...");
    this->set_title(SCOREPRESS_TITLE);
    this->set_default_size(750, 530);
    this->set_icon_list(icon);
    
    //setup_about_dialog();
    aboutDlg = &aboutdlg;
    
    // setup menus
    log_debug("setup menus...");
    setup_action_groups();
    uiManager = Gtk::UIManager::create();
    uiManager->insert_action_group(actionGrp);
    uiManager->add_ui_from_string(mainwnd_ui);
    this->add_accel_group(uiManager->get_accel_group());
    
    noteBar = static_cast<Gtk::Toolbar*>(uiManager->get_widget("/NoteBar"));
    noteValueBtn = new Gtk::MenuButton();
    
    mainMnu = static_cast<Gtk::Menu*>(uiManager->get_widget("/MainMnu"));
    /*
    mainBar = static_cast<Gtk::Toolbar*>(uiManager->get_widget("/MainBar"));
    mainBar->unset_toolbar_style();
    mainBar->set_can_default(false);
    mainBar->set_can_focus(false);
    viewBar = static_cast<Gtk::Toolbar*>(uiManager->get_widget("/ViewBar"));
    viewBar->unset_toolbar_style();
    viewBar->set_can_default(false);
    viewBar->set_can_focus(false);
    */
    
    // setup controls
    log_debug("setup controls...");
    //mainBarBox = new Gtk::HandleBox();
    zoomScl = new ZoomScale();
    zoomItm = new Gtk::ToolItem();
    //viewBarBox = new Gtk::HandleBox();
    mainBox = new Gtk::VBox(false, 0);
    toolBox = new Gtk::HBox(false, 0);
    scoreBox = new Gtk::HBox(false, 0);
    scoreTabs = new Gtk::Notebook();
    statusBar = new Gtk::Statusbar();
    scoreTabs->set_scrollable(true);
    
    //mainBarBox->add(*mainBar);
    //viewBarBox->add(*viewBar);
    toolBox->pack_start(*noteBar, Gtk::PACK_EXPAND_WIDGET);
    //toolBox->pack_start(*mainBarBox, Gtk::PACK_EXPAND_WIDGET);
    //toolBox->pack_start(*viewBarBox, Gtk::PACK_EXPAND_WIDGET);
    scoreTabs->set_can_focus(false);
    scoreBox->pack_start(*scoreTabs);
    zoomScl->set_can_focus(false);
    zoomItm->add(*zoomScl);
    zoomItm->set_expand(false);
    statusBar->pack_end(*zoomItm);
    mainBox->pack_start(*mainMnu, Gtk::PACK_SHRINK);
    mainBox->pack_start(*toolBox, Gtk::PACK_SHRINK);
    mainBox->pack_start(*scoreBox);
    mainBox->pack_end(*statusBar, Gtk::PACK_SHRINK, 0);
    this->add(*mainBox);
    
    // setup signal handlers
    scoreTabs->signal_switch_page().connect(sigc::mem_fun(*this, &MainWnd::on_switch_tab), false);
    scoreTabs->signal_page_removed().connect(sigc::mem_fun(*this, &MainWnd::on_close_tab), false);
    zoomScl->signal_value_changed().connect(sigc::mem_fun(*this, &MainWnd::on_zoom_changed), false);
    
    // prepare controls
    uiManager->get_widget("/MainMnu/EditMnu/UndoMnu" )->set_sensitive(false);
    uiManager->get_widget("/MainBar/UndoMnu"         )->set_sensitive(false);
    uiManager->get_widget("/MainMnu/EditMnu/RedoMnu" )->set_sensitive(false);
    uiManager->get_widget("/MainBar/RedoMnu"         )->set_sensitive(false);
    uiManager->get_widget("/MainMnu/EditMnu/PasteMnu")->set_sensitive(false);
    uiManager->get_widget("/MainBar/PasteMnu"        )->set_sensitive(false);
    
    uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/LineboundsMnu"  )->set_sensitive(false);
    uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/AttachboundsMnu")->set_sensitive(false);
    uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/NoteboundsMnu"  )->set_sensitive(false);
    uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/EOVboundsMnu"   )->set_sensitive(false);
    
    zoomScl->set_sensitive(false);
    zoomScl->set_value(20);
    
    this->show_all();
}

// destructor
MainWnd::~MainWnd()
{
    log_debug("delete window");
    
    delete statusBar;
    delete scoreTabs;
    delete scoreBox;
    delete mainBox;
    delete zoomItm;
    delete zoomScl;
    //delete mainBarBox;
    //delete viewBarBox;
    //delete toolBox;
    delete noteValueBtn;
    for (ViewSet::iterator i = views.begin(); i != views.end(); ++i)
        delete i->second;
}

// add new tab to show the given controller
void MainWnd::register_view(View& new_view, bool select)
{
    // check curent state
    if (views.empty())  // if no tab exists yet
    {                   //      activate menus
        uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/LineboundsMnu")->set_sensitive(true);
        uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/AttachboundsMnu")->set_sensitive(true);
        uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/NoteboundsMnu")->set_sensitive(true);
        uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/EOVboundsMnu")->set_sensitive(true);
        zoomScl->set_sensitive(true);
        select = true;  //      select the added view
    };
    
    // add view
    log_debug("add view...");
    ViewSet::iterator view_it = views.find(new_view.get_widget());
    int idx = -1;
    if (view_it != views.end())
    {
        idx = scoreTabs->page_num(*view_it->second);
    };
    if (idx < 0)
    {
        view_it = views.insert(std::pair<Gtk::Widget*, View*>(new_view.get_widget(), &new_view)).first;
        idx = scoreTabs->append_page(new_view, new_view.controller.get_basename());
    };
    
    // select the new view
    if (select)
    {
        scoreTabs->set_current_page(idx);
    };
}

// close tab
void MainWnd::close_view(View& _view)
{
    scoreTabs->remove(_view);
    if (views.empty())
    {
        zoomScl->set_sensitive(false);
        zoomScl->set_value(20);
    };
}

// update dependant title and tab label
bool MainWnd::update_title(View& _view)
{
    ViewSet::iterator view_it = views.find(_view.get_widget());
    if (view_it == views.end()) return false;
    if (view_it == view)
        this->set_title(Glib::ustring(view->second->controller.get_document().meta.title) += " - ScorePress");
    if (_view.controller.is_changed())
        scoreTabs->set_tab_label_text(_view, Glib::ustring("*").append(_view.controller.get_basename()));
    else
        scoreTabs->set_tab_label_text(_view, _view.controller.get_basename());
    return true;
}

// show tab with given Controller
bool MainWnd::select_view(View& _view)
{
    ViewSet::iterator view_it = views.find(_view.get_widget());
    if (view_it == views.end()) return false;
    return select_view(scoreTabs->page_num(*view_it->second));
}

// show tab with given index
bool MainWnd::select_view(int idx)
{
    if (idx < 0 || idx >= scoreTabs->get_n_pages())
        return false;
    scoreTabs->set_current_page(idx);
    return true;
}

// update state-dependant menus
void MainWnd::update_menus()
{
    const ScorePress::Engine& engine = view->second->controller.get_engine();
    static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/LineboundsMnu"))->set_active(
        engine.get_press_parameters().draw_linebounds);
    static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/AttachboundsMnu"))->set_active(
        engine.get_press_parameters().draw_attachbounds);
    static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/NoteboundsMnu"))->set_active(
        engine.get_press_parameters().draw_notebounds);
    static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/EOVboundsMnu"))->set_active(
        engine.get_press_parameters().draw_eov);
}

