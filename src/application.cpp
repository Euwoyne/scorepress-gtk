
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

#include "application.hh"
#include "cmdline_options.hh"
#include "controller.hh"
#include "config.hh"
#include "i18n.hh"

#include <scorepress/log.hh>
#include <sstream>
#include <iostream>

void ScorePressApp::on_activate()
{
    try
    {
        icon_manager.load("scorepress", "main");
        icon_manager.load("scorepress-sm", "main-small");
    }
    catch (IconManager::Error e)
    {
        std::cerr << "[WARNING] ScorePressApp::on_activate(): " << e << "\n";
    };
    
    key_listener.assign(KeyMap::KEY_UP,    GDK_KEY_Up);
    key_listener.assign(KeyMap::KEY_UP,    GDK_KEY_KP_Up);
    key_listener.assign(KeyMap::KEY_DOWN,  GDK_KEY_Down);
    key_listener.assign(KeyMap::KEY_DOWN,  GDK_KEY_KP_Down);
    key_listener.assign(KeyMap::KEY_RIGHT, GDK_KEY_Right);
    key_listener.assign(KeyMap::KEY_RIGHT, GDK_KEY_KP_Right);
    key_listener.assign(KeyMap::KEY_LEFT,  GDK_KEY_Left);
    key_listener.assign(KeyMap::KEY_LEFT,  GDK_KEY_KP_Left);
    key_listener.assign(KeyMap::KEY_HOME,  GDK_KEY_Home);
    key_listener.assign(KeyMap::KEY_HOME,  GDK_KEY_KP_Home);
    key_listener.assign(KeyMap::KEY_END,   GDK_KEY_End);
    key_listener.assign(KeyMap::KEY_END,   GDK_KEY_KP_End);
    
    key_listener.assign(KeyMap::KEY_NEWLINE,   GDK_KEY_Return);
    key_listener.assign(KeyMap::KEY_NEWLINE,   GDK_KEY_KP_Enter);
    key_listener.assign(KeyMap::KEY_PAGEBREAK, GDK_KEY_Return, true);
    key_listener.assign(KeyMap::KEY_PAGEBREAK, GDK_KEY_KP_Enter, true);
    key_listener.assign(KeyMap::KEY_NEWVOICE,  GDK_KEY_V, true);
    key_listener.assign(KeyMap::KEY_DELETE,    GDK_KEY_Delete);
    key_listener.assign(KeyMap::KEY_BACKSPACE, GDK_KEY_BackSpace);
    key_listener.assign(KeyMap::KEY_DELVOICE,  GDK_KEY_Delete, true);
    
    key_listener.assign(KeyMap::KEY_LONGA,   GDK_KEY_0); key_listener.assign(KeyMap::KEY_LONGA,   GDK_KEY_KP_0);
    key_listener.assign(KeyMap::KEY_BREVE,   GDK_KEY_5); key_listener.assign(KeyMap::KEY_BREVE,   GDK_KEY_KP_5);
    key_listener.assign(KeyMap::KEY_WHOLE,   GDK_KEY_1); key_listener.assign(KeyMap::KEY_WHOLE,   GDK_KEY_KP_1);
    key_listener.assign(KeyMap::KEY_HALF,    GDK_KEY_2); key_listener.assign(KeyMap::KEY_HALF,    GDK_KEY_KP_2);
    key_listener.assign(KeyMap::KEY_QUARTER, GDK_KEY_4); key_listener.assign(KeyMap::KEY_QUARTER, GDK_KEY_KP_4);
    key_listener.assign(KeyMap::KEY_EIGHTH,  GDK_KEY_8); key_listener.assign(KeyMap::KEY_EIGHTH,  GDK_KEY_KP_8);
    key_listener.assign(KeyMap::KEY_16TH,    GDK_KEY_6); key_listener.assign(KeyMap::KEY_16TH,    GDK_KEY_KP_6);
    key_listener.assign(KeyMap::KEY_32TH,    GDK_KEY_3); key_listener.assign(KeyMap::KEY_32TH,    GDK_KEY_KP_3);
    key_listener.assign(KeyMap::KEY_64TH,    GDK_KEY_7); key_listener.assign(KeyMap::KEY_64TH,    GDK_KEY_KP_7);
    key_listener.assign(KeyMap::KEY_128TH,   GDK_KEY_9); key_listener.assign(KeyMap::KEY_128TH,   GDK_KEY_KP_9);
    key_listener.assign(KeyMap::KEY_REST,    GDK_KEY_space);
    
    key_listener.assign(KeyMap::KEY_C, GDK_KEY_C); key_listener.assign(KeyMap::KEY_C, GDK_KEY_c);
    key_listener.assign(KeyMap::KEY_D, GDK_KEY_D); key_listener.assign(KeyMap::KEY_D, GDK_KEY_d);
    key_listener.assign(KeyMap::KEY_E, GDK_KEY_E); key_listener.assign(KeyMap::KEY_E, GDK_KEY_e);
    key_listener.assign(KeyMap::KEY_F, GDK_KEY_F); key_listener.assign(KeyMap::KEY_F, GDK_KEY_f);
    key_listener.assign(KeyMap::KEY_G, GDK_KEY_G); key_listener.assign(KeyMap::KEY_G, GDK_KEY_g);
    key_listener.assign(KeyMap::KEY_A, GDK_KEY_A); key_listener.assign(KeyMap::KEY_A, GDK_KEY_a);
    key_listener.assign(KeyMap::KEY_B, GDK_KEY_B); key_listener.assign(KeyMap::KEY_B, GDK_KEY_b);
    key_listener.assign(KeyMap::KEY_B, GDK_KEY_H); key_listener.assign(KeyMap::KEY_B, GDK_KEY_h);
    
    key_listener.assign(KeyMap::KEY_SHARP,         GDK_KEY_plus);
    key_listener.assign(KeyMap::KEY_FLAT,          GDK_KEY_minus);
    key_listener.assign(KeyMap::KEY_DOUBLESHARP,   GDK_KEY_asterisk);
    key_listener.assign(KeyMap::KEY_DOUBLEFLAT,    GDK_KEY_underscore);
    key_listener.assign(KeyMap::KEY_HALFSHARP,     GDK_KEY_plus, true);
    key_listener.assign(KeyMap::KEY_HALFFLAT,      GDK_KEY_minus, true);
    key_listener.assign(KeyMap::KEY_SHARPANDAHALF, GDK_KEY_asterisk, true);
    key_listener.assign(KeyMap::KEY_FLATANDAHALF,  GDK_KEY_underscore, true);
    
    key_listener.assign(KeyMap::KEY_8VA,        GDK_KEY_Shift_R);
    key_listener.assign(KeyMap::KEY_8VAB,       GDK_KEY_Shift_L);
    key_listener.assign(KeyMap::KEY_OCTAVEUP,   GDK_KEY_X); key_listener.assign(KeyMap::KEY_OCTAVEUP, GDK_KEY_x);
    key_listener.assign(KeyMap::KEY_OCTAVEDOWN, GDK_KEY_Y); key_listener.assign(KeyMap::KEY_OCTAVEDOWN, GDK_KEY_y);
    
    key_listener.assign(KeyMap::KEY_DOT,  GDK_KEY_period);
    key_listener.assign(KeyMap::KEY_2DOT, GDK_KEY_colon);
    key_listener.assign(KeyMap::KEY_NDOT, GDK_KEY_semicolon);
    
    key_listener.assign(KeyMap::KEY_STEMLENGTH, GDK_KEY_N);     key_listener.assign(KeyMap::KEY_STEMLENGTH, GDK_KEY_n);
    key_listener.assign(KeyMap::KEY_STEMDIR,    GDK_KEY_I);     key_listener.assign(KeyMap::KEY_STEMDIR,    GDK_KEY_i);
    key_listener.assign(KeyMap::KEY_CHROMATIC,  GDK_KEY_P);     key_listener.assign(KeyMap::KEY_CHROMATIC,  GDK_KEY_p);
    key_listener.assign(KeyMap::KEY_STAFFSHIFT, GDK_KEY_V);     key_listener.assign(KeyMap::KEY_STAFFSHIFT, GDK_KEY_v);
    key_listener.assign(KeyMap::KEY_MOVE,       GDK_KEY_J);     key_listener.assign(KeyMap::KEY_MOVE,       GDK_KEY_j);
    key_listener.assign(KeyMap::KEY_ACCMOVE,    GDK_KEY_K);     key_listener.assign(KeyMap::KEY_ACCMOVE,    GDK_KEY_k);
    key_listener.assign(KeyMap::KEY_NOBEAM,     GDK_KEY_F5);
    key_listener.assign(KeyMap::KEY_AUTOBEAM,   GDK_KEY_F6);
    key_listener.assign(KeyMap::KEY_CUTBEAM,    GDK_KEY_F7);
    key_listener.assign(KeyMap::KEY_FORCEBEAM,  GDK_KEY_F8);
    
    key_listener.set_head_insert_method(KeyMap::HEAD_HOLD);
    key_listener.assign(KeyMap::KEY_HEAD_MODE, GDK_KEY_Insert);
}

