
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

#include "application.hh"
#include "mainwnd.hh"       // MainWnd
#include "config.hh"        // SCOREPRESS_TITLE
#include "i18n.hh"          // I18N, _

#include <iostream>

// return "Unsaved Score #", with number # not used in controllers
std::string Application::get_next_unnamed() const
{
    ///  Application
    /// =============
    ///
    /// Tab-title for unsaved files
    static const std::string unnamed_str = _("Unsaved Score");
    
    unsigned int n = 1;
    std::string name;
    std::ostringstream ss;
    ss << unnamed_str << ' ' << n;
    name = ss.str();
    for (std::list<Controller*>::const_iterator c = controllers.begin(); c != controllers.end();)
    {
        if ((*c)->get_basename() == name)
        {
            ss.seekp(0);
            ss << unnamed_str << ' ' << ++n;
            name = ss.str();
            c = controllers.begin();
        } else ++c;
    };
    return name;
}

// default constructor
Application::Application() : Gtk::Application("ScorePress.Gtk.Main", Gio::APPLICATION_HANDLES_OPEN | Gio::APPLICATION_HANDLES_COMMAND_LINE), key_listener(key_map)
{
    Glib::set_application_name(SCOREPRESS_TITLE);
    
    about_dialog.authors.push_back("Dominik Lehmann");
    about_dialog.authors.push_back("Gereon Kaiping");
    about_dialog.artists.push_back("Micha Schinzel");
    about_dialog.setup();
}

// default destructor
Application::~Application()
{
    for (std::list<Controller*>::iterator i = controllers.begin(); i != controllers.end(); ++i)
        delete *i;
}

// add new window to the application
void Application::add_window()
{
    MainWnd* window = new MainWnd();
    Gtk::Application::add_window(*window);
    window->log_set(log);
    window->setup(icon_manager.get("main-small"), about_dialog);
    window->show();
    window->grab_focus();
}

