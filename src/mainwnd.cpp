
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

#include "mainwnd.hh"
#include "config.hh"
#include "i18n.hh"
#include <vector>
#include <scorepress/log.hh>
#include <iostream>

// view class constructor
MainWnd::View::View(Controller& ctrl) : controller(&ctrl), widget(ctrl)
{
    scrollWnd.add(widget);
    widget.show();
    scrollWnd.show();
}

// zoom slider widget: value formatter
Glib::ustring MainWnd::ZoomScale::on_format_value(double value)
{
    try
    {
        return Glib::ustring::compose("%1%% ", 5.0 * value);
    }
    catch (std::exception e)
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

// UI layout (XML for UIManager)
static const char* UI = 
    "<ui>"
    "    <menubar name='MainMnu'>"
    "        <menu action='FileMnu'>"
    "            <menu action='NewMnu'>"
    "                <menuitem action='NewEmptyMnu'/>"
    "                <menuitem action='NewCustomMnu'/>"
    "            </menu>"
    "            <menuitem action='OpenMnu'/>"
    "            <menuitem action='SaveMnu'/>"
    "            <separator/>"
    "            <menuitem action='QuitMnu'/>"
    "        </menu>"
    "        <menu action='EditMnu'>"
    "            <menuitem action='UndoMnu'/>"
    "            <menuitem action='RedoMnu'/>"
    "            <separator/>"
    "            <menuitem action='CutMnu'/>"
    "            <menuitem action='CopyMnu'/>"
    "            <menuitem action='PasteMnu'/>"
    "        </menu>"
    "        <menu action='ViewMnu'>"
    "            <menu action='BoundsMnu'>"
    "                <menuitem action='LineboundsMnu'/>"
    "                <menuitem action='AttachboundsMnu'/>"
    "                <menuitem action='NoteboundsMnu'/>"
    "                <menuitem action='EOVboundsMnu'/>"
    "                <separator/>"
    "                <menuitem action='AllboundsMnu'/>"
    "                <menuitem action='NoboundsMnu'/>"
    "            </menu>"
    "            <separator/>"
    "            <menuitem action='RefreshMnu'/>"
    "        </menu>"
    "        <menu action='HelpMnu'>"
    "            <menuitem action='HelpIdxMnu'/>"
    "            <menuitem action='AboutMnu'/>"
    "        </menu>"
    "    </menubar>"
    "    <toolbar name='MainBar'>"
    "        <toolitem action='NewEmptyMnu'/>"
    "        <toolitem action='OpenMnu'/>"
    "        <toolitem action='SaveMnu'/>"
    "        <separator/>"
    "        <toolitem action='UndoMnu'/>"
    "        <toolitem action='RedoMnu'/>"
    "        <separator/>"
    "        <toolitem action='CutMnu'/>"
    "        <toolitem action='CopyMnu'/>"
    "        <toolitem action='PasteMnu'/>"
    "    </toolbar>"
    "    <toolbar name='ViewBar'>"
    "    </toolbar>"
    "</ui>";

// setup the "actionGrp" instance (requires properly set up menus)
inline void MainWnd::setup_action_groups()
{
    actionGrp = Gtk::ActionGroup::create();
    actionGrp->add(Gtk::Action::create("FileMnu", _("_File")));
    actionGrp->add(Gtk::Action::create("NewMnu", _("_New..."), _("New score")));
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
    actionGrp->add(Gtk::Action::create("CutMnu", Gtk::Stock::CUT, _("Cu_t"), _("Cut")),
                   Gtk::AccelKey("<control>x"),
                   sigc::mem_fun(*this, &MainWnd::menu_cut));
    actionGrp->add(Gtk::Action::create("CopyMnu", Gtk::Stock::COPY, _("_Copy"), _("Copy")),
                   Gtk::AccelKey("<control>c"),
                   sigc::mem_fun(*this, &MainWnd::menu_copy));
    actionGrp->add(Gtk::Action::create("PasteMnu", Gtk::Stock::PASTE, _("_Paste"), _("Paste")),
                   Gtk::AccelKey("<control>v"),
                   sigc::mem_fun(*this, &MainWnd::menu_paste));
    actionGrp->add(Gtk::Action::create("ViewMnu", _("_View")));
    actionGrp->add(Gtk::Action::create("RefreshMnu", Gtk::Stock::REFRESH, _("_Refresh"), _("Refresh view")),
                   Gtk::AccelKey("<control>r"),
                   sigc::mem_fun(*this, &MainWnd::menu_refresh));
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

// setup the "aboutDlg" instance
void MainWnd::setup_about_dialog()
{
    Glib::RefPtr<Glib::IOChannel> eupl = Glib::IOChannel::create_from_file(std::string(scorepress_datadir) += "/EUPL.txt", "r");
    eupl->read_to_end(licence);
    logo = Gdk::Pixbuf::create_from_file(std::string(scorepress_datadir) += "/logo.png");
    
    aboutDlg.set_program_name(SCOREPRESS_TITLE);
    aboutDlg.set_version(SCOREPRESS_VERSION_STRING);
    aboutDlg.set_copyright(SCOREPRESS_COPYRIGHT_UTF);
    aboutDlg.set_comments(_("ScorePress is a WYSIWYG music typesetting program."));
    aboutDlg.set_license(licence);
    aboutDlg.set_license_type(Gtk::LICENSE_CUSTOM);
    aboutDlg.set_website("https://github.com/Euwoyne/scorepress-gtk");
    aboutDlg.set_website_label("GitHub Repository");
    authors.push_back("Dominik Lehmann");
    authors.push_back("Gereon Kaiping");
    aboutDlg.set_authors(authors);
    //aboutDlg.set_documenters(documenters);
    artists.push_back("Micha Schinzel");
    aboutDlg.set_artists(artists);
    //aboutDlg.set_translator_credits("");
    aboutDlg.set_logo(logo);
    aboutDlg.set_wrap_license(true);
}

// tab switch
void MainWnd::on_switch_tab(Gtk::Widget* page, guint)
{
    try
    {
        // get controller/engine
        ScoreWidget* widget = static_cast<ScoreWidget*>(static_cast<Gtk::Viewport*>(static_cast<Gtk::ScrolledWindow*>(page)->get_child())->get_child());
        controller = &widget->get_controller();
        engine = &controller->get_engine();
        zoomScl->set_value(engine->get_press_parameters().scale / 50.0);
        
        // set window title
        this->set_title(Glib::ustring(engine->get_document().meta.title) += " - ScorePress");
        
        // refresh menus
        static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/LineboundsMnu"))->set_active(
            engine->get_press_parameters().draw_linebounds);
        static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/AttachboundsMnu"))->set_active(
            engine->get_press_parameters().draw_attachbounds);
        static_cast<Gtk::CheckMenuItem*>(uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/NoteboundsMnu"))->set_active(
            engine->get_press_parameters().draw_notebounds);
        
        // set focus
        widget->grab_focus();
    }
    catch (ScorePress::Error& s)
    {
        std::cerr << "[ERROR] MainWnd::on_switch_tab(): " << s << "\n";
    }
    catch (std::exception& e)
    {
        std::cerr << "[ERROR] MainWnd::on_switch_tab(): " << e.what() << "\n";
    }
    catch (...)
    {
        std::cerr << "[ERROR] MainWnd::on_switch_tab(): Unknown Exception\n";
    };
}

