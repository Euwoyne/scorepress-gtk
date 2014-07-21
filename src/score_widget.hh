
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

#ifndef SCOREWIDGET_HH
#define SCOREWIDGET_HH

#define GTK_DISABLE_DEPRECATED 1
#include <gtkmm.h>
#include <cairomm/surface.h>
#include "controller.hh"

class ScoreWidget : public Gtk::DrawingArea
{
 private:
    struct MoveInfo
    {
        gdouble                                 x, y;   // start of movement
        bool                                    rdy;    // move ready
        bool                                    ack;    // move acknowledged
        ScorePress::Position<ScorePress::mpx_t> offset; // current position offset
    };
    
 private:
    Controller*  controller;    // corresponding controller
    clock_t      t;             // timing reference point
    MoveInfo     move;          // object move info
    
 public:
    ScorePress::Position<ScorePress::mpx_t> margin; // margin around the page (i.e. minimal offset)
    ScorePress::Position<ScorePress::mpx_t> offset; // actual offset (after centering)
    
 public:
    ScoreWidget(Controller& ctrl);      // constructor
    
    Controller& get_controller();       // return the corresponding controller
    
    void refresh();                     // redraw the widget
    void center(unsigned int width);    // center the score on the widget
    
    // signal handlers
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& context);
    bool on_blink();
    void on_score_resize();
    
    bool on_button_press(GdkEventButton* event);
    bool on_button_release(GdkEventButton* event);
    bool on_motion_notify(GdkEventMotion* event);
    
    bool on_key_press(GdkEventKey* event);
    bool on_key_release(GdkEventKey* event);
};


// inline method implementations
#include "controller.hh"

inline Controller& ScoreWidget::get_controller()  {return *controller;}
inline void        ScoreWidget::refresh()         {get_window()->invalidate(false);}
inline void        ScoreWidget::on_score_resize() {center(get_allocated_width());}

#endif

