
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

#include <scorepress/file_format.hh>    // XMLSpritesetReader
#include <libxml/xmlreader.h>           // xmlReaderForMemory, ...
#include <cstring>                      // strlen
#include <iostream>
#include <limits>

#include "rsvg_renderer.hh"             // RsvgRenderer
#include "i18n.hh"                      // _()

#define STR_CAST(str)    reinterpret_cast<const xmlChar*>(str)
#define XML_CAST(xmlstr) reinterpret_cast<const char*>(xmlstr)

// throwing function (general file error)
static void mythrow(const char* trns, const std::string& filename)
{
    char* msg = new char[strlen(trns) + filename.size() + 1];   // allocate memory
    sprintf(msg, trns, filename.c_str());                       // assemble message
    std::string s(msg);                                         // convert to string
    delete[] msg;                                               // delete buffer
    throw ScorePress::FileReader::Error(s);                     // throw message
}

// SVG spriteset parser
///  SVG Spriteset File Reader
/// ===========================
///
/// File-Type label
RSVGRenderer::RSVGSpritesetReader::RSVGSpritesetReader()
    : ScorePress::FileReader(_("SVG Sprites"), "image/svg+xml", "svg"),
      parser(NULL),
      lib   (NULL) {}

// use memory for reading
void RSVGRenderer::RSVGSpritesetReader::open(const char*        _data,
                                             const std::string& _filename)
{
    // check data length
    const size_t len = strlen(_data);
    if (len > std::numeric_limits<int>::max())
        throw ScorePress::FileReader::IOException(
                "Unable to read SVG sprite-set from memory");
    
    // create XML parser
    if (parser) xmlFreeTextReader(parser);
    parser = xmlReaderForMemory(_data, static_cast<int>(len), _filename.c_str(),
                                NULL,  XML_PARSE_NOBLANKS |
                                       XML_PARSE_NOENT |
                                       XML_PARSE_NONET);
    if (parser == NULL)
        throw ScorePress::FileReader::IOException(
                "Unable to read SVG sprite-set from memory");
    
    // create SVG handle
    if (lib) g_object_unref(lib);
    lib = rsvg_handle_new_from_data(reinterpret_cast<const guint8*>(_data),
                                    strlen(_data), NULL);
    if (lib == NULL)
        throw ScorePress::FileReader::IOException(
                "Unable to read SVG sprite-set from memory");
    
    // save filename
    filename = _filename;
}

// open file for reading
void RSVGRenderer::RSVGSpritesetReader::open(const std::string& _filename)
{
    /// Error message (%s is the file, that could not be opened)
    static const char* message = _("Unable to open SVG sprite-set \"%s\"");
    
    // create XML parser
    if (parser) xmlFreeTextReader(parser);
    parser = xmlReaderForFile(_filename.c_str(), NULL, XML_PARSE_NOBLANKS |
                                                       XML_PARSE_NOENT    |
                                                       XML_PARSE_NONET);
    if (parser == NULL) mythrow(message, _filename);
    
    // create SVG handle
    if (lib) g_object_unref(lib);
    lib = rsvg_handle_new_from_file(_filename.c_str(), NULL);
    if (lib == NULL) mythrow(message, _filename);
    
    // save filename
    filename = _filename;
}

// close file
void RSVGRenderer::RSVGSpritesetReader::close()
{
    if (!parser) return;        // do nothing, of no file is open
    xmlFreeTextReader(parser);  // delete parser instance
    parser = NULL;              // erase parser
}

// check if a file is opened
bool RSVGRenderer::RSVGSpritesetReader::is_open() const
{
    return (parser != NULL);
}

// return the filename (or NULL)
const char* RSVGRenderer::RSVGSpritesetReader::get_filename() const
{
    return (parser ? filename.c_str() : NULL);
}