// tab close
void MainWnd::on_close_tab(Gtk::Widget* page, guint)
{
    try
    {
        // get controller
        ScoreWidget* widget = static_cast<ScoreWidget*>(static_cast<Gtk::Viewport*>(static_cast<Gtk::ScrolledWindow*>(page)->get_child())->get_child());
        Controller* ctrl = &widget->get_controller();
        
        // TODO: ask for save
        
        // delete the view
        ViewSet::iterator view = views.find(ctrl);
        if (view != views.end()) views.erase(view);
    }
    catch (ScorePress::Error& s)
    {
        std::cerr << "[ERROR] MainWnd::on_close_tab(): " << s << "\n";
    }
    catch (std::exception& e)
    {
        std::cerr << "[ERROR] MainWnd::on_close_tab(): " << e.what() << "\n";
    }
    catch (...)
    {
        std::cerr << "[ERROR] MainWnd::on_close_tab(): Unknown Exception\n";
    };
}

// window size change (center score)
void MainWnd::on_size_changed(Gtk::Allocation& allocation)
{
    try
    {
        ViewSet::iterator view = views.find(controller);
        if (view != views.end() && view->second->widget.get_realized())
            view->second->widget.center(allocation.get_width());
    }
    catch (ScorePress::Error& s)
    {
        std::cerr << "[ERROR] MainWnd::on_size_changed(): " << s << "\n";
    }
    catch (std::exception& e)
    {
        std::cerr << "[ERROR] MainWnd::on_size_changed(): " << e.what() << "\n";
    }
    catch (...)
    {
        std::cerr << "[ERROR] MainWnd::on_size_changed(): Unknown Exception\n";
    };
}

