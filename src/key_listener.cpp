
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

void KeyListener::insert(Controller& controller)
{
    if (head_input)  controller.get_cursor().insert_head(note);
    else             controller.get_cursor().insert(note);
    controller.reengrave();
    if (!head_input) controller.get_cursor().next();
    note.dots = 0;
    note.accidental = ScorePress::Accidental::natural;
}

void KeyListener::insert_rest(Controller& controller)
{
    controller.get_cursor().insert_rest(note.exp, note.dots);
    controller.reengrave();
    controller.get_cursor().next();
    note.dots = 0;
    note.accidental = ScorePress::Accidental::natural;
}

KeyListener::KeyListener() : mode(NORMAL),
                             head_input(false),
                             got_home(false),
                             got_end(false),
                             got_octaveup(false),
                             got_octavedown(false) {}

void KeyListener::action_on(const ActionKey code, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    
    switch (code)
    {
    // cursor movement
    case KEY_UP:    if      (cursor.has_prev_voice()) cursor.prev_voice();
                    else if (cursor.has_prev_line())  cursor.prev_line();  return;
    case KEY_DOWN:  if      (cursor.has_next_voice()) cursor.next_voice();
                    else if (cursor.has_next_line())  cursor.next_line();  return;
    case KEY_RIGHT: if      (!cursor.at_end())        cursor.next();       return;
    case KEY_LEFT:  if      (cursor.has_prev())       cursor.prev();       return;
    case KEY_HOME:  if (got_home) cursor.home(); else cursor.home_voice();
                    got_home = false;                                      return;
    case KEY_END:   if (got_end)  cursor.end();  else cursor.end_voice(); 
                    got_end  = false;                                      return;
    
    // special action
    case KEY_NEWLINE:   cursor.insert_newline();        break;
    case KEY_PAGEBREAK: cursor.insert_pagebreak();      break;
    case KEY_NEWVOICE:  cursor.add_voice();             break;
    case KEY_DELETE:    if (cursor.at_end())            break;
                        cursor.remove();                break;
    case KEY_BACKSPACE: if (!cursor.has_prev())         break;
                        cursor.prev(); cursor.remove(); break;
    case KEY_DELVOICE:  cursor.remove_voice();          break;
    case KEY_DELBREAK:  cursor.remove_break();          break;
    
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
    case KEY_8VA:        got_octaveup = true;
    case KEY_OCTAVEUP:   ++note.octave; return;
    case KEY_8VAB:       got_octavedown = true;
    case KEY_OCTAVEDOWN: --note.octave; return;
    
    // dot input
    case KEY_DOT:  note.dots = 1;     return;
    case KEY_2DOT: note.dots = 2;     return;
    case KEY_NDOT: mode = NDOT_INPUT; return;
    
    // note modification
    case KEY_STEMLENGTH: mode = STEMLENGTH_INPUT; return;
    case KEY_STEMDIR:    mode = STEMDIR_INPUT;    return;
    case KEY_CHROMATIC:  mode = CHROMATIC_INPUT;  return;
    case KEY_MOVE:       mode = MOVE_INPUT;       return;
    case KEY_ACCMOVE:    mode = ACCMOVE_INPUT;    return;
    case KEY_STAFFSHIFT: mode = STAFFSHIFT_INPUT; return;
    case KEY_NOBEAM:
    case KEY_AUTOBEAM:
    case KEY_CUTBEAM:
    case KEY_FORCEBEAM:
        {
        ScorePress::EditCursor cur(cursor);
        if (!cur.has_prev()) break;
        do
        {
            cur.prev();
        } while (cur.has_prev() && !cur.get_cursor()->is(ScorePress::Class::CHORD));
        
        if (cur.get_cursor()->is(ScorePress::Class::CHORD))
        {
            switch (code)
            {
            case KEY_NOBEAM:
                static_cast<ScorePress::Chord&>(*cur.get_cursor()).beam = ScorePress::Chord::NO_BEAM;
                break;
            case KEY_AUTOBEAM:
                static_cast<ScorePress::Chord&>(*cur.get_cursor()).beam = ScorePress::Chord::AUTO_BEAM;
                break;
            case KEY_FORCEBEAM:
                static_cast<ScorePress::Chord&>(*cur.get_cursor()).beam = ScorePress::Chord::FORCE_BEAM;
                break;
            case KEY_CUTBEAM:
                static_cast<ScorePress::Chord&>(*cur.get_cursor()).beam = ScorePress::Chord::CUT_BEAM;
                break;
            default:
                break;
            };
        }
        else return;
        };
        break;
    // mode modification
    case KEY_HEAD_MODE: head_input = (insert_head_hold || !head_input); return;
    
    // ignore key-count indicator (shouldn't be set anyway)
    case KEY__COUNT__: return;
    };
    
    // reengrave score
    controller.reengrave();
    if (code == KEY_NEWLINE || code == KEY_PAGEBREAK)
    {
        cursor.next_line_home();
        controller.on_score_resize();
    }
    else if (code == KEY_DELBREAK)
    {
        cursor.update_voices();
        controller.on_score_resize();
    };
}

