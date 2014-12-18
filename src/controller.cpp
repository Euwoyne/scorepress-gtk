
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

#include "controller.hh"
#include "mainwnd.hh"
#include <scorepress/test.hh>
#include <scorepress/config.hh>

// constructor (with target window and key-listener)
Controller::Controller(MainWnd& window, KeyListener& _keys) : engine(document, renderer.get_sprites()), view(*this, window), keys(_keys)
{
    // load default spriteset
    ScorePress::Renderer::ReaderPtr reader = renderer.spriteset_reader();
    reader->open(std::string(scorepress_config.datadir) += "/symbol/default.svg");
    renderer.add_spriteset(reader);
    
    // setup engine
    engine.set_resolution((1000L * Gdk::Screen::get_default()->get_width())  / Gdk::Screen::get_default()->get_width_mm(),
                          (1000L * Gdk::Screen::get_default()->get_height()) / Gdk::Screen::get_default()->get_height_mm());
    
    // register view
    window.register_view(view, false);
}

// destructor
Controller::~Controller() {}

// open document-file (if NULL, load test-document)
void Controller::open(Glib::RefPtr<Gio::File> file)
{
    // if no file is give, load test-document
    if (!file)
    {
        document = ScorePress::Test::get_document(renderer.get_sprites());
        engine.engrave();
        edit_cursor = engine.get_cursor();
        on_resize();
    };
    
    // TODO: otherwise load document from file
}

// the document size changed (-> reengrave, reallocate cache and redraw)
void Controller::on_resize()
{
    engine.reengrave();
    view.on_resize();
}

// the document instance changed (-> reengrave and redraw)
void Controller::reengrave()
{
    engine.reengrave();
    view.rerender();
}

// zoom handler
void Controller::on_zoom_changed(const unsigned int zoom)
{
    engine.get_press_parameters().scale = zoom; // set zoom parameter
    renderer.clear_cache();                     // clear renderer cache
    view.on_resize();                           // re-center view
}

// render edit cursor
void Controller::render_edit_cursor(const Offset& offset)
{
    if (edit_cursor)
        engine.render_cursor(renderer, *edit_cursor, layout, offset);
}

// render object cursors
void Controller::render_object_cursors(const Offset& offset)
{
    for (ObjectList::iterator cur = object_cursors.begin(); cur != object_cursors.end(); ++cur)
        engine.render_cursor(renderer, **cur, layout, offset);
}

// logging control
void Controller::log_set(ScorePress::Log& log)
{
    this->ScorePress::Logging::log_set(log);
    engine.log_set(log);
}

void Controller::log_unset()
{
    this->ScorePress::Logging::log_unset();
    engine.log_unset();
}