// zoom slider handler
void MainWnd::on_zoom_changed()
{
    try
    {
        // find current view
        ViewSet::iterator view = views.find(controller);
        if (!engine || view == views.end()) return;
        
        // set zoom parameter
        engine->get_press_parameters().scale = static_cast<int>(50.0 * zoomScl->get_value());
        
        // clear renderer cache
        controller->get_renderer().clear_cache();
        
        // redraw score
        if (view->second->widget.get_realized())
            view->second->widget.center(view->second->scrollWnd.get_width());
        if (view->second->scrollWnd.get_realized())
            view->second->scrollWnd.get_window()->invalidate(true);
    }
    catch (ScorePress::Error& s)
    {
        std::cerr << "[ERROR] MainWnd::on_zoom_changed(): " << s << "\n";
    }
    catch (std::exception& e)
    {
        std::cerr << "[ERROR] MainWnd::on_zoom_changed(): " << e.what() << "\n";
    }
    catch (...)
    {
        std::cerr << "[ERROR] MainWnd::on_zoom_changed(): Unknown Exception\n";
    };
}

// constructor
MainWnd::MainWnd() : controller(NULL), engine(NULL) {}

// setup window and controls
void MainWnd::setup(const Icon& icon)
{
    // setup window
    log_debug("setup window...");
    this->set_title(SCOREPRESS_TITLE);
    this->set_default_size(750, 530);
    this->set_icon_list(icon);
    
    setup_about_dialog();
    
    // setup menus
    log_debug("setup menus...");
    setup_action_groups();
    uiManager = Gtk::UIManager::create();
    uiManager->insert_action_group(actionGrp);
    uiManager->add_ui_from_string(UI);
    this->add_accel_group(uiManager->get_accel_group());
    mainMnu = static_cast<Gtk::Menu*>(uiManager->get_widget("/MainMnu"));
    mainBar = static_cast<Gtk::Toolbar*>(uiManager->get_widget("/MainBar"));
    mainBar->unset_toolbar_style();
    mainBar->set_can_default(false);
    mainBar->set_can_focus(false);
    viewBar = static_cast<Gtk::Toolbar*>(uiManager->get_widget("/ViewBar"));
    viewBar->unset_toolbar_style();
    viewBar->set_can_default(false);
    viewBar->set_can_focus(false);
    
    // setup controls
    log_debug("setup controls...");
    mainBarBox = new Gtk::HandleBox();
    zoomItm = new Gtk::ToolItem();
    zoomScl = new ZoomScale();
    viewBarBox = new Gtk::HandleBox();
    mainBox = new Gtk::VBox(false, 0);
    toolBox = new Gtk::HBox(false, 0);
    scoreBox = new Gtk::HBox(false, 0);
    scoreTabs = new Gtk::Notebook();
    statusBar = new Gtk::Statusbar();
    scoreTabs->set_scrollable(true);
    
    mainBarBox->add(*mainBar);
    viewBarBox->add(*viewBar);
    toolBox->pack_start(*mainBarBox, Gtk::PACK_EXPAND_WIDGET);
    toolBox->pack_start(*viewBarBox, Gtk::PACK_EXPAND_WIDGET);
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
    
    scoreTabs->signal_switch_page().connect(sigc::mem_fun(*this, &MainWnd::on_switch_tab), false);
    scoreTabs->signal_page_removed().connect(sigc::mem_fun(*this, &MainWnd::on_close_tab), false);
    zoomScl->signal_value_changed().connect(sigc::mem_fun(*this, &MainWnd::on_zoom_changed), false);
    
    // prepare controls
    uiManager->get_widget("/MainMnu/EditMnu/UndoMnu")->set_sensitive(false);
    uiManager->get_widget("/MainBar/UndoMnu")->set_sensitive(false);
    uiManager->get_widget("/MainMnu/EditMnu/RedoMnu")->set_sensitive(false);
    uiManager->get_widget("/MainBar/RedoMnu")->set_sensitive(false);
    uiManager->get_widget("/MainMnu/EditMnu/PasteMnu")->set_sensitive(false);
    uiManager->get_widget("/MainBar/PasteMnu")->set_sensitive(false);
    uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/LineboundsMnu")->set_sensitive(false);
    uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/AttachboundsMnu")->set_sensitive(false);
    uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/NoteboundsMnu")->set_sensitive(false);
    this->show_all();
}

