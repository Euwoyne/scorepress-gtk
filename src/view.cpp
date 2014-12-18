
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

#include "view.hh"
#include "mainwnd.hh"
#include "controller.hh"
#include <iostream>

// constructor
View::View(Controller& ctrl, MainWnd& wnd) : controller(ctrl), window(&wnd), widget(ctrl)
{
    scrollWnd.add(widget);
    widget.show();
    scrollWnd.show();
    scrollWnd.get_child()->signal_size_allocate().connect(sigc::mem_fun(*this, &View::on_size_changed), false);
}

// copy constructor
View::View(const View& view) : controller(view.controller), window(view.window), widget(view.controller)
{
    scrollWnd.add(widget);
    widget.show();
    scrollWnd.show();
    scrollWnd.get_child()->signal_size_allocate().connect(sigc::mem_fun(*this, &View::on_size_changed), false);
}

// destructor
View::~View()
{
    window->close_view(*this);
}

// document size changed  (reallocate and redraw cache)
void View::on_resize()
{
    widget.center(scrollWnd.get_child()->get_allocated_width(), true);  // re-center view / request re-render
    widget.get_window()->invalidate(false);                             // request widget redraw
}

// document data changed (redraw cache)
void View::rerender()
{
    widget.invalidate_cache();              // request score re-render
    widget.get_window()->invalidate(false); // request widget redraw
}

// view changed (refresh widget)
void View::redraw()
{
    widget.get_window()->invalidate(false); // just redraw the widget
}

// status bar content changed (refresh status bar)
void View::on_status_changed()
{
    // TODO: update status bar (in MainWnd)
}

// resize signal handler
void View::on_size_changed(Gtk::Allocation& alloc)
{
    widget.center(alloc.get_width(), false);    // re-center view
    widget.get_window()->invalidate(false);     // request widget redraw
}

