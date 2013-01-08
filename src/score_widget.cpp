
/*
  ScorePress - Music Engraving Software  (scorepress-gtk)
  Copyright (C) 2013 Dominik Lehmann
  
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
#include <renderer.hh>
#include <context.hh>
#include <press.hh>

#include "score_widget.hh"
#include "configuration.hh"

// constructor
ScoreWidget::ScoreWidget(Controller& ctrl) : controller(&ctrl),
                                             margin(30000, 40000)
{
    offset = margin;
    this->set_can_focus(true);
    this->set_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
    this->signal_button_press_event().connect(sigc::mem_fun(*this, &ScoreWidget::on_button_press), false);
    this->signal_button_release_event().connect(sigc::mem_fun(*this, &ScoreWidget::on_button_release), false);
    this->signal_draw().connect(sigc::mem_fun(*this, &ScoreWidget::on_draw), false);
    this->signal_key_press_event().connect(sigc::mem_fun(*this, &ScoreWidget::on_key_press));
    this->signal_key_release_event().connect(sigc::mem_fun(*this, &ScoreWidget::on_key_release));
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &ScoreWidget::on_blink), 500);
    set_size_request((controller->get_engine().page_width()  + 2 * offset.x) / 1000.0,
                     (controller->get_engine().page_height() + 2 * offset.y) / 1000.0);
}

// center the score on the widget
void ScoreWidget::center(unsigned int width)
{
    if (width * 1000.0 < controller->get_engine().page_width()) offset.x = margin.x;
    else offset.x = ((width - 2) * 1000.0 - controller->get_engine().page_width()) / 2.0;
    set_size_request((controller->get_engine().page_width()  + 2 * offset.x) / 1000.0,
                     (controller->get_engine().page_height() + 2 * offset.y) / 1000.0);
}

bool ScoreWidget::on_button_press(GdkEventButton* event)
{
    try
    {
        controller->mouse_on((event->x * 1000 - offset.x) * 1000, (event->y * 1000 - offset.y) * 1000);
    }
    catch (std::string& s)
    {
        std::cerr << "Exception caught in signal handler (mouse on): " << s << "\n";
    };
    return true;
}

bool ScoreWidget::on_button_release(GdkEventButton* event)
{
    try
    {
        controller->mouse_off((event->x * 1000 - offset.x) * 1000, (event->y * 1000 - offset.y) * 1000);
    }
    catch (std::string& s)
    {
        std::cerr << "Exception caught in signal handler (mouse off): " << s << "\n";
    };
    return true;
}

bool ScoreWidget::on_key_press(GdkEventKey* event)
{
    try
    {
        controller->key_press(KeyListener::Key(event->keyval, event->state & GDK_CONTROL_MASK));
        this->get_window()->invalidate(false);
        if (event->keyval == GDK_KEY_q) controller->get_engine().cursor.get_platenote().dump();
        if (event->keyval == GDK_KEY_Q) controller->get_engine().plate_dump();
        if (event->keyval == GDK_KEY_w) controller->get_engine().cursor.dump();
    }
    catch (std::string& s)
    {
        std::cerr << "Exception caught in signal handler (key press): " << s << "\n";
    };
    return true;
}

bool ScoreWidget::on_key_release(GdkEventKey* event)
{
    try
    {
        controller->key_release(KeyListener::Key(event->keyval, event->state & GDK_CONTROL_MASK));
        this->get_window()->invalidate(false);
    }
    catch (std::string s)
    {
        std::cerr << "Exception caught in signal handler (key release): " << s << "\n";
    };
    return true;
}

bool ScoreWidget::on_blink()
{
    try
    {
        const Cairo::RefPtr<Cairo::Context>& drawingCtx = this->get_window()->create_cairo_context();
        drawingCtx->set_antialias(Cairo::ANTIALIAS_NONE);
        controller->get_renderer().begin(drawingCtx->cobj());
        controller->get_engine().render_cursor(controller->get_renderer(), offset);
        controller->get_renderer().end();
    }
    catch (std::string s)
    {
        std::cerr << "Exception caught in signal handler (blink): " << s << "\n";
    };
    return true;
}

bool ScoreWidget::on_draw(const Cairo::RefPtr<Cairo::Context>& drawingCtx)
{
    try
    {
        controller->get_renderer().begin(drawingCtx->cobj());
        drawingCtx->set_antialias(Cairo::ANTIALIAS_SUBPIXEL);
        controller->get_engine().render(controller->get_renderer(), offset);
        drawingCtx->set_antialias(Cairo::ANTIALIAS_NONE);
        controller->get_engine().render_cursor(controller->get_renderer(), offset);
        controller->get_renderer().end();
    }
    catch (ScorePress::StaffContext::Error e)
    {
        std::cerr << "Exception caught in signal handler (draw): " << e << " (class: Context)\n";
    }
    catch (ScorePress::Press::Error e)
    {
        std::cerr << "Exception caught in signal handler (draw): " << e << " (class: Press)\n";
    }
    catch (std::string s)
    {
        std::cerr << "Exception caught in signal handler (draw): " << s << "\n";
    };
    return true;
}