// application startup code
void Application::on_startup()
{
    // call base GTK startup
    Gtk::Application::on_startup();
    
    // load icons
    try
    {
        icon_manager.load("scorepress",    "main");
        icon_manager.load("scorepress-sm", "main-small");
    }
    catch (IconManager::Error& e)
    {
        std::cerr << "[WARNING] ScorePressApp::on_activate(): " << e << "\n";
    };
    
    // setup key-listener
    key_map.assign(KeyMap::KEY_UP,    GDK_KEY_Up);
    key_map.assign(KeyMap::KEY_UP,    GDK_KEY_KP_Up);
    key_map.assign(KeyMap::KEY_DOWN,  GDK_KEY_Down);
    key_map.assign(KeyMap::KEY_DOWN,  GDK_KEY_KP_Down);
    key_map.assign(KeyMap::KEY_RIGHT, GDK_KEY_Right);
    key_map.assign(KeyMap::KEY_RIGHT, GDK_KEY_KP_Right);
    key_map.assign(KeyMap::KEY_LEFT,  GDK_KEY_Left);
    key_map.assign(KeyMap::KEY_LEFT,  GDK_KEY_KP_Left);
    key_map.assign(KeyMap::KEY_HOME,  GDK_KEY_Home);
    key_map.assign(KeyMap::KEY_HOME,  GDK_KEY_KP_Home);
    key_map.assign(KeyMap::KEY_END,   GDK_KEY_End);
    key_map.assign(KeyMap::KEY_END,   GDK_KEY_KP_End);
    
    key_map.assign(KeyMap::KEY_NEWLINE,   GDK_KEY_Return);
    key_map.assign(KeyMap::KEY_NEWLINE,   GDK_KEY_KP_Enter);
    key_map.assign(KeyMap::KEY_PAGEBREAK, GDK_KEY_Return, GDK_CONTROL_MASK);
    key_map.assign(KeyMap::KEY_PAGEBREAK, GDK_KEY_KP_Enter, GDK_CONTROL_MASK);
    key_map.assign(KeyMap::KEY_NEWVOICE,  GDK_KEY_V, GDK_CONTROL_MASK);
    key_map.assign(KeyMap::KEY_DELETE,    GDK_KEY_Delete);
    key_map.assign(KeyMap::KEY_BACKSPACE, GDK_KEY_BackSpace);
    key_map.assign(KeyMap::KEY_DELVOICE,  GDK_KEY_Delete, GDK_CONTROL_MASK);
    key_map.assign(KeyMap::KEY_DELBREAK,  GDK_KEY_BackSpace, GDK_CONTROL_MASK);
    
    key_map.assign(KeyMap::KEY_LONGA,   GDK_KEY_0); key_map.assign(KeyMap::KEY_LONGA,   GDK_KEY_KP_0);
    key_map.assign(KeyMap::KEY_BREVE,   GDK_KEY_5); key_map.assign(KeyMap::KEY_BREVE,   GDK_KEY_KP_5);
    key_map.assign(KeyMap::KEY_WHOLE,   GDK_KEY_1); key_map.assign(KeyMap::KEY_WHOLE,   GDK_KEY_KP_1);
    key_map.assign(KeyMap::KEY_HALF,    GDK_KEY_2); key_map.assign(KeyMap::KEY_HALF,    GDK_KEY_KP_2);
    key_map.assign(KeyMap::KEY_QUARTER, GDK_KEY_4); key_map.assign(KeyMap::KEY_QUARTER, GDK_KEY_KP_4);
    key_map.assign(KeyMap::KEY_EIGHTH,  GDK_KEY_8); key_map.assign(KeyMap::KEY_EIGHTH,  GDK_KEY_KP_8);
    key_map.assign(KeyMap::KEY_16TH,    GDK_KEY_6); key_map.assign(KeyMap::KEY_16TH,    GDK_KEY_KP_6);
    key_map.assign(KeyMap::KEY_32TH,    GDK_KEY_3); key_map.assign(KeyMap::KEY_32TH,    GDK_KEY_KP_3);
    key_map.assign(KeyMap::KEY_64TH,    GDK_KEY_7); key_map.assign(KeyMap::KEY_64TH,    GDK_KEY_KP_7);
    key_map.assign(KeyMap::KEY_128TH,   GDK_KEY_9); key_map.assign(KeyMap::KEY_128TH,   GDK_KEY_KP_9);
    key_map.assign(KeyMap::KEY_REST,    GDK_KEY_space);
    
    key_map.assign(KeyMap::KEY_C, GDK_KEY_C); key_map.assign(KeyMap::KEY_C, GDK_KEY_c);
    key_map.assign(KeyMap::KEY_D, GDK_KEY_D); key_map.assign(KeyMap::KEY_D, GDK_KEY_d);
    key_map.assign(KeyMap::KEY_E, GDK_KEY_E); key_map.assign(KeyMap::KEY_E, GDK_KEY_e);
    key_map.assign(KeyMap::KEY_F, GDK_KEY_F); key_map.assign(KeyMap::KEY_F, GDK_KEY_f);
    key_map.assign(KeyMap::KEY_G, GDK_KEY_G); key_map.assign(KeyMap::KEY_G, GDK_KEY_g);
    key_map.assign(KeyMap::KEY_A, GDK_KEY_A); key_map.assign(KeyMap::KEY_A, GDK_KEY_a);
    key_map.assign(KeyMap::KEY_B, GDK_KEY_B); key_map.assign(KeyMap::KEY_B, GDK_KEY_b);
    key_map.assign(KeyMap::KEY_B, GDK_KEY_H); key_map.assign(KeyMap::KEY_B, GDK_KEY_h);
    
    key_map.assign(KeyMap::KEY_SHARP,         GDK_KEY_plus);
    key_map.assign(KeyMap::KEY_FLAT,          GDK_KEY_minus);
    key_map.assign(KeyMap::KEY_DOUBLESHARP,   GDK_KEY_asterisk);
    key_map.assign(KeyMap::KEY_DOUBLEFLAT,    GDK_KEY_underscore);
    key_map.assign(KeyMap::KEY_HALFSHARP,     GDK_KEY_plus, GDK_CONTROL_MASK);
    key_map.assign(KeyMap::KEY_HALFFLAT,      GDK_KEY_minus, GDK_CONTROL_MASK);
    key_map.assign(KeyMap::KEY_SHARPANDAHALF, GDK_KEY_asterisk, GDK_CONTROL_MASK);
    key_map.assign(KeyMap::KEY_FLATANDAHALF,  GDK_KEY_underscore, GDK_CONTROL_MASK);
    
    key_map.assign(KeyMap::KEY_8VA,        GDK_KEY_Shift_R);
    key_map.assign(KeyMap::KEY_8VAB,       GDK_KEY_Shift_L);
    key_map.assign(KeyMap::KEY_OCTAVEUP,   GDK_KEY_X); key_map.assign(KeyMap::KEY_OCTAVEUP, GDK_KEY_x);
    key_map.assign(KeyMap::KEY_OCTAVEDOWN, GDK_KEY_Y); key_map.assign(KeyMap::KEY_OCTAVEDOWN, GDK_KEY_y);
    
    key_map.assign(KeyMap::KEY_1DOT, GDK_KEY_period);
    key_map.assign(KeyMap::KEY_2DOT, GDK_KEY_colon);
    key_map.assign(KeyMap::KEY_NDOT, GDK_KEY_semicolon);
    key_map.assign(KeyMap::NDOT_0,   GDK_KEY_0);
    key_map.assign(KeyMap::NDOT_1,   GDK_KEY_1);
    key_map.assign(KeyMap::NDOT_2,   GDK_KEY_2);
    key_map.assign(KeyMap::NDOT_3,   GDK_KEY_3);
    key_map.assign(KeyMap::NDOT_4,   GDK_KEY_4);
    key_map.assign(KeyMap::NDOT_5,   GDK_KEY_5);
    key_map.assign(KeyMap::NDOT_6,   GDK_KEY_6);
    key_map.assign(KeyMap::NDOT_7,   GDK_KEY_7);
    key_map.assign(KeyMap::NDOT_8,   GDK_KEY_8);
    key_map.assign(KeyMap::NDOT_9,   GDK_KEY_9);
    
    key_map.assign(KeyMap::KEY_STEMLENGTH, GDK_KEY_N);     key_map.assign(KeyMap::KEY_STEMLENGTH, GDK_KEY_n);
    key_map.assign(KeyMap::STEMLENGTH_UP,       GDK_KEY_Up);
    key_map.assign(KeyMap::STEMLENGTH_DOWN,     GDK_KEY_Down);
    key_map.assign(KeyMap::STEMLENGTH_BEAMUP,   GDK_KEY_Left);
    key_map.assign(KeyMap::STEMLENGTH_BEAMDOWN, GDK_KEY_Right);
    key_map.assign(KeyMap::STEMLENGTH_AUTO,     GDK_KEY_Home);
    
    key_map.assign(KeyMap::KEY_STEMDIR,    GDK_KEY_I);     key_map.assign(KeyMap::KEY_STEMDIR,    GDK_KEY_i);
    key_map.assign(KeyMap::STEMDIR_UP,     GDK_KEY_Up);
    key_map.assign(KeyMap::STEMDIR_DOWN,   GDK_KEY_Down);
    key_map.assign(KeyMap::STEMDIR_AUTO,   GDK_KEY_Home);
    
    key_map.assign(KeyMap::KEY_CHROMATIC,  GDK_KEY_P);     key_map.assign(KeyMap::KEY_CHROMATIC,  GDK_KEY_p);
    key_map.assign(KeyMap::CHROMATIC_UP,   GDK_KEY_Up);
    key_map.assign(KeyMap::CHROMATIC_DOWN, GDK_KEY_Down);
    
    key_map.assign(KeyMap::KEY_MOVE,       GDK_KEY_J);     key_map.assign(KeyMap::KEY_MOVE,       GDK_KEY_j);
    key_map.assign(KeyMap::MOVE_UP,    GDK_KEY_Up);
    key_map.assign(KeyMap::MOVE_DOWN,  GDK_KEY_Down);
    key_map.assign(KeyMap::MOVE_LEFT,  GDK_KEY_Left);
    key_map.assign(KeyMap::MOVE_RIGHT, GDK_KEY_Right);
    key_map.assign(KeyMap::MOVE_HOME,  GDK_KEY_Home);
    
    key_map.assign(KeyMap::KEY_ACCMOVE,    GDK_KEY_K);     key_map.assign(KeyMap::KEY_ACCMOVE,    GDK_KEY_k);
    key_map.assign(KeyMap::ACCMOVE_LEFT,  GDK_KEY_Left);
    key_map.assign(KeyMap::ACCMOVE_RIGHT, GDK_KEY_Right);
    key_map.assign(KeyMap::ACCMOVE_HOME,  GDK_KEY_Home);
    
    key_map.assign(KeyMap::KEY_STAFFSHIFT,  GDK_KEY_V);    key_map.assign(KeyMap::KEY_STAFFSHIFT, GDK_KEY_v);
    key_map.assign(KeyMap::STAFFSHIFT_UP,   GDK_KEY_Up);
    key_map.assign(KeyMap::STAFFSHIFT_DOWN, GDK_KEY_Down);
    
    key_map.assign(KeyMap::KEY_BEAM_NONE,   GDK_KEY_F5);
    key_map.assign(KeyMap::KEY_BEAM_AUTO,   GDK_KEY_F6);
    key_map.assign(KeyMap::KEY_BEAM_FORCED, GDK_KEY_F8);
    key_map.assign(KeyMap::KEY_BEAM_CUT,    GDK_KEY_F7);
    
    key_map.set_head_insert_method(KeyMap::HEAD_HOLD);
    key_map.assign(KeyMap::KEY_HEAD_MODE, GDK_KEY_Insert);
    
    // TODO: load general configuration file
    // TODO: setup plugins
}

