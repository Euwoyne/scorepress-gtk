
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

#ifndef RSVG_RENDERER_HH
#define RSVG_RENDERER_HH

#include <vector>                    // std::vector
#include <librsvg/rsvg.h>            // RsvgHandle
#ifndef RSVG_CAIRO_H
#   include <librsvg/rsvg-cairo.h>   // cairo_t
#endif
#include <pango/pangocairo.h>        // PangoLayout, PangoFontDescription

#include <scorepress/renderer.hh>    // Renderer, std::string, std::map
#include <scorepress/file_format.hh> // SpritesetReader

// libxml2 prototype
struct _xmlTextReader;

//
//     class RSVGRenderer 
//    ====================
//
class RSVGRenderer : public ScorePress::Renderer
{
 private:
    // SVG spriteset parser
    class RSVGSpritesetReader : public ScorePress::SpritesetReader
    {
     private:
        _xmlTextReader* parser;
        RsvgHandle*     lib;
        std::string     filename;
        
     public:
        // constructor
        RSVGSpritesetReader();
        
        // virtual parser interface
        virtual void open(const char* data, const std::string& filename);   // use memory for reading
        virtual void open(const std::string& filename);     // open file for reading
        virtual void close();                               // close file
        
        virtual bool is_open() const;                       // check if a file is opened
        virtual const char* get_filename() const;           // return the filename (or NULL)
        
        virtual void parse_spriteset(ScorePress::SpriteSet&   target,   // sprite-set parser
                                     ScorePress::Renderer&    renderer,
                                     const size_t setid);
    };
    friend class RSVGSpritesetReader;
    
    // image cache
#ifdef CAIRO_HAS_IMAGE_SURFACE
    class CacheKey
    {
     public:
        ScorePress::SpriteId sprite_id;
        int xscale;
        int yscale;
        
        CacheKey(ScorePress::SpriteId sid, int xs, int ys) : sprite_id(sid), xscale(xs), yscale(ys) {};
        
        bool operator < (const CacheKey& key) const
        {
            return                                           (sprite_id.setid    < key.sprite_id.setid    ||
            (sprite_id.setid    == key.sprite_id.setid    && (sprite_id.spriteid < key.sprite_id.spriteid ||
            (sprite_id.spriteid == key.sprite_id.spriteid && (xscale             < key.xscale             ||
            (xscale             == key.xscale             &&  yscale             < key.yscale     ))))));
        };
    };
    
    std::map<CacheKey, cairo_surface_t*> cache;
    bool use_cache;
#else
    static const bool use_cache = false;
#endif
    
    // RGB font color structure
    struct FontColor {unsigned char r, g, b;};
    
 private:
    std::vector<RsvgHandle*> libs;  // handles for the loaded sprite-set SVG files
    
    RSVGSpritesetReader filereader; // spriteset reader instance
    
    cairo_t*       drawingCtx;      // target drawing context
    unsigned int   clipped;         // number of "clip" calls
    
    PangoLayout*   text_layout;     // text layout
    PangoAttrList* text_attributes; // text attributes
    std::string    text;            // complete text to be rendered
    
    PangoFontDescription* font;             // temporary font description (pushed to the attributes, when the text is added)
    bool                  font_underline;   // temporary font description (underline property)
    FontColor             font_color;       // temporary font description (color property)
    
 private:   // helper function (common code for both "load" methods)
    void load(_xmlTextReader* parser, RsvgHandle* lib, const std::string& filename);
    
 public:
    RSVGRenderer(cairo_t* drawingCtx = NULL);   // constructor
    
    void begin(cairo_t* drawingCtx);            // initialize rendering on given context
    void end();                                 // finalize rendering on current context
    void unload_set(const size_t setid);        // remove sprite-set from sprites collection
    
    void clear_cache();                         // erase image cache
    void clear_cache(const size_t setid);       // erase sprites of given set from cache
    bool enable_cache(bool enable);             // enable/disable caching of image data (return actual status)
    bool has_cache() {return use_cache;};       // return, if the cache is used
    
    ~RSVGRenderer();                            // destructor
    
 public:
    // renderer methods (to be implemented by actual renderer)
    virtual bool   ready() const;                                           // is the object ready to render?
    virtual bool   exist(const std::string& path) const;                    // does the path exist?
    virtual bool   exist(const std::string& path, const size_t setid) const;
    
    // sprite-set readers
    typedef ScorePress::SpritesetReader SpritesetReader;
    
    virtual size_t    spriteset_format_count() const;           // number of supported formats
    virtual ReaderPtr spriteset_reader(const size_t idx = 0);   // get file-reader for spriteset
    virtual size_t    add_spriteset(ReaderPtr reader);          // read new spriteset from reader (returns index)
    
    // sprite rendering
    virtual void draw_sprite(const ScorePress::SpriteId sprite_id, double x, double y);
    virtual void draw_sprite(const ScorePress::SpriteId sprite_id, double x, double y, double xscale, double yscale);
    
    // basic rendering
    virtual void set_line_width(const double width);        // set width of following lines
    virtual void set_color(const unsigned char r,           // set the current foreground color
                           const unsigned char g,
                           const unsigned char b,
                           const unsigned char a);
    virtual void move_to(const double x, const double y);       // move cursor
    virtual void line_to(const double x, const double y);       // draw line
    virtual void fill();                        // fill the drawn object
    virtual void stroke();                      // render the drawn object
    virtual void close();                       // close the drawn object
    
    // clipping
    virtual void clip(const int x1, const int y1, const int w, const int h);    // set rectangle clipping
    virtual void unclip();                                                      // reset the last "clip" call
    
    // text rendering
    virtual void set_font_family(const std::string& family); // set the font family
    virtual void set_font_size(const double pt);             // set the font size (in pt)
    virtual void set_font_bold(const bool bold);             // set, if the font is bold
    virtual void set_font_italic(const bool italic);         // set, if the font is italic
    virtual void set_font_underline(const bool underline);   // set, if the font is underlined
    virtual void set_font_color(const unsigned char r,       // set the font color
                                const unsigned char g,
                                const unsigned char b);
    
    virtual void set_text_width(const double width);       // set the textbox width
    virtual void reset_text_width();                       // reset the textbox to the whole drawing area
    virtual void set_text_align(const enuAlignment align); // set the alignment
    virtual void set_text_justify(const bool justify);     // enable/disable justification
    virtual void add_text(const std::string& utf8);        // add a string to the paragraph
    virtual void render_text();                            // render the prepared text
    
    // advanced rendering
    virtual void rect_invert(double x1, double y1, double x2, double y2);   // invert the given rectangle
    virtual bool has_rect_invert() const {return true;};                    // check, if the invert method is available
    
    // cubic bézier algorithm
    virtual void bezier(double  x1, double  y1,         // render a cubic bézier curve
                        double cx1, double cy1,
                        double cx2, double cy2,
                        double  x2, double  y2);
    
    virtual void bezier_slur(double  x1, double  y1,    // render a cubic bézier curve with different line width
                            double cx1, double cy1,     //         in the center than at the ends
                            double cx2, double cy2,
                            double  x2, double  y2,
                            double  w0, double  w1);
};

#endif

