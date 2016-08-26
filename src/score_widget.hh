
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

#ifndef SCOREWIDGET_HH
#define SCOREWIDGET_HH

#define GTK_DISABLE_DEPRECATED 1
#include <scorepress/basetypes.hh>
#include <gtkmm.h>
#include <cairomm/surface.h>
#include <cairo.h>

class Controller;
class ScoreWidget : public Gtk::DrawingArea
{
 public:
    typedef ScorePress::Position<ScorePress::mpx_t> Offset;
    
 private:
    // move info struct
    struct MoveInfo
    {
        gdouble x, y;   // start of movement
        bool    rdy;    // move ready
        bool    ack;    // move acknowledged
        Offset  offset; // current position offset
    };
    
 private:
    Controller& controller; // corresponding controller
    clock_t     t;          // timing reference point
    MoveInfo    move;       // object move info
    
    // view cache
    Cairo::RefPtr<Cairo::Surface>        cache;         // cache surface (rendered score without cursors)
    Cairo::RefPtr<Cairo::SurfacePattern> cache_pattern; // pattern referencing the cache
    bool                                 cache_valid;   // cache validity (mark for rerender)
    
 public:
    // public parameters
    Offset  margin;     // margin around the page (i.e. minimal offset)
    Offset  offset;     // actual offset (after centering)
    int     min_move;   // minimal moving distance
    
 public:
    ScoreWidget(Controller& ctrl);      // constructor
    
    void center(unsigned int width, bool layout_changed);   // center the score on the widget
    
    void redraw_cache();                // render the score to the cache
    void invalidate_cache();            // trigger cache re-render
    
 protected:
    // signal handlers
    bool on_draw(const Cairo::RefPtr<Cairo::Context>&);
    bool on_blink();
    
    bool on_button_press(GdkEventButton* event);
    bool on_button_release(GdkEventButton* event);
    bool on_motion_notify(GdkEventMotion* event);
    
    void translate_key(GdkEventKey* event);
    bool on_key_press(GdkEventKey* event);
    bool on_key_release(GdkEventKey* event);
};

inline void ScoreWidget::invalidate_cache() {cache_valid = false;}

#endif