// command line entry point
int Application::on_command_line(const Glib::RefPtr<Gio::ApplicationCommandLine>& command_line)
{
    ///  Commandline Options
    /// =====================
    ///
    /// help text
    static const char* help_text = _("\
    Usage: score [options] [filename]\n\
    \n\
     Log Options:\n\
       --nolog            don't create any log\n\
       --debuglog         log debug output\n\
       --verboselog       log verbose output\n\
       --logfile <file>   write log to file\n\
    \n\
     General Options:\n\
       -s, --silent       emit no output on stdout\n\
       -d, --debug        generate debug output\n\
       -v, --verbose      generate verbose output\n\
       -h, --help         print this help\n\
       -V, --version      print program version\n");
    // greetings text
    static const char* greetings = "ScorePress " SCOREPRESS_VERSION_STRING;
    // copyright notice
    static const char* copyright = "This application is licensed under the EUPL, Version 1.1";
    
    puts(greetings);
    
    // get arguments
    int argn = 0;
    char** argv = command_line->get_arguments(argn);
    
    // argument data
    CmdlineOptions opt;
    bool version = false;
    bool dirs = false;
    bool help = false;
    bool help_gtk = false;
    std::string arg;
    
    // check arguments
    for (int i = 1; i < argn; ++i)
    {
        arg = argv[i];
        if (arg[0] != '-') opt.files.push_back(arg);    // check if option or file
        
        if (arg[1] == '-')  // if we got a long option
        {
                 if (arg == "--nolog"     ) opt.log.silent = true;
            else if (arg == "--debuglog"  ) opt.log.debug = true;
            else if (arg == "--verboselog") opt.log.verbose = true;
            else if (arg == "--logfile"   && i < argn-1) opt.log.file = argv[++i];
            else if (arg == "--silent"    ) opt.stdout.silent = true;
            else if (arg == "--debug"     ) opt.stdout.debug = true;
            else if (arg == "--verbose"   ) opt.stdout.verbose = true;
            else if (arg == "--version"   ) version = true;
            else if (arg == "--dirs"      ) dirs = true;
            else if (arg == "--help"      ) help = true;
            else if (arg == "--help-all"  ) help = help_gtk = true;
            else if (arg == "--help-gtk" ||
                     arg == "--help-gapplication") help_gtk = true;
            else if (strncmp(argv[i], "--display=", 10))
                /// unrecognized option warning (%s is replaced by the given option)
                printf(_("Warning: Unrecognized option \"%s\".\n"), arg.c_str());
        }
        else                // otherwise check for short option string
        {
            for (int pos = 1; argv[i][pos] != 0; ++pos)
            {
                switch (argv[i][pos])
                {
                case 's': opt.stdout.silent = true; break;
                case 'd': opt.stdout.debug = true; break;
                case 'v': opt.stdout.verbose = true; break;
                case 'V': version = true; break;
                case 'D': dirs = true; break;
                case 'h': help = true; break;
                /// unrecognized option warning (-%c is replaced by the given short option)
                default:  printf(_("Warning: Unrecognized option \"-%c\".\n"), argv[i][pos]); break;
                };
            };
        };
    };
    
    // --help (-h)
    if (help)
    {
        puts(help_text);
        if (help_gtk)
        {
            puts("");
            Gtk::Application::on_command_line(command_line);
        };
        puts(copyright);
    }
    
    // --version (-V)
    else if (version)
    {
        puts(copyright);
        puts("");
        if (SCOREPRESS_DEBUG || opt.stdout.verbose)
        {
            /// title line for library version output (argument --version, -V)
            puts(_("Libraries:"));
            print_library_info();
            puts("");
        };
    };
    
    // --dirs (-D)
    if (dirs)
    {
        /// title line for directory info output (argument --dirs, -D)
        puts(_("Directories:"));
        print_directory_info();
        puts("");
        return 0;
    };
    
    if (help || version) return 0;
    
    // normal startup
    try
    {
        // parse GTK+ options
        Gtk::Application::on_command_line(command_line);
        
        // setup log
        log.echo_info(!opt.stdout.silent);
        log.echo_debug(!opt.stdout.silent && opt.stdout.debug);
        log.echo_verbose(!opt.stdout.silent && opt.stdout.verbose);
        log.echo_warn(!opt.stdout.silent);
        log.echo_error(!opt.stdout.silent);
        log.log_info(false);
        log.log_debug(opt.log.debug);
        log.log_verbose(opt.log.verbose);
        log.log_warn(true);
        log.log_error(true);
        if (!opt.log.file.empty())
            log.open(opt.log.file.c_str());
        
        // setup window
        if (!get_active_window())
            add_window();
        
        // if no file is specified
        if (opt.files.empty())
        {
            MainWnd& window = *static_cast<MainWnd*>(get_active_window());
            controllers.push_back(new Controller(window, key_listener));
            controllers.back()->log_set(log);
            controllers.back()->set_basename(get_next_unnamed());
        }
        
        // if files are to be opened
        else
        {
            std::vector< Glib::RefPtr<Gio::File> > files;
            for (std::vector<std::string>::const_iterator file = opt.files.begin(); file != opt.files.end(); ++file)
                files.push_back(Gio::File::create_for_path(*file));
            open(files);
        };
    }
    catch (std::string& s)
    {
        std::cerr << "[ERROR] ScorePressApp::on_command_line(): " << s << "\n";
        return -2;
    }
    catch (Glib::Error& e)
    {
        std::cerr << "[ERROR] ScorePressApp::on_command_line(): " << e.what() << "\n";
        return -2;
    }
    catch (...)
    {
        std::cerr << "[ERROR] ScorePressApp::on_command_line(): Unknown Exception\n";
        return -2;
    };
    
    return 0;
}

// open given files (entry point)
void Application::on_open(const std::vector< Glib::RefPtr<Gio::File> >& files, const Glib::ustring& hint)
{
    Gtk::Application::on_open(files, hint);
    
    // setup window
    if (!get_active_window())
        add_window();
    
    MainWnd& window = *static_cast<MainWnd*>(get_active_window());
    
    // open all given files
    for (std::vector< Glib::RefPtr<Gio::File> >::const_iterator i = files.begin(); i != files.end(); ++i)
    {
        if (!*i) continue;
        controllers.push_back(new Controller(window, key_listener));
        controllers.back()->log_set(log);
        controllers.back()->open(*i);
    };
}

// window closing handler
void Application::on_window_hide(Gtk::Window* window)
{
    std::cout << "Hide Window\n";
    
    // close all controllers within the given window
    for (std::list<Controller*>::iterator i = controllers.begin(); i != controllers.end();)
    {
        if (&(*i)->get_window() == window)
        {
            delete *i;
            i = controllers.erase(i);
        }
        else
            ++i;
    };
    
    // dispose of the window
    delete window;
}
