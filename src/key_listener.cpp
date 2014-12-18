
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

#include "key_listener.hh"
#include "controller.hh"

// insert current input-note
void KeyListener::insert(Controller& controller)
{
    if (mode == HEAD) controller.get_cursor().insert_head(note);
    else              controller.get_cursor().insert(note);
    note.dots = 0;
    note.accidental = ScorePress::Accidental::natural;
    controller.reengrave();
    if (mode == HEAD) return;
    controller.get_cursor().next();
    controller.redraw();
}

// insert rest
void KeyListener::insert_rest(Controller& controller)
{
    controller.get_cursor().insert_rest(note.exp, note.dots);
    controller.reengrave();
    controller.get_cursor().next();
    controller.redraw();
    note.dots = 0;
    note.accidental = ScorePress::Accidental::natural;
}

// constructor
KeyListener::KeyListener() : mode(NORMAL),
                             got_home(false),
                             got_end(false),
                             got_8va(false),
                             got_8vab(false),
                             stem_step(500),
                             note_step(500),
                             rest_step(500) {}

// signal handler (on key press)
bool KeyListener::press(const Key key, Controller& controller)
{
    if (!controller.has_cursor()) return false;
    
    // digit input for NDOT
    if (mode == NDOT)
    {
        mode = NORMAL;                          // reset mode
        for (unsigned char i = 0; i < 10; ++i)
        {
            if (digitkeys[i] == key)
            {
                note.dots = i;
                return true;
            };
        };
        return false;
    };
    
    // check other modes
    if (!has_key(key)) return false;
    switch (mode)
    {
    case NORMAL:     action_on(codes[key], controller);         break;
    case HEAD:       action_on(codes[key], controller);         break;
    case NDOT:       /* see above */                            break;
    case STEMLENGTH: action_stemlength(codes[key], controller); break;
    case STEMDIR:    action_stemdir(codes[key], controller);    break;
    case CHROMATIC:  action_chromatic(codes[key], controller);  break;
    case MOVE:       action_move(codes[key], controller);       break;
    case ACCMOVE:    action_accmove(codes[key], controller);    break;
    case STAFFSHIFT: action_staffshift(codes[key], controller); break;
    };
    return true;
}

// signal handler (on key release)
bool KeyListener::release(const Key key)
{
    if (!has_key(key)) return false;
    action_off(codes[key]);
    return true;
}