void KeyListener::action_off(const ActionKey code)
{
    switch (code)
    {
    // octave modification
    case KEY_8VA:  if (!got_octaveup) break;
                   --note.octave;
                   got_octaveup = false;
                   break;
    case KEY_8VAB: if (!got_octavedown) break;
                   ++note.octave;
                   got_octavedown = false;
                   break;
    
    // note modification
    case KEY_STEMLENGTH: if (mode == STEMLENGTH_INPUT) mode = NORMAL; break;
    case KEY_STEMDIR:    if (mode == STEMDIR_INPUT)    mode = NORMAL; break;
    case KEY_CHROMATIC:  if (mode == CHROMATIC_INPUT)  mode = NORMAL; break;
    case KEY_MOVE:       if (mode == MOVE_INPUT)       mode = NORMAL; break;
    case KEY_ACCMOVE:    if (mode == ACCMOVE_INPUT)    mode = NORMAL; break;
    case KEY_STAFFSHIFT: if (mode == STAFFSHIFT_INPUT) mode = NORMAL; break;
    
    // mode modification
    case KEY_HEAD_MODE: head_input = (!insert_head_hold && head_input); break;
    
    // ignore other codes
    default: break;
    };
}

void KeyListener::action_stemlength(const ActionKey code, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    
    if (cursor.at_end()) return;
    if (!cursor.ready()) return;
    if (!cursor.get_cursor()->is(ScorePress::Class::CHORD)) return;
    
    switch (code)
    {
    // stem length modification
    case KEY_UP:   cursor.add_stem_length( 500); break;
    case KEY_DOWN: cursor.add_stem_length(-500); break;
    
    // beam angle adjustment (stemlength key)
    case KEY_LEFT:  cursor.add_stem_slope( 500); break;
    case KEY_RIGHT: cursor.add_stem_slope(-500); break;
    
    case KEY_HOME:
        cursor.set_stem_length_auto();
        break;
    
    // ignore other codes (skip reengraving)
    default: return;
    };
    
    controller.reengrave();
}

inline void _positive(int& i) {i = (i < 0) ? -i : i;}
inline void _negative(int& i) {i = (i > 0) ? -i : i;}

void KeyListener::action_stemdir(const ActionKey code, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    
    if (cursor.at_end()) return;
    if (!cursor.ready()) return;
    if (!cursor.get_cursor()->is(ScorePress::Class::CHORD)) return;
    
    switch (code)
    {
    // stem direction
    case KEY_UP:   cursor.set_stem_dir(false); break;
    case KEY_DOWN: cursor.set_stem_dir(true);  break;
    case KEY_HOME: cursor.set_stem_dir_auto(); break;
    
    // ignore other codes (skip reengraving)
    default: return;
    };
    
    controller.reengrave();
}