std::string ScorePressApp::get_next_unnamed() const
{
    static const std::string unnamed_str = _("Unsaved Score");
    unsigned int n = 1;
    std::string name;
    std::ostringstream ss;
    ss << unnamed_str << ' ' << n;
    name = ss.str();
    for (std::vector<Controller*>::const_iterator c = controllers.begin(); c != controllers.end();)
    {
        if ((*c)->get_filename() == name)
        {
            ss.seekp(0);
            ss << unnamed_str << ' ' << ++n;
            name = ss.str();
            c = controllers.begin();
        } else ++c;
    };
    return name;
}

int ScorePressApp::on_command_line(const Glib::RefPtr<Gio::ApplicationCommandLine>& command_line)
{
    if (controllers.empty())
        log.info("ScorePress " SCOREPRESS_VERSION_STRING "\n" SCOREPRESS_COPYRIGHT "\nLicensed under the EUPL V.1.1\n");
    CmdlineOptions options;
    int argn = 0;
    char** argv = command_line->get_arguments(argn);
    int ret = parse_cmdline(options, argn, argv) != 0;
    if (ret != 0) return (ret < 0) ? ret : 0;
    
    try
    {
        log.echo_info(!options.stdout.silent);
        log.echo_debug(!options.stdout.silent && options.stdout.debug);
        log.echo_verbose(!options.stdout.silent && options.stdout.verbose);
        log.echo_warn(!options.stdout.silent);
        log.echo_error(!options.stdout.silent);
        log.log_info(false);
        log.log_debug(!options.log.silent && options.log.debug);
        log.log_verbose(!options.log.silent && options.log.verbose);
        log.log_warn(!options.log.silent);
        log.log_error(!options.log.silent);
        
        if (options.files.empty())
        {
            if (!add_tab())
            {
                add_window();
                add_tab(true);
            };
            controllers.back()->set_filename(get_next_unnamed());
            controllers.back()->get_engine().get_document().meta.title = controllers.back()->get_filename();
            controllers.back()->change();
        };
        
        std::vector< Glib::RefPtr<Gio::File> > files;
        for (std::vector<std::string>::const_iterator file = options.files.begin(); file != options.files.end(); ++file)
            files.push_back(Gio::File::create_for_path(*file));
        
        if (!files.empty()) open(files);
    }
    catch (std::string s)
    {
        std::cerr << "[ERROR] ScorePressApp::on_command_line(): " << s << "\n";
    }
    catch (Glib::Error& e)
    {
        std::cerr << "[ERROR] ScorePressApp::on_command_line(): " << e.what() << "\n";
    }
    catch (...)
    {
        std::cerr << "[ERROR] ScorePressApp::on_command_line(): Unknown Exception\n";
    };

    return 0;
}