// virtual parser interface
void RSVGRenderer::RSVGSpritesetReader::parse_spriteset(ScorePress::SpriteSet& target, ScorePress::Renderer& _renderer, const size_t setid)
{
    // check, if a file is open
    if (!parser)
        throw ScorePress::FileReader::Error("FileReader has no open file (please call 'RSVGFileReader::open' first).");
    
    // cast the target renderer
    RSVGRenderer& renderer(static_cast<RSVGRenderer&>(_renderer));
    
    // strip the filename of its path (for the use in error messages)
    const std::string err_file = (filename.rfind('/') != std::string::npos) ?
                                    filename.substr(filename.rfind('/') + 1) :
                                    filename;
    
    // local temp data
    const xmlChar* tag = NULL;  // tag string
    xmlChar* title     = NULL;  // <title> content
    xmlChar* desc      = NULL;  // <desc> content
    
    // get <title> and <desc>
    setlocale(LC_ALL, "C");         // set default locale (for parsing)
    while (xmlTextReaderRead(parser) == 1)
    {
        tag = xmlTextReaderConstName(parser);   // get the current tag
        
        if (xmlTextReaderDepth(parser) == 1 && xmlStrEqual(tag, STR_CAST("title")) == 1 && title == NULL)
            title = xmlNodeGetContent(xmlTextReaderCurrentNode(parser));
        
        if (xmlTextReaderDepth(parser) == 1 && xmlStrEqual(tag, STR_CAST("desc")) == 1 && desc == NULL)
            desc = xmlNodeGetContent(xmlTextReaderCurrentNode(parser));
    };
    setlocale(LC_ALL, "");          // reset locale
    
    // check title
    if (title == NULL)
    {
        if (desc) xmlFree(desc);
        mythrow("Title missing (in file \"%s\")", err_file);
    };
    
    // push rsvg handle
    renderer.libs.push_back(lib);
    
    // parse the sprite info
    try
    {
        // create parser
        ScorePress::XMLSpritesetReader desc_parser;
        
        // if we have no spriteinfo
        if (desc == NULL)
        {
            // try to open the XML-file which is named as the spriteset-file
            std::string infofile(filename, 0, filename.rfind('/') + 1);     // extract the full path
            if (err_file.rfind('.') != std::string::npos)                   // append filename
                infofile.append(err_file, 0, err_file.rfind('.'));          //    without extension
            else
                infofile.append(err_file);
            infofile.insert(0, "file://");                                  // insert protocol
            infofile.append(".xml");                                        // append new extension
            
            // create parser
            desc_parser.open(infofile.c_str());
        }
        
        // if the spriteinfo is external
        else if (strncmp(XML_CAST(desc), "file://", 7) == 0)
        {
            // try to open the external XML-file
            std::string infofile(XML_CAST(desc));
            if (desc[7] != '/' && filename.rfind('/') != std::string::npos) // interpret relative path
                infofile.insert(7, filename, 0, filename.rfind('/') + 1);
            
            // create parser
            desc_parser.open(infofile.c_str());
        }
        
        // if the spriteinfo is within the description
        else
        {
            // create parser for the description itself
            desc_parser.open(XML_CAST(desc), ("file://" + filename).c_str());
        };
        
        // parse description
        setlocale(LC_ALL, "C");
        desc_parser.parse_spriteset(target, renderer, setid);
        setlocale(LC_ALL, "");
    }
    catch (ScorePress::XMLFileReader::Error)
    {
        renderer.libs.pop_back();
        xmlFree(title);
        xmlFree(desc);
        xmlFreeTextReader(parser);
        g_object_unref(lib);
        parser = NULL;
        lib = NULL;
        setlocale(LC_ALL, "");
        throw;
    };
    
    // get dimensions
    RsvgPositionData  pos;
    RsvgDimensionData dim;
    RsvgDimensionData libdim;
    rsvg_handle_get_dimensions(lib, &libdim);
    for (ScorePress::SpriteSet::iterator i = target.begin(); i != target.end(); i++)
    {
        rsvg_handle_get_position_sub(lib, &pos, ("#" + i->path).c_str());
        rsvg_handle_get_dimensions_sub(lib, &dim, ("#" + i->path).c_str());
        if (dim.width != libdim.width   || i->width == 0)  i->width  = dim.width  + pos.x;
        if (dim.height != libdim.height || i->height == 0) i->height = dim.height + pos.y;
    };
    
    // cleanup
    xmlFree(title);
    xmlFree(desc);
    xmlFreeTextReader(parser);
    parser = NULL;
    lib = NULL;
}

// constructor
RSVGRenderer::RSVGRenderer(cairo_t* _drawingCtx) : drawingCtx(_drawingCtx),
                                                   clipped(0),
                                                   font_underline(false)
{
    text_layout = (drawingCtx != NULL) ? pango_cairo_create_layout(drawingCtx) : NULL;
    text_attributes = pango_attr_list_new();
    font = pango_font_description_new();
    font_color.r = font_color.g = font_color.b = 0;
#ifdef CAIRO_HAS_IMAGE_SURFACE
    use_cache = true;
#endif
}

// initialize rendering on given context
void RSVGRenderer::begin(cairo_t* _drawingCtx)
{
    if (text_layout) g_object_unref(text_layout);
    drawingCtx = _drawingCtx;
    text_layout = pango_cairo_create_layout(drawingCtx);
}