// NORMAL mode signal handler
void KeyListener::action_on(const ActionKey code, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    
    switch (code)
    {
    // cursor movement
    case KEY_UP:    if      (cursor.has_prev_voice()) cursor.prev_voice();
                    else if (cursor.has_prev_line())  cursor.prev_line();
                    controller.redraw();
                    return;
    
    case KEY_DOWN:  if      (cursor.has_next_voice()) cursor.next_voice();
                    else if (cursor.has_next_line())  cursor.next_line();
                    controller.redraw();
                    return;
    
    case KEY_RIGHT: if (cursor.at_end()) return;
                    cursor.next();
                    controller.redraw();
                    return;
    
    case KEY_LEFT:  if (!cursor.has_prev()) return;
                    cursor.prev();
                    controller.redraw();
                    return;
    
    case KEY_HOME:  if (got_home) cursor.home();
                    else          cursor.home_voice();
                    got_home = false;
                    controller.redraw();
                    return;
                    
    case KEY_END:   if (got_end) cursor.end();
                    else         cursor.end_voice(); 
                    got_end  = false;
                    controller.redraw();
                    return;
    
    // special action
    case KEY_NEWLINE:   cursor.insert_newline();
                        controller.on_resize();
                        cursor.next_line_home();
                        controller.redraw();
                        return;
    
    case KEY_PAGEBREAK: cursor.insert_pagebreak();
                        controller.on_resize();
                        cursor.next_line_home();
                        controller.redraw();
                        return;
    
    case KEY_NEWVOICE:  cursor.add_voice();
                        cursor.update_voices();
                        controller.on_resize();
                        return;
    
    case KEY_DELETE:    if (cursor.at_end()) return;
                        cursor.remove();
                        controller.reengrave();
                        return;
    
    case KEY_BACKSPACE: if (!cursor.has_prev()) return;
                        cursor.prev();
                        cursor.remove();
                        controller.reengrave();
                        return;
    
    case KEY_DELVOICE:  cursor.remove_voice(); controller.reengrave(); return;
    case KEY_DELBREAK:  cursor.remove_break(); controller.reengrave(); return;
    
    // note value
    case KEY_LONGA:   note.exp = ScorePress::VALUE_BASE + 2; if (!insert_on_name) insert(controller); return;
    case KEY_BREVE:   note.exp = ScorePress::VALUE_BASE + 1; if (!insert_on_name) insert(controller); return;
    case KEY_WHOLE:   note.exp = ScorePress::VALUE_BASE;     if (!insert_on_name) insert(controller); return;
    case KEY_HALF:    note.exp = ScorePress::VALUE_BASE - 1; if (!insert_on_name) insert(controller); return;
    case KEY_QUARTER: note.exp = ScorePress::VALUE_BASE - 2; if (!insert_on_name) insert(controller); return;
    case KEY_EIGHTH:  note.exp = ScorePress::VALUE_BASE - 3; if (!insert_on_name) insert(controller); return;
    case KEY_16TH:    note.exp = ScorePress::VALUE_BASE - 4; if (!insert_on_name) insert(controller); return;
    case KEY_32TH:    note.exp = ScorePress::VALUE_BASE - 5; if (!insert_on_name) insert(controller); return;
    case KEY_64TH:    note.exp = ScorePress::VALUE_BASE - 6; if (!insert_on_name) insert(controller); return;
    case KEY_128TH:   note.exp = ScorePress::VALUE_BASE - 7; if (!insert_on_name) insert(controller); return;
    
    case KEY_REST:    insert_rest(controller); break;
    
    // note name
    case KEY_C: note.name = ScorePress::EditCursor::C; if (insert_on_name) insert(controller); return;
    case KEY_D: note.name = ScorePress::EditCursor::D; if (insert_on_name) insert(controller); return;
    case KEY_E: note.name = ScorePress::EditCursor::E; if (insert_on_name) insert(controller); return;
    case KEY_F: note.name = ScorePress::EditCursor::F; if (insert_on_name) insert(controller); return;
    case KEY_G: note.name = ScorePress::EditCursor::G; if (insert_on_name) insert(controller); return;
    case KEY_A: note.name = ScorePress::EditCursor::A; if (insert_on_name) insert(controller); return;
    case KEY_B: note.name = ScorePress::EditCursor::B; if (insert_on_name) insert(controller); return;
    
    // accidentals
    case KEY_SHARP:         note.accidental = ScorePress::Accidental::sharp;         return;
    case KEY_FLAT:          note.accidental = ScorePress::Accidental::flat;          return;
    case KEY_DOUBLESHARP:   note.accidental = ScorePress::Accidental::double_sharp;  return;
    case KEY_DOUBLEFLAT:    note.accidental = ScorePress::Accidental::double_flat;   return;
    case KEY_HALFSHARP:     note.accidental = ScorePress::Accidental::half_sharp;    return;
    case KEY_HALFFLAT:      note.accidental = ScorePress::Accidental::half_flat;     return;
    case KEY_SHARPANDAHALF: note.accidental = ScorePress::Accidental::flat_andahalf; return;
    case KEY_FLATANDAHALF:  note.accidental = ScorePress::Accidental::flat_andahalf; return;
    
    // octave modification
    case KEY_8VA:        got_8va = true;  // continue with OCTAVEUP
    case KEY_OCTAVEUP:   ++note.octave;   return;
    case KEY_8VAB:       got_8vab = true; // continue with OCTAVEDOWN
    case KEY_OCTAVEDOWN: --note.octave;   return;
    
    // dot input
    case KEY_DOT:  note.dots = 1; return;
    case KEY_2DOT: note.dots = 2; return;
    case KEY_NDOT: mode = NDOT;   return;
    
    // mode modification
    case KEY_STEMLENGTH: mode = STEMLENGTH; return;
    case KEY_STEMDIR:    mode = STEMDIR;    return;
    case KEY_CHROMATIC:  mode = CHROMATIC;  return;
    case KEY_MOVE:       mode = MOVE;       return;
    case KEY_ACCMOVE:    mode = ACCMOVE;    return;
    case KEY_STAFFSHIFT: mode = STAFFSHIFT; return;
    
    // beam modification
    case KEY_NOBEAM:
    case KEY_AUTOBEAM:
    case KEY_CUTBEAM:
    case KEY_FORCEBEAM:
        {
        // get cursor to the nearest previous chord
        ScorePress::EditCursor cur(cursor);
        if (!cur.has_prev()) return;
        do cur.prev(); while (cur.has_prev() && !cur.get_cursor()->is(ScorePress::Class::CHORD));
        if (!cur.get_cursor()->is(ScorePress::Class::CHORD)) return;
        
        // if we have got a chord, set the beam property
        switch (code)
        {
        case KEY_NOBEAM:
            static_cast<ScorePress::Chord&>(*cur.get_cursor()).beam = ScorePress::Chord::NO_BEAM;
            return;
        case KEY_AUTOBEAM:
            static_cast<ScorePress::Chord&>(*cur.get_cursor()).beam = ScorePress::Chord::AUTO_BEAM;
            return;
        case KEY_FORCEBEAM:
            static_cast<ScorePress::Chord&>(*cur.get_cursor()).beam = ScorePress::Chord::FORCE_BEAM;
            return;
        case KEY_CUTBEAM:
            static_cast<ScorePress::Chord&>(*cur.get_cursor()).beam = ScorePress::Chord::CUT_BEAM;
            return;
        default:
            return;
        };
        }
    
    // head insertion mode
    case KEY_HEAD_MODE: mode = ((mode == HEAD) ? NORMAL : HEAD); return;
    
    // ignore key-count indicator (shouldn't be set anyway)
    case KEY_COUNT: return;
    };
}