void KeyListener::action_chromatic(const ActionKey code, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    
    if (cursor.at_end()) return;
    if (!cursor.ready()) return;
    if (!cursor.get_cursor()->is(ScorePress::Class::CHORD)) return;
    ScorePress::Chord& chord = static_cast<ScorePress::Chord&>(*cursor.get_cursor());
    
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
    
    controller.reengrave();
}

void KeyListener::action_move(const ActionKey code, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    if (cursor.at_end()) return;
    
    switch (code)
    {
    // cursor movement
    case KEY_UP:    if (cursor.ready() && cursor.get_cursor()->is(ScorePress::Class::REST))
                        static_cast<ScorePress::Rest&>(*cursor.get_cursor()).offset_y -= 500;
                    break;
    
    case KEY_DOWN:  if (cursor.ready() && cursor.get_cursor()->is(ScorePress::Class::REST))
                        static_cast<ScorePress::Rest&>(*cursor.get_cursor()).offset_y += 500;
                    break;
    
    case KEY_RIGHT: if (cursor.ready() && cursor.get_cursor()->is(ScorePress::Class::VISIBLEOBJECT))
                        cursor.get_cursor()->get_visible().offset_x += 500;
                    break;
    
    case KEY_LEFT:  if (cursor.ready() && cursor.get_cursor()->is(ScorePress::Class::VISIBLEOBJECT))
                        cursor.get_cursor()->get_visible().offset_x -= 500;
                    break;
    
    case KEY_HOME:  if (cursor.ready() && cursor.get_cursor()->is(ScorePress::Class::VISIBLEOBJECT))
                        cursor.get_cursor()->get_visible().offset_x = 0;
                    if (cursor.ready() && cursor.get_cursor()->is(ScorePress::Class::REST))
                        static_cast<ScorePress::Rest&>(*cursor.get_cursor()).offset_y = 0;
                    break;
    
    // ignore other codes (skip reengraving)
    default: return;
    };
    
    controller.reengrave();
}

#include <iostream>
void KeyListener::action_accmove(const ActionKey code, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    if (cursor.at_end()) return;
    
    switch (code)
    {
    // cursor movement
    case KEY_RIGHT: std::cout << "ACC. MOVE: -->\n"; cursor.get_cursor()->acc_offset += 500; break;
    case KEY_LEFT:  std::cout << "ACC. MOVE: <--\n"; cursor.get_cursor()->acc_offset -= 500; break;
    case KEY_HOME:  std::cout << "ACC. MOVE: ->|<-\n"; cursor.get_cursor()->acc_offset = 0; break;
    
    // ignore other codes (skip reengraving)
    default: return;
    };
    
    controller.reengrave();
}

void KeyListener::action_staffshift(const ActionKey code, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    
    if (cursor.at_end()) return;
    if (!cursor.ready()) return;
    if (!cursor.get_cursor()->is(ScorePress::Class::NOTEOBJECT)) return;
    
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
    
    controller.reengrave();
}

bool KeyListener::press(const Key key, Controller& controller)
{
    if (mode == NDOT_INPUT)
    {
        mode = NORMAL;
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
    
    if (!has_key(key)) return false;
    switch (mode)
    {
    case NORMAL:           action_on(codes[key], controller);         break;
    case NDOT_INPUT:       /* see above */                            break;
    case STEMLENGTH_INPUT: action_stemlength(codes[key], controller); break;
    case STEMDIR_INPUT:    action_stemdir(codes[key], controller);    break;
    case CHROMATIC_INPUT:  action_chromatic(codes[key], controller);  break;
    case MOVE_INPUT:       action_move(codes[key], controller);       break;
    case ACCMOVE_INPUT:    action_accmove(codes[key], controller);    break;
    case STAFFSHIFT_INPUT: action_staffshift(codes[key], controller); break;
    };
    return true;
}

bool KeyListener::release(const Key key)
{
    if (!has_key(key)) return false;
    action_off(codes[key]);
    return true;
}

