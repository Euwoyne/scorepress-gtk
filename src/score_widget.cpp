
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

#include <iostream>
#include <scorepress/renderer.hh>
#include <scorepress/context.hh>
#include <scorepress/press.hh>

#include "score_widget.hh"

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
ScoreWidget::ScoreWidget(Controller& ctrl) : controller(&ctrl),
                                             t(clock()),
                                             margin(30000, 40000),
                                             offset(margin)
                                             
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
    set_size_request(controller->layout_width()  / 1000 + offset.x / 500,
                     controller->layout_height() / 1000 + offset.y / 500);
}

// center the score on the widget
void ScoreWidget::center(unsigned int width)
{
    offset.x = 500 * width - controller->layout_width() / 2;
    if (offset.x < margin.x)
    {
        offset.x = margin.x;
        set_size_request(controller->layout_width()  / 1000 + offset.x / 500,
                         controller->layout_height() / 1000 + offset.y / 500);
    }
    else
    {
        set_size_request(0, controller->layout_height() / 1000 + offset.y / 500);
    };
}

bool ScoreWidget::on_draw(const Cairo::RefPtr<Cairo::Context>& drawingCtx)
{
    PRINT_CLOCK("on_draw: in  ");
    try
    {
        controller->get_renderer().begin(drawingCtx->cobj());
        drawingCtx->set_antialias(Cairo::ANTIALIAS_SUBPIXEL);
        controller->render_document(offset);
        if (move.ack)
            controller->render_selected(offset, move.offset);
        controller->render_selection(offset, move.offset);
        drawingCtx->set_antialias(Cairo::ANTIALIAS_NONE);
        controller->render_cursor(offset);
        controller->get_renderer().end();
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
        controller->get_renderer().begin(drawingCtx->cobj());
        controller->render_cursor(offset);
        controller->get_renderer().end();
    }
    CATCH_ERRORS("on_blink");
    return true;
}

bool ScoreWidget::on_button_press(GdkEventButton* evnt)
{
    PRINT_CLOCK("on_button_press: in  ");
    try
    {
        if (controller->mouse_on(evnt->x * 1000 - offset.x, evnt->y * 1000 - offset.y))
        {
            move.x = evnt->x;
            move.y = evnt->y;
            move.rdy = true;
            move.ack = false;
            move.offset.x = move.offset.y = 0;
        };
    }
    CATCH_ERRORS("on_button_press");
    PRINT_CLOCK("on_button_press: out ");
    return true;
}

bool ScoreWidget::on_button_release(GdkEventButton* evnt)
{
    PRINT_CLOCK("on_button_release: in  ");
    try
    {
        controller->mouse_off(evnt->x * 1000 - offset.x, evnt->y * 1000 - offset.y);
        if (move.ack)
        {
            move.offset.x = _round((evnt->x - move.x) * 1000);
            move.offset.y = _round((evnt->y - move.y) * 1000);
            if (controller->get_engine().has_selected_object())
                controller->get_engine().move_object(controller->get_engine().selected_object(), move.offset);
            move.rdy = false;
            move.ack = false;
            move.offset.x = 0;
            move.offset.y = 0;
            this->get_window()->invalidate(false);
        };
    }
    CATCH_ERRORS("on_button_release");
    PRINT_CLOCK("on_button_release: out ");
    return true;
}

bool ScoreWidget::on_motion_notify(GdkEventMotion *evnt)
{
    if (!move.rdy) return true;
    try
    {
        if (!move.ack)
            if (   move.x - evnt->x > 10.0 || move.x - evnt->x < -10.0
                || move.y - evnt->y > 10.0 || move.y - evnt->y < -10.0)
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
        else controller->key_press(KeyListener::Key(evnt->keyval, evnt->state & GDK_CONTROL_MASK));
        if (evnt->keyval == GDK_KEY_q) controller->get_cursor().get_platenote().dump();
        if (evnt->keyval == GDK_KEY_Q) controller->get_engine().plate_dump();
        if (evnt->keyval == GDK_KEY_w) controller->get_cursor().dump();
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
        controller->key_release(KeyListener::Key(evnt->keyval, true));
        controller->key_release(KeyListener::Key(evnt->keyval, false));
        if (evnt->state & GDK_SHIFT_MASK)
        {
            controller->key_release(KeyListener::Key(GDK_KEY_Shift_R, true));
            controller->key_release(KeyListener::Key(GDK_KEY_Shift_R, false));
            controller->key_release(KeyListener::Key(GDK_KEY_Shift_L, true));
            controller->key_release(KeyListener::Key(GDK_KEY_Shift_L, false));
        };
        this->get_window()->invalidate(false);
    }
    CATCH_ERRORS("on_key_release");
    PRINT_CLOCK("on_key_release: out ");
    return true;
}

