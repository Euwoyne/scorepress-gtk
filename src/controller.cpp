
/*
  ScorePress - Music Engraving Software  (scorepress-gtk)
  Copyright (C) 2012 Dominik Lehmann
  
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

#include "controller.hh"
#include "mainwnd.hh"
#include "configuration.hh"

void Controller::setup_engine()
{
    // setup engine
    engine->set_test(renderer->get_sprites());
    engine->set_resolution((1000L * Gdk::Screen::get_default()->get_width())  / Gdk::Screen::get_default()->get_width_mm(),
                           (1000L * Gdk::Screen::get_default()->get_height()) / Gdk::Screen::get_default()->get_height_mm());
}

Controller::Controller(MainWnd& wnd, KeyListener& keys) : renderer(new RSVGRenderer()),
                                                          engine(new ScorePress::Engine(renderer->get_sprites())),
                                                          keylistener(&keys),
                                                          window(&wnd)
{
    renderer->load(conf.data_path + "/symbol/default.svg");
    setup_engine();
    window->add_view(*this);
}

Controller::Controller(MainWnd& wnd, KeyListener& keys, const std::string& sprite_file) : renderer(new RSVGRenderer()),
                                                                                          engine(new ScorePress::Engine(renderer->get_sprites())),
                                                                                          keylistener(&keys),
                                                                                          window(&wnd)
{
    renderer->load(sprite_file);
    setup_engine();
    window->add_view(*this);
}

Controller::~Controller()
{
    delete engine;
    delete renderer;
}

void Controller::set_filename(const std::string& s)
{
    filename = s;
    window->refresh_label(*this);
}

/*
int Controller::run(int argn, char* argv[])
{
    if (running)
    {
        std::cerr << "Exception caught: Unable to execute Controller::run(): Controller is already running\n";
        return -1;
    };
    
    try
    {
        running = true;
        printf("ScorePress v%i.%i.%i\n", SCOREPRESS_VERSION_MAJOR, SCOREPRESS_VERSION_MINOR, SCOREPRESS_VERSION_PATCH);
        
        setlocale(LC_ALL, "");
        if (parse_cmdline(argn, argv) != 0) return 0;
        
        ScorePress::Log::echo_info(!cmdline_options.silent);
        ScorePress::Log::echo_debug(!cmdline_options.silent && cmdline_options.debug);
        ScorePress::Log::echo_verbose(!cmdline_options.silent && cmdline_options.verbose);
        ScorePress::Log::echo_warn(!cmdline_options.silent);
        ScorePress::Log::echo_error(!cmdline_options.silent);
        ScorePress::Log::log_info(false);
        ScorePress::Log::log_debug(cmdline_options.log.debug);
        ScorePress::Log::log_verbose(cmdline_options.log.verbose);
        ScorePress::Log::log_warn(true);
        ScorePress::Log::log_error(true);
        
        gnomekit.reset();
        delete renderer;
        delete engine;
        delete icon_manager;
        delete main_window;
        
        ScorePress::Log::debug("prepare Gnome...");
        gnomekit = Gtk::Application::create(argn, argv, "ScorePress.Gtk.Main", Gio::APPLICATION_FLAGS_NONE);
        
        ScorePress::Log::debug("setup renderer...");
        renderer = new RSVGRenderer();
        renderer->load(conf.data_path + "/symbol/default.svg");
        renderer->dump();
        
        ScorePress::Log::debug("setup engine->..");
        engine = new ScorePress::Engine(renderer->get_sprites());
        engine->set_resolution((1000L * Gdk::Screen::get_default()->get_width())  / Gdk::Screen::get_default()->get_width_mm(),
                               (1000L * Gdk::Screen::get_default()->get_height()) / Gdk::Screen::get_default()->get_height_mm());
        
        ScorePress::Log::debug("setup gui...");
        icon_manager = new IconManager();
        icon_manager->load("score", "main");
        icon_manager->load("score-sm", "main-small");
        main_window = new MainWnd(*this, icon_manager->get("main-small"));
        gnomekit->add_window(*main_window);
        
        ScorePress::Log::debug("setup... [DONE]");
        setup = true;
        gnomekit->run();
        setup = false;
        running = false;
        return 0;
    }
    catch (std::string s)
    {
        std::cerr << "Exception caught: " << s << "\n";
        running = false;
        return -1;
    };
}
*/

#include <iostream>
bool Controller::open(const Glib::RefPtr<Gio::File>& file)
{
    std::cout << "OPEN: " << file->get_basename() << "\n";
    filepath = file->get_path();
    filename = file->get_basename();
    // TODO: file parsing
    return true;
}

void Controller::mouse_on(double x, double y)
{
    engine->cursor.set_pos(ScorePress::Position<ScorePress::mpx_t>(
                                x / engine->get_press_parameters().scale,
                                y / engine->get_press_parameters().scale),
                            engine->get_viewport());
    window->refresh();
}

void Controller::mouse_off(double, double)
{
}

void Controller::set_scale(unsigned int scale)
{
    //if (!setup) return;
    engine->get_press_parameters().scale = scale;
    window->refresh();
}

void Controller::set_linebounds(bool value)
{
    //if (!setup) return;
    engine->get_press_parameters().draw_linebounds = value;
    window->refresh();
}

void Controller::set_attachbounds(bool value)
{
    //if (!setup) return;
    engine->get_press_parameters().draw_attachbounds = value;
    window->refresh();
}

void Controller::set_notebounds(bool value)
{
    //if (!setup) return;
    engine->get_press_parameters().draw_notebounds = value;
    window->refresh();
}

