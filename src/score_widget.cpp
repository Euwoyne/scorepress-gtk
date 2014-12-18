
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

#include <iostream>
#include <scorepress/renderer.hh>
#include <scorepress/context.hh>
#include <scorepress/press.hh>

#include "score_widget.hh"
#include "controller.hh"

#if 0
#define PRINT_CLOCK(s) \
    std::cout << s << static_cast<float>(clock() - t) / CLOCKS_PER_SEC << "s\n"; \
    t = clock();
#else
#define PRINT_CLOCK(s)
#endif

#define CATCH_ERRORS(NAME) \
    catch (ScorePress::Engine::Error& s) { \
        std::cerr << "[ERROR] ScoreWidget::" NAME "(): " << s << " (Engine Error)\n"; \
    } \
    catch (ScorePress::UserCursor::Error& s) { \
        std::cerr << "[ERROR] ScoreWidget::" NAME "(): " << s << " (User Cursor Error)\n"; \
    } \
    catch (ScorePress::Cursor::Error& s) { \
        std::cerr << "[ERROR] ScoreWidget::" NAME "(): " << s << " (Cursor Error)\n"; \
    } \
    catch (ScorePress::FileReader::Error& s) { \
        std::cerr << "[ERROR] ScoreWidget::" NAME "(): " << s << " (File Reader Error)\n"; \
    } \
    catch (ScorePress::Press::Error& s) { \
        std::cerr << "[ERROR] ScoreWidget::" NAME "(): " << s << " (Press Error)\n"; \
    } \
    catch (ScorePress::StaffContext::Error& s) { \
        std::cerr << "[ERROR] ScoreWidget::" NAME "(): " << s << " (Context Error)\n"; \
    } \
    catch (ScorePress::Error& s) { \
        std::cerr << "[ERROR] ScoreWidget::" NAME "(): " << s << "\n"; \
    } \
    catch (std::exception& e) { \
        std::cerr << "[ERROR] ScoreWidget::" NAME "(): " << e.what() << "\n"; \
    } \
    catch (...) { \
        std::cerr << "[ERROR] ScoreWidget::" NAME "(): Unknown Exception\n"; \
    }

inline static int _round(const double d) {return static_cast<int>(d + 0.5);}

// constructor
ScoreWidget::ScoreWidget(Controller& ctrl) : controller(ctrl),
                                             t(clock()),
                                             cache_valid(false),
                                             margin(30000, 40000),
                                             offset(margin),
                                             min_move(10)
{
    move.rdy = false;
    
    this->set_can_focus(true);
    this->set_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::BUTTON1_MOTION_MASK);
    this->signal_button_press_event().connect(sigc::mem_fun(*this, &ScoreWidget::on_button_press), false);
    this->signal_button_release_event().connect(sigc::mem_fun(*this, &ScoreWidget::on_button_release), false);
    this->signal_draw().connect(sigc::mem_fun(*this, &ScoreWidget::on_draw), false);
    this->signal_key_press_event().connect(sigc::mem_fun(*this, &ScoreWidget::on_key_press));
    this->signal_key_release_event().connect(sigc::mem_fun(*this, &ScoreWidget::on_key_release));
    this->signal_motion_notify_event().connect(sigc::mem_fun(*this, &ScoreWidget::on_motion_notify));
    Glib::signal_timeout().connect_seconds(sigc::mem_fun(*this, &ScoreWidget::on_blink), 1);
    
    set_size_request(controller.layout_width()  / 1000 + offset.x / 500,
                     controller.layout_height() / 1000 + offset.y / 500);
    
    cache = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32,
                                        controller.layout_width()  / 1000,
                                        controller.layout_height() / 1000);
    
    cache_pattern = Cairo::SurfacePattern::create(cache);
}

// center the score on the widget
void ScoreWidget::center(unsigned int width, bool layout_changed)
{
    // calculate center offset
    offset.x = 500 * width - controller.layout_width() / 2;
    
    // center view if necessary
    if (offset.x < margin.x)    // if the view does not fit
    {
        // use default margin and request scrolling
        offset.x = margin.x;
        set_size_request(controller.layout_width()  / 1000 + offset.x / 500,
                         controller.layout_height() / 1000 + offset.y / 500);
    }
    else                        // if it fits, center
    {
        set_size_request(0, controller.layout_height() / 1000 + offset.y / 500);
    };
    
    // resize cache
    if (layout_changed)
    {
        cache = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32,
                                            controller.layout_width()  / 1000,
                                            controller.layout_height() / 1000);
        
        cache_pattern = Cairo::SurfacePattern::create(cache);
        cache_valid = false;
    };
}

// render the score to the cache
void ScoreWidget::redraw_cache()
{
    PRINT_CLOCK("redraw_cache: in  ");
    try
    {
        Cairo::RefPtr<Cairo::Context> drawingCtx = Cairo::Context::create(cache);
        controller.get_renderer().begin(drawingCtx->cobj());
        drawingCtx->set_antialias(Cairo::ANTIALIAS_SUBPIXEL);
        controller.render_document(Offset());
        controller.get_renderer().end();
        cache_valid = true;
    }
    CATCH_ERRORS("redraw_cache");
    PRINT_CLOCK("redraw_cache: out ");
}

