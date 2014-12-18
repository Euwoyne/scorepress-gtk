
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

#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <scorepress/engine.hh> // Engine, Document, MultipageLayout
#include <giomm/file.h>         // Gio::File, Glib::RefPtr
#include "view.hh"              // View
#include "rsvg_renderer.hh"     // RsvgRenderer
#include "key_listener.hh"      // KeyListener

//
//     class Controller
//    ==================
//
// Receives signals from the "KeyListener" instance and signal handlers within
// the "View" instances (i.e. "MainWnd" and "ScoreWidget").
// Responsible for updating the "Document" and "Engine" instances and
// refreshing the displaying instance and its cache.
//
class Controller : public ScorePress::Logging
{
 public:
    // convenience typedefs
    typedef ScorePress::mpx_t                            mpx_t;
    typedef ScorePress::Position<ScorePress::mpx_t>      Position;
    typedef ScorePress::Position<ScorePress::mpx_t>      Offset;
    typedef ScorePress::RefPtr<ScorePress::EditCursor>   CursorPtr;
    typedef ScorePress::RefPtr<ScorePress::ObjectCursor> ObjectPtr;
    typedef std::list<ObjectPtr>                         ObjectList;
    
    
 private:
    // data instances
    ScorePress::Document         document;      // target document instance
    ScorePress::Engine           engine;        // engine operating on the document
    ScorePress::MultipageLayout  layout;        // multipage layout for rendering
    
    // rendering/controlling instances
    CursorPtr    edit_cursor;       // edit cursor
    ObjectList   object_cursors;    // object cursor
    View         view;              // view instance (target widget)
    RSVGRenderer renderer;          // renderer
    KeyListener& keys;              // key listener (normally hosted by Application instance)
    
    // file information
    std::string  basename;          // the file's basename
    std::string  filepath;          // complete filename (with path)
    bool         changed;           // file is changed?
    
    // copy constructor
    Controller(const Controller&);  // hide copy constructor
    
 public:
    // constructor (with target window and key-listener)
    Controller(MainWnd& window, KeyListener& keys);
    virtual ~Controller();
    
    bool has_cursor()        const;
    bool has_object_cursor() const;
    
    // data access
    ScorePress::Document&   get_document();         // document instance
    ScorePress::Engine&     get_engine();           // engine instance
    ScorePress::EditCursor& get_cursor();           // main cursor instance
    RSVGRenderer&           get_renderer();         // renderer instance
    MainWnd&                get_window();           // parent window
    
    // document status
    const std::string&      get_basename() const;   // basename  (set on create)
    const std::string&      get_filepath() const;   // file path (set on save)
    bool                    is_new()       const;   // never saved (i.e. empty path)
    bool                    is_changed()   const;   // changed after creation?
    
    // file control
    void open(Glib::RefPtr<Gio::File>);             // open document-file (if NULL, load test-document)
    void set_basename(const std::string& s);        // sets the documents basename
    void set_filepath(const std::string& s);        // sets the documents source path
    void change(bool b = true);                     // mark the document as changed (or unchanged)
    
    // signal handler
        // (from KeyListener)
    void on_resize();   // the document size changed  (-> allocate cache, engrave and render)
    void reengrave();   // the document data changed  (->                 engrave and render)
    void rerender();    // the plate instance changed (->                             render)
    void redraw();      // the cursors changed        (-> only refresh view)
    
        // (from MainWnd)
    void on_zoom_changed(const unsigned int);   // the zoom changed (-> redraw)
    
        // (from ScoreWidget)
    bool on_key_press(  const KeyMap::Key);     // key press signal
    bool on_key_release(const KeyMap::Key);     // key release signal
    bool on_mouse_press(const Position&);       // mouse click signal
    
    // page layout
    ScorePress::mpx_t layout_width()  const;    // width of complete layout
    ScorePress::mpx_t layout_height() const;    // height of complete layout
    
    // rendering
    void render_document(      const Offset& offset, bool decor = true);    // all pages (with layout)
    void render_edit_cursor(   const Offset& offset);                       // edit cursor
    void render_object_cursors(const Offset& offset);                       // object cursors
    
    // logging control
    using Logging::log_set;
    void log_set(ScorePress::Log& log);         // set log instance
    void log_unset();                           // unset log instance
};

// inline methods
inline bool Controller::has_cursor()        const {return edit_cursor;}
inline bool Controller::has_object_cursor() const {return !object_cursors.empty();}

// inline methods (data access)
inline ScorePress::Document&   Controller::get_document()       {return document;}
inline ScorePress::Engine&     Controller::get_engine()         {return engine;}
inline ScorePress::EditCursor& Controller::get_cursor()         {return *edit_cursor;}
inline RSVGRenderer&           Controller::get_renderer()       {return renderer;}
inline MainWnd&                Controller::get_window()         {return view.get_window();}

// inline methods (document status)
inline const std::string&      Controller::get_basename() const {return basename;}
inline const std::string&      Controller::get_filepath() const {return filepath;}
inline bool                    Controller::is_new()       const {return filepath.empty();}
inline bool                    Controller::is_changed()   const {return changed;}

// inline methods (file control)
inline void Controller::set_basename(const std::string& s)  {basename = s; view.on_title_changed();}
inline void Controller::set_filepath(const std::string& s)  {filepath = s;}
inline void Controller::change(bool b)                      {changed  = b; view.on_title_changed();}

// inline methods (signal handler)
inline void Controller::rerender()                          {view.rerender();}
inline void Controller::redraw()                            {view.redraw();}
inline bool Controller::on_key_press(  const KeyMap::Key k) {return keys.press(k, *this);}
inline bool Controller::on_key_release(const KeyMap::Key k) {return keys.release(k);}
inline bool Controller::on_mouse_press(const Position& pos) {engine.set_cursor(edit_cursor, pos, layout); redraw(); return false;}

// inline methods (page layout)
inline ScorePress::mpx_t Controller::layout_width()  const  {return engine.layout_width(layout);}
inline ScorePress::mpx_t Controller::layout_height() const  {return engine.layout_height(layout);}

// inline methods (rendering)
inline void Controller::render_document(const Offset& offset, bool decor)
{
    engine.render_all(renderer, layout, offset, decor);
}

#endif