void ScorePressApp::on_open(const std::vector< Glib::RefPtr<Gio::File> >& files, const Glib::ustring& hint)
{
    for (std::vector< Glib::RefPtr<Gio::File> >::const_iterator i = files.begin(); i != files.end(); ++i)
    {
        if (!*i) continue;
        if (!add_tab())
        {
            add_window();
            add_tab(true);
        };
        controllers.back()->open(*i);
    };
    Gtk::Application::on_open(files, hint);
}

void ScorePressApp::on_window_hide(Gtk::Window* window)
{
    for (std::vector<Controller*>::iterator i = controllers.begin(); i != controllers.end();)
    {
        if (&(*i)->get_window() == window)
        {
            delete *i;
            i = controllers.erase(i);
        }
        else ++i;
    };
    delete window;
}

ScorePressApp::ScorePressApp() : Gtk::Application("ScorePress.Gtk.Main", Gio::APPLICATION_HANDLES_OPEN | Gio::APPLICATION_HANDLES_COMMAND_LINE)
{
    Glib::set_application_name(SCOREPRESS_TITLE);
}

ScorePressApp::~ScorePressApp()
{
    for (std::vector<Controller*>::iterator i = controllers.begin(); i != controllers.end(); ++i)
        delete *i;
}

bool ScorePressApp::add_tab(bool select)
{
    if (!get_active_window()) return false;
    controllers.push_back(new Controller(*static_cast<MainWnd*>(get_active_window()), key_listener));
    controllers.back()->log_set(log);
    static_cast<MainWnd*>(get_active_window())->add_view(*controllers.back(), select);
    return true;
}

void ScorePressApp::add_tab(MainWnd& wnd, bool select)
{
    controllers.push_back(new Controller(wnd, key_listener));
    controllers.back()->log_set(log);
    wnd.add_view(*controllers.back(), select);
}

void ScorePressApp::add_window()
{
    MainWnd* window = new MainWnd();
    Application::add_window(*window);
    window->log_set(log);
    window->setup(icon_manager.get("main-small"));
    window->show();
    window->grab_focus();
}