// finalize rendering on current context
void RSVGRenderer::end()
{
    if (this->text_layout) g_object_unref(this->text_layout);
    this->drawingCtx = NULL;
    this->text_layout = NULL;
}

// remove sprite-set from sprites collection
void RSVGRenderer::unload_set(const size_t setid)
{
    clear_cache(setid);
    g_object_unref(libs[setid]);
    libs.erase(libs.begin() + setid);
    sprites.erase(sprites.begin() + setid);
}

// erase image cache
#ifdef CAIRO_HAS_IMAGE_SURFACE
void RSVGRenderer::clear_cache()
{
    for (std::map<CacheKey, cairo_surface_t*>::iterator i = cache.begin(); i != cache.end(); ++i)
    {
        cairo_surface_destroy(i->second);
    };
    cache.clear();
}
#else
void RSVGRenderer::clear_cache() {}
#endif

// erase sprites of given set from cache
#ifdef CAIRO_HAS_IMAGE_SURFACE
void RSVGRenderer::clear_cache(const size_t setid)
{
    for (std::map<CacheKey, cairo_surface_t*>::iterator i = cache.begin(); i != cache.end(); ++i)
    {
        while (i->first.sprite_id.setid == setid && i != cache.end())
        {
            cairo_surface_destroy(i->second);
            cache.erase(i);
        };
    };
}
#else
void RSVGRenderer::clear_cache(const size_t) {}
#endif

// enable/disable caching of image data
#ifdef CAIRO_HAS_IMAGE_SURFACE
bool RSVGRenderer::enable_cache(bool enable)
{
    use_cache = enable;
    return use_cache;
}
#else
bool RSVGRenderer::enable_cache(bool) {return use_cache;}
#endif

// destructor
RSVGRenderer::~RSVGRenderer()
{
    clear_cache();
    if (text_layout) g_object_unref(text_layout);
    pango_font_description_free(font);
    pango_attr_list_unref(text_attributes);
    for (std::vector<RsvgHandle*>::iterator i = libs.begin(); i != libs.end(); ++i)
        g_object_unref(*i);
}

// is the object ready to render?
bool RSVGRenderer::ready() const
{
    return (!libs.empty() && drawingCtx != NULL);
}

// does the path exist?
bool RSVGRenderer::exist(const std::string& path) const
{
    for (std::vector<RsvgHandle*>::const_iterator lib = libs.begin(); lib != libs.end(); ++lib)
        if (rsvg_handle_has_sub(*lib, ("#" + path).c_str())) return true;
    return false;
}

// does the path exist in given sprite-set?
bool RSVGRenderer::exist(const std::string& path, const size_t setid) const
{
    return rsvg_handle_has_sub(libs[setid], ("#" + path).c_str());
}

// number of supported formats
size_t RSVGRenderer::spriteset_format_count() const
{
    return 1;
}

// get file-reader for spriteset
RSVGRenderer::ReaderPtr RSVGRenderer::spriteset_reader(const size_t)
{
    return ReaderPtr(new RSVGSpritesetReader());
}

// read new spriteset from reader (returns index)
size_t RSVGRenderer::add_spriteset(ReaderPtr reader)
{
    sprites.push_back(ScorePress::SpriteSet());
    try
    {
        reader->parse_spriteset(sprites.back(), *this, sprites.size() - 1);
        return sprites.size() - 1;
    }
    catch (...)
    {
        sprites.pop_back();
        throw;
    };
}


#ifndef ABS
#   define ABS(x) (((x)<0)?-(x):(x))    // should already be defined by glib-2.0, included by "rsvg-cairo"
#endif

void RSVGRenderer::draw_sprite(const ScorePress::SpriteId sprite_id, double x, double y)
{
    cairo_save(drawingCtx);
    cairo_translate(drawingCtx, x, y);
    rsvg_handle_render_cairo_sub(libs[sprite_id.setid], drawingCtx, ("#" + sprites[sprite_id].path).c_str());
    cairo_restore(drawingCtx);
}