// destructor
MainWnd::~MainWnd()
{
    log_debug("delete window");
    delete statusBar;
    delete scoreTabs;
    delete scoreBox;
    delete zoomScl;
    delete zoomItm;
    delete mainBarBox;
    delete viewBarBox;
    delete toolBox;
    delete mainBox;
    for (ViewSet::iterator i = views.begin(); i != views.end(); ++i)
        delete i->second;
}

// add new tab to show the given controller
void MainWnd::add_view(Controller& ctrl, bool select)
{
    // check curent state
    if (views.empty())  // if no tab exists yet
    {                           //      activate menus
        uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/LineboundsMnu")->set_sensitive(true);
        uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/AttachboundsMnu")->set_sensitive(true);
        uiManager->get_widget("/MainMnu/ViewMnu/BoundsMnu/NoteboundsMnu")->set_sensitive(true);
        select = true;          //      select the added view
    };
    
    // add view
    log_debug("add view...");
    ViewSet::iterator view = views.find(&ctrl);
    int idx = -1;
    if (view != views.end())
    {
        idx = scoreTabs->page_num(view->second->scrollWnd);
    };
    if (idx < 0)
    {
        view = views.insert(std::pair<Controller*, View*>(&ctrl, new View(ctrl))).first;
        view->second->scrollWnd.get_child()->signal_size_allocate().connect(sigc::mem_fun(*this, &MainWnd::on_size_changed), false);
        idx = scoreTabs->append_page(view->second->scrollWnd, ctrl.get_filename());
    };
    
    // select the new view
    if (select)
    {
        scoreTabs->set_current_page(idx);
    };
}

// refresh tab title
bool MainWnd::refresh_label(Controller& ctrl)
{
    ViewSet::iterator view = views.find(&ctrl);
    if (view == views.end()) return false;
    scoreTabs->set_tab_label_text(view->second->scrollWnd, ctrl.get_filename());
    return true;
}

// show tab with given Controller
bool MainWnd::select_view(Controller& ctrl)
{
    ViewSet::iterator view = views.find(&ctrl);
    if (view == views.end()) return false;
    return select_view(scoreTabs->page_num(view->second->scrollWnd));
}

// show tab with given index
bool MainWnd::select_view(int idx)
{
    if (idx < 0 || idx >= scoreTabs->get_n_pages())
        return false;
    scoreTabs->set_current_page(idx);
    return true;
}

#include <iostream>
void MainWnd::menu_newEmpty() {std::cout << "NEW (empty)\n";}
void MainWnd::menu_newCustom() {std::cout << "NEW (wizard)\n";}

void MainWnd::menu_open() {std::cout << "Open!\n";}
void MainWnd::menu_save() {std::cout << "Save\n";}
void MainWnd::menu_quit() {std::cout << "QUIT!\n"; exit(0);}

void MainWnd::menu_undo() {std::cout << "Undo\n";}
void MainWnd::menu_redo() {std::cout << "Redo\n";}
void MainWnd::menu_cut() {std::cout << "CUT\n";}
void MainWnd::menu_copy() {std::cout << "COPY\n";}
void MainWnd::menu_paste() {std::cout << "PASTE\n";}

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

void MainWnd::menu_help() {std::cout << "Help (?)\n";}