bool ScoreWidget::on_draw(const Cairo::RefPtr<Cairo::Context>& drawingCtx)
{
    PRINT_CLOCK("on_draw: in  ");
    try
    {
        if (!cache_valid) redraw_cache();
        drawingCtx->set_antialias(Cairo::ANTIALIAS_NONE);
        drawingCtx->save();
        drawingCtx->translate(offset.x / 1000.0, offset.y / 1000.0);
        drawingCtx->set_source(cache_pattern);
        drawingCtx->paint();
        drawingCtx->restore();
        controller.get_renderer().begin(drawingCtx->cobj());
        controller.render_edit_cursor(offset);
        //drawingCtx->set_antialias(Cairo::ANTIALIAS_SUBPIXEL);
        //if (move.ack)
        //    controller.render_selected(offset, move.offset);
        //controller.render_selection(offset, move.offset);
        //drawingCtx->set_antialias(Cairo::ANTIALIAS_NONE);
        controller.get_renderer().end();
    }
    CATCH_ERRORS("on_draw");
    PRINT_CLOCK("on_draw: out ");
    return true;
}

bool ScoreWidget::on_blink()
{
    try
    {
        const Glib::RefPtr<Gdk::Window> wnd = this->get_window();
        if (!wnd) return true;
        const Cairo::RefPtr<Cairo::Context>& drawingCtx = wnd->create_cairo_context();
        drawingCtx->set_antialias(Cairo::ANTIALIAS_NONE);
        controller.get_renderer().begin(drawingCtx->cobj());
        controller.render_edit_cursor(offset);
        controller.get_renderer().end();
    }
    CATCH_ERRORS("on_blink");
    return true;
}

bool ScoreWidget::on_button_press(GdkEventButton* evnt)
{
    PRINT_CLOCK("on_button_press: in  ");
    try
    {
        controller.on_mouse_press(Controller::Position(static_cast<ScorePress::mpx_t>(evnt->x * 1000 - offset.x + .5), static_cast<ScorePress::mpx_t>(evnt->y * 1000 - offset.y + .5)));
        /*
        if (controller.on_mouse_press(Controller::Position(evnt->x * 1000 - offset.x, evnt->y * 1000 - offset.y)))
        {
            move.x = evnt->x;
            move.y = evnt->y;
            move.rdy = true;
            move.ack = false;
            move.offset.x = move.offset.y = 0;
        };
        */
    }
    CATCH_ERRORS("on_button_press");
    PRINT_CLOCK("on_button_press: out ");
    return true;
}

bool ScoreWidget::on_button_release(GdkEventButton*)
{
    /*
    PRINT_CLOCK("on_button_release: in  ");
    try
    {
        controller.mouse_off(evnt->x * 1000 - offset.x, evnt->y * 1000 - offset.y);
        if (move.ack)
        {
            move.offset.x = _round((evnt->x - move.x) * 1000);
            move.offset.y = _round((evnt->y - move.y) * 1000);
            if (controller.get_engine().has_selected_object())
                controller.get_engine().move_object(controller.get_engine().selected_object(), move.offset);
            move.rdy = false;
            move.ack = false;
            move.offset.x = 0;
            move.offset.y = 0;
            cache_valid = false;
            this->get_window()->invalidate(false);
        };
    }
    CATCH_ERRORS("on_button_release");
    PRINT_CLOCK("on_button_release: out ");
    */
    return true;
}

bool ScoreWidget::on_motion_notify(GdkEventMotion* evnt)
{
    if (!move.rdy) return true;
    try
    {
        if (!move.ack)
            if (   move.x - evnt->x > min_move || move.x - evnt->x < -min_move
                || move.y - evnt->y > min_move || move.y - evnt->y < -min_move)
                    move.ack = true;
        if (move.ack)
        {
            move.offset.x = _round((evnt->x - move.x) * 1000);
            move.offset.y = _round((evnt->y - move.y) * 1000);
            this->get_window()->invalidate(false);
        };
    }
    CATCH_ERRORS("on_motion_notify");
    return true;
}

bool ScoreWidget::on_key_press(GdkEventKey* evnt)
{
    PRINT_CLOCK("on_key_press: in  ");
    try
    {
        if (move.rdy && evnt->keyval == GDK_KEY_Escape) move.rdy = false;
        else if (controller.on_key_press(KeyListener::Key(evnt->keyval, evnt->state & GDK_CONTROL_MASK)));
        else if (evnt->keyval == GDK_KEY_W)
            controller.get_engine().plate_dump();
        else if (evnt->keyval == GDK_KEY_q && controller.has_cursor())
            controller.get_cursor().dump();
        else if (evnt->keyval == GDK_KEY_w && controller.has_cursor() && !controller.get_cursor().at_end())
            controller.get_cursor().get_platenote().dump();
        this->get_window()->invalidate(false);
    }
    CATCH_ERRORS("on_key_press");
    PRINT_CLOCK("on_key_press: out ");
    return true;
}

bool ScoreWidget::on_key_release(GdkEventKey* evnt)
{
    PRINT_CLOCK("on_key_release: in  ");
    try
    {
        controller.on_key_release(KeyListener::Key(evnt->keyval, true));
        controller.on_key_release(KeyListener::Key(evnt->keyval, false));
        this->get_window()->invalidate(false);
    }
    CATCH_ERRORS("on_key_release");
    PRINT_CLOCK("on_key_release: out ");
    return true;
}