// key release signal handler
void KeyListener::action_off(const ActionKey code)
{
    switch (code)
    {
    // octave modification
    case KEY_8VA:  if (!got_8va) break;
                   --note.octave;
                   got_8va = false;
                   break;
    
    case KEY_8VAB: if (!got_8vab) break;
                   ++note.octave;
                   got_8vab = false;
                   break;
    
    // mode modification
    case KEY_STEMLENGTH: if (mode == STEMLENGTH)               mode = NORMAL; break;
    case KEY_STEMDIR:    if (mode == STEMDIR)                  mode = NORMAL; break;
    case KEY_CHROMATIC:  if (mode == CHROMATIC)                mode = NORMAL; break;
    case KEY_MOVE:       if (mode == MOVE)                     mode = NORMAL; break;
    case KEY_ACCMOVE:    if (mode == ACCMOVE)                  mode = NORMAL; break;
    case KEY_STAFFSHIFT: if (mode == STAFFSHIFT)               mode = NORMAL; break;
    case KEY_HEAD_MODE:  if (mode == HEAD && insert_head_hold) mode = NORMAL; break;
    
    // ignore other codes
    default: break;
    };
}

// STEMLENGTH mode signal handler
void KeyListener::action_stemlength(const ActionKey code, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    
    // check cursor validity
    if (cursor.at_end()) return;
    if (!cursor.ready()) return;
    if (!cursor.get_cursor()->is(ScorePress::Class::CHORD)) return;
    
    // check key
    switch (code)
    {
        // stem length modification
    case KEY_UP:   cursor.add_stem_length( stem_step); break;
    case KEY_DOWN: cursor.add_stem_length(-stem_step); break;
    
        // beam angle adjustment (stemlength key)
    case KEY_LEFT:  cursor.add_stem_slope( stem_step); break;
    case KEY_RIGHT: cursor.add_stem_slope(-stem_step); break;
    
    case KEY_HOME:
        cursor.set_stem_length_auto();
        break;
    
        // ignore other codes (skip reengraving)
    default: return;
    };
    
    // signal changes
    controller.reengrave();
}

// STEMDIR mode signal handler
void KeyListener::action_stemdir(const ActionKey code, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    
    // check cursor validity
    if (cursor.at_end()) return;
    if (!cursor.ready()) return;
    if (!cursor.get_cursor()->is(ScorePress::Class::CHORD)) return;
    
    // check key
    switch (code)
    {
        // stem direction
    case KEY_UP:   cursor.set_stem_dir(false); break;
    case KEY_DOWN: cursor.set_stem_dir(true);  break;
    case KEY_HOME: cursor.set_stem_dir_auto(); break;
    
        // ignore other codes (skip reengraving)
    default: return;
    };
    
    // signal changes
    controller.reengrave();
}