void RSVGRenderer::draw_sprite(const ScorePress::SpriteId sprite_id, double x, double y, double xscale, double yscale)
{
#ifdef CAIRO_HAS_IMAGE_SURFACE
    if (use_cache)
    {
        CacheKey key(sprite_id, static_cast<int>(xscale * 1000), static_cast<int>(yscale * 1000));
        
        std::map<CacheKey, cairo_surface_t*>::iterator surface = cache.find(key);
        
        if (surface == cache.end())
        {
            cache[key] = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                    static_cast<int>(sprites[sprite_id].width * ABS(xscale)) + 5,
                    static_cast<int>(sprites[sprite_id].height * ABS(yscale)) + 5);
            surface = cache.find(key);
            cairo_t* context = cairo_create(surface->second);
            cairo_scale(context, xscale, yscale);
            if (xscale < 0 || yscale < 0)
                cairo_translate(context,
                            (xscale < 0) ? -sprites[sprite_id].width : 0,
                            (yscale < 0) ? -sprites[sprite_id].height : 0);
            rsvg_handle_render_cairo_sub(libs[sprite_id.setid], context, ("#" + sprites[sprite_id].path).c_str());
            cairo_destroy(context);
        };
        
        cairo_mask_surface(drawingCtx, surface->second, static_cast<int>(x+.5), static_cast<int>(y+.5));
    }
    else
#endif
    {
        cairo_save(drawingCtx);
        cairo_new_path(drawingCtx);
        cairo_translate(drawingCtx, x, y);
        cairo_scale(drawingCtx, xscale, yscale);
        if (xscale < 0 || yscale < 0)
            cairo_translate(drawingCtx,
                        (xscale < 0) ? -sprites[sprite_id].width : 0,
                        (yscale < 0) ? -sprites[sprite_id].height : 0);
        rsvg_handle_render_cairo_sub(libs[sprite_id.setid], drawingCtx, ("#" + sprites[sprite_id].path).c_str());
        cairo_restore(drawingCtx);
    };
}

// set width of following lines
void RSVGRenderer::set_line_width(const double width)
{
    cairo_set_line_width(drawingCtx, width);
}

// set the current foreground color
void RSVGRenderer::set_color(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    cairo_set_source_rgba(drawingCtx, r / 255.0, g / 255.0, b / 255.0, a / 255.0);
}

// move cursor
void RSVGRenderer::move_to(const double x, const double y)
{
    cairo_move_to(drawingCtx, x, y);
}

// draw line
void RSVGRenderer::line_to(const double x, const double y)
{
    cairo_line_to(drawingCtx, x, y);
}

// fill the drawn object
void RSVGRenderer::fill()
{
    cairo_fill(drawingCtx);
}

// render the drawn object
void RSVGRenderer::stroke()
{
    cairo_stroke(drawingCtx);
}

// close the drawn object
void RSVGRenderer::close()
{
    cairo_close_path(drawingCtx);
}

// set rectangle clipping
void RSVGRenderer::clip(const int x1, const int y1, const int w, const int h)
{
    cairo_save(drawingCtx);
    cairo_rectangle(drawingCtx, x1, y1, w, h);
    cairo_clip(drawingCtx);
    ++clipped;
}

// reset the last "clip" call
void RSVGRenderer::unclip()
{
    if (!clipped) return;
    --clipped;
    cairo_restore(drawingCtx);
}

// set the font family
void RSVGRenderer::set_font_family(const std::string& family)
{
    pango_font_description_set_family(font, family.c_str());
}

// set the font size (in pt)
void RSVGRenderer::set_font_size(const double pt)
{
    pango_font_description_set_size(font, static_cast<gint>(pt * PANGO_SCALE));
}

// set, if the font is bold
void RSVGRenderer::set_font_bold(const bool bold)
{
    pango_font_description_set_weight(font, (bold) ? PANGO_WEIGHT_BOLD : PANGO_WEIGHT_NORMAL);
}

// set, if the font is italic
void RSVGRenderer::set_font_italic(const bool italic)
{
    pango_font_description_set_style(font, (italic) ? PANGO_STYLE_ITALIC : PANGO_STYLE_NORMAL);
}

// set, if the font is underlined
void RSVGRenderer::set_font_underline(const bool underline)
{
    font_underline = underline;
}

// set the font color
void RSVGRenderer::set_font_color(const unsigned char r, const unsigned char g, const unsigned char b)
{
    font_color.r = r;
    font_color.g = g;
    font_color.b = b;
}

// set the textbox size
void RSVGRenderer::set_text_width(const double width)
{
    pango_layout_set_width(text_layout, static_cast<int>(width * PANGO_SCALE));
}

// reset the textbox to the whole drawing area
void RSVGRenderer::reset_text_width()
{
    pango_layout_set_width(text_layout, -1);
}

