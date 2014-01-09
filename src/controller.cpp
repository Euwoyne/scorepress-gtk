
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

#include <scorepress/config.hh>  // libscorepress_datadir

#include "controller.hh"
#include "mainwnd.hh"
#include "config.hh"

#include "_testdoc.cpp"

inline static int _round(const double d) {return static_cast<int>(d + 0.5);}

void Controller::setup_engine()
{
    // setup engine
    set_test(document, renderer.get_sprites());
    engine.set_resolution((1000L * Gdk::Screen::get_default()->get_width())  / Gdk::Screen::get_default()->get_width_mm(),
                           (1000L * Gdk::Screen::get_default()->get_height()) / Gdk::Screen::get_default()->get_height_mm());
}

Controller::Controller(MainWnd& wnd, KeyListener& keys) : engine(document, renderer.get_sprites()),
                                                          keylistener(keys),
                                                          window(wnd)
{
    renderer.load(std::string(libscorepress_datadir) += "/symbol/default.svg");
    setup_engine();
    cursor = &engine.get_cursor();
    window.add_view(*this);
}

Controller::Controller(MainWnd& wnd, KeyListener& keys, const std::string& sprite_file) : engine(document, renderer.get_sprites()),
                                                                                          keylistener(keys),
                                                                                          window(wnd)
{
    renderer.load(sprite_file);
    setup_engine();
    cursor = &engine.get_cursor();
    window.add_view(*this);
}

void Controller::set_filename(const std::string& s)
{
    filename = s;
    window.refresh_label(*this);
}

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
    cursor = &engine.get_cursor(ScorePress::Position<ScorePress::mpx_t>(static_cast<int>(x), static_cast<int>(y)), layout);
    window.refresh();
}

void Controller::mouse_off(double, double)
{
}

void Controller::set_scale(unsigned int scale)
{
    //if (!setup) return;
    engine.get_press_parameters().scale = scale;
    window.refresh();
}

void Controller::set_linebounds(bool value)
{
    //if (!setup) return;
    engine.get_press_parameters().draw_linebounds = value;
    window.refresh();
}

void Controller::set_attachbounds(bool value)
{
    //if (!setup) return;
    engine.get_press_parameters().draw_attachbounds = value;
    window.refresh();
}

void Controller::set_notebounds(bool value)
{
    //if (!setup) return;
    engine.get_press_parameters().draw_notebounds = value;
    window.refresh();
}

void Controller::set_eovbounds(bool value)
{
    //if (!setup) return;
    engine.get_press_parameters().draw_eov = value;
    window.refresh();
}

void Controller::log_set(ScorePress::Log& log)
{
    ScorePress::Logging::log_set(log);
    engine.log_set(log);
    window.log_set(log);
}

void Controller::log_unset()
{
    ScorePress::Logging::log_unset();
    engine.log_unset();
    window.log_unset();
}