// CHROMATIC mode signal handler
void KeyListener::action_chromatic(const ActionKey code, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    
    // check cursor validity
    if (cursor.at_end()) return;
    if (!cursor.ready()) return;
    if (!cursor.get_cursor()->is(ScorePress::Class::CHORD)) return;
    ScorePress::Chord& chord = static_cast<ScorePress::Chord&>(*cursor.get_cursor());
    
    // check key
    switch (code)
    {
        // shift direction
    case KEY_UP:
        for (ScorePress::HeadList::iterator head = chord.heads.begin(); head != chord.heads.end(); ++head)
            ++(*head)->tone;
        cursor.set_accidental_auto();
        break;
    
    case KEY_DOWN:
        for (ScorePress::HeadList::iterator head = chord.heads.begin(); head != chord.heads.end(); ++head)
            --(*head)->tone;
        cursor.set_accidental_auto();
        break;
    
        // ignore other codes (skip reengraving)
    default: return;
    };
    
    // signal changes
    controller.reengrave();
}

// MOVE mode signal handler
void KeyListener::action_move(const ActionKey code, Controller& controller)
{
    // check cursor validity
    ScorePress::EditCursor& cursor = controller.get_cursor();
    if (cursor.at_end()) return;
    
    // check key
    switch (code)
    {
        // cursor movement
    case KEY_UP:    if (cursor.ready() && cursor.get_cursor()->is(ScorePress::Class::REST))
                        static_cast<ScorePress::Rest&>(*cursor.get_cursor()).offset_y -= rest_step;
                    break;
    
    case KEY_DOWN:  if (cursor.ready() && cursor.get_cursor()->is(ScorePress::Class::REST))
                        static_cast<ScorePress::Rest&>(*cursor.get_cursor()).offset_y += rest_step;
                    break;
    
    case KEY_RIGHT: if (cursor.ready() && cursor.get_cursor()->is(ScorePress::Class::VISIBLEOBJECT))
                        cursor.get_cursor()->get_visible().offset_x += note_step;
                    break;
    
    case KEY_LEFT:  if (cursor.ready() && cursor.get_cursor()->is(ScorePress::Class::VISIBLEOBJECT))
                        cursor.get_cursor()->get_visible().offset_x -= note_step;
                    break;
    
    case KEY_HOME:  if (cursor.ready() && cursor.get_cursor()->is(ScorePress::Class::VISIBLEOBJECT))
                        cursor.get_cursor()->get_visible().offset_x = 0;
                    if (cursor.ready() && cursor.get_cursor()->is(ScorePress::Class::REST))
                        static_cast<ScorePress::Rest&>(*cursor.get_cursor()).offset_y = 0;
                    break;
    
        // ignore other codes (skip reengraving)
    default: return;
    };
    
    // signal changes
    controller.reengrave();
}

// ACCMOVE mode signal handler
void KeyListener::action_accmove(const ActionKey code, Controller& controller)
{
    // check cursor validity
    ScorePress::EditCursor& cursor = controller.get_cursor();
    if (cursor.at_end()) return;
    
    // check key
    switch (code)
    {
        // cursor movement
    case KEY_RIGHT: cursor.get_cursor()->acc_offset += note_step; break;
    case KEY_LEFT:  cursor.get_cursor()->acc_offset -= note_step; break;
    case KEY_HOME:  cursor.get_cursor()->acc_offset = 0; break;
    
        // ignore other codes (skip reengraving)
    default: return;
    };
    
    // signal changes
    controller.reengrave();
}

// STAFFSHIFT mode signal handler
void KeyListener::action_staffshift(const ActionKey code, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    
    // check cursor validity
    if (cursor.at_end()) return;
    if (!cursor.ready()) return;
    if (!cursor.get_cursor()->is(ScorePress::Class::NOTEOBJECT)) return;
    
    // check key
    switch (code)
    {
        // cursor movement
    case KEY_UP:
        static_cast<ScorePress::NoteObject&>(*cursor.get_cursor()).staff_shift -= 1;
        break;
    
    case KEY_DOWN:
        static_cast<ScorePress::NoteObject&>(*cursor.get_cursor()).staff_shift += 1;
        break;
    
    case KEY_HOME:
        static_cast<ScorePress::NoteObject&>(*cursor.get_cursor()).staff_shift = 0;
        break;
    
        // ignore other codes (skip reengraving)
    default: return;
    };
    
    // signal changes
    controller.reengrave();
}