// set the alignment
void RSVGRenderer::set_text_align(const enuAlignment align)
{
    switch (align)
    {
    case ALIGN_LEFT:   pango_layout_set_alignment(text_layout, PANGO_ALIGN_LEFT); break;
    case ALIGN_CENTER: pango_layout_set_alignment(text_layout, PANGO_ALIGN_CENTER); break;
    case ALIGN_RIGHT:  pango_layout_set_alignment(text_layout, PANGO_ALIGN_RIGHT); break;
    };
}

// enable/disable justification
void RSVGRenderer::set_text_justify(const bool justify)
{
    pango_layout_set_justify(text_layout, justify);
}

// add a string to the paragraph
void RSVGRenderer::add_text(const std::string& utf8)
{
    if (utf8.empty()) return;
    if (text.size() + utf8.size() > std::numeric_limits<guint>::max()) return;
    PangoAttribute* attr = pango_attr_font_desc_new(font);
    attr->start_index = static_cast<guint>(text.size());
    attr->end_index = static_cast<guint>(text.size() + utf8.size());
    pango_attr_list_insert(text_attributes, attr);
    attr = pango_attr_underline_new((font_underline) ? PANGO_UNDERLINE_SINGLE
                                                     : PANGO_UNDERLINE_NONE);
    attr->start_index = static_cast<guint>(text.size());
    attr->end_index = static_cast<guint>(text.size() + utf8.size());
    pango_attr_list_insert(text_attributes, attr);
    attr = pango_attr_foreground_new(static_cast<guint16>(font_color.r << 8),
                                    static_cast<guint16>(font_color.g << 8),
                                    static_cast<guint16>(font_color.b << 8));
    attr->start_index = static_cast<guint>(text.size());
    attr->end_index = static_cast<guint>(text.size() + utf8.size());
    pango_attr_list_insert(text_attributes, attr);
    text.append(utf8);
}

// render the prepared text
gboolean remove_all(PangoAttribute*, gpointer);
gboolean remove_all(PangoAttribute*, gpointer) {return TRUE;}

void RSVGRenderer::render_text()
{
    pango_layout_set_text(text_layout, text.c_str(), -1);
    pango_layout_set_attributes(text_layout, text_attributes);
    pango_cairo_update_layout(drawingCtx, text_layout);
    pango_cairo_show_layout(drawingCtx, text_layout);
    text.clear();
    pango_attr_list_filter(text_attributes, &remove_all, NULL);
    PangoRectangle rect;
    pango_layout_get_extents(text_layout, NULL, &rect);
    if (cairo_has_current_point(drawingCtx))
        cairo_rel_move_to(drawingCtx, 0, (rect.height - rect.y) / PANGO_SCALE);
    else
        cairo_move_to(drawingCtx, 0, (rect.height + rect.y) / PANGO_SCALE);
}

// invert the given rectangle
void RSVGRenderer::rect_invert(double x1, double y1, double x2, double y2)
{
    cairo_set_source_rgb(drawingCtx, 1.0, 1.0, 1.0);
    cairo_set_operator(drawingCtx, CAIRO_OPERATOR_DIFFERENCE);
    cairo_move_to(drawingCtx, x1, y1);
    cairo_line_to(drawingCtx, x2, y1);
    cairo_line_to(drawingCtx, x2, y2);
    cairo_line_to(drawingCtx, x1, y2);
    cairo_fill(drawingCtx);
    cairo_set_operator(drawingCtx, CAIRO_OPERATOR_OVER);
}

// render a cubic bézier curve
void RSVGRenderer::bezier(double  x1, double  y1,
                           double cx1, double cy1,
                           double cx2, double cy2,
                           double  x2, double  y2)
{
    cairo_move_to(drawingCtx, x1, y1);
    cairo_curve_to(drawingCtx, cx1, cy1, cx2, cy2, x2, y2);
    cairo_stroke(drawingCtx);
}

// render a cubic bézier curve with different thickness in the center than at the ends
void RSVGRenderer::bezier_slur(double  x1, double  y1,
                               double cx1, double cy1,
                               double cx2, double cy2,
                               double  x2, double  y2,
                               double  w0, double  w1)
{
    // save current line-width (for restoring)
    const double line_width = cairo_get_line_width(drawingCtx);
    
    // draw bezier
    if (w0 <= w1 + 0.1 && w0 >= w1 - 0.1)
    {
        cairo_set_line_width(drawingCtx, w0);
        bezier(x1, y1, cx1, cy1, cx2, cy2, x2, y2);
    }
    else
    {
        Renderer::bezier_slur(x1, y1, cx1, cy1, cx2, cy2, x2, y2, w0, w1);
    };
    
    // restore line-width
    cairo_set_line_width(drawingCtx, line_width);
}

