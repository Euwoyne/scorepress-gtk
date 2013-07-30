
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

#include "key_listener.hh"

void KeyListener::insert(ScorePress::EditCursor& cursor)
{
    if (head_input) cursor.insert_head(note);
    else            cursor.insert(note);
    note.dots = 0;
    note.accidental = ScorePress::Accidental::natural;
}

KeyListener::KeyListener() : mode(NORMAL),
                             head_input(false),
                             got_home(false),
                             got_end(false) {}

//#include <iostream>
/*
static const std::string ids[52] = {"KEY_UP",   "KEY_DOWN",    "KEY_RIGHT",  "KEY_LEFT", "KEY_HOME", "KEY_END",
                              "KEY_REST", "KEY_NEWLINE", "KEY_DELETE", "KEY_BACKSPACE",
                              "KEY_LONGA", "KEY_BREVE", "KEY_WHOLE", "KEY_HALF", "KEY_QUARTER", "KEY_EIGHTH",
                                           "KEY_16TH",  "KEY_32TH",  "KEY_64TH", "KEY_128TH",
                              "KEY_C", "KEY_D", "KEY_E", "KEY_F", "KEY_G", "KEY_A", "KEY_B",
                              "KEY_SHARP",  "KEY_FLAT",  "KEY_DOUBLESHARP",    "KEY_DOUBLEFLAT",
                                                         "KEY_HALFSHARP",      "KEY_HALFFLAT",
                                                         "KEY_SHARPANDAHALF",  "KEY_FLATANDAHALF",
                              "KEY_8VA", "KEY_8VAB", "KEY_OCTAVEUP", "KEY_OCTAVEDOWN",
                              "KEY_DOT", "KEY_2DOT", "KEY_NDOT",
                              "KEY_STEMLENGTH", "KEY_STEMDIR", "KEY_CHROMATIC", "KEY_MOVE", "KEY_ACCMOVE", "KEY_STAFFSHIFT",
                              "KEY_NOBEAM", "KEY_AUTOBEAM", "KEY_FORCEBEAM", "KEY_CUTBEAM",
                              "KEY_HEAD_MODE", "__COUNT___"};
//*/

void KeyListener::action_on(const ActionKey code, ScorePress::EditCursor& cursor)
{
    //std::cout << ids[code] << "\n";
    
    switch (code)
    {
    // cursor movement
    case KEY_UP:    if      (cursor.has_prev_voice()) cursor.prev_voice();
                    else if (cursor.has_prev_line())  cursor.prev_line();  break;
    case KEY_DOWN:  if      (cursor.has_next_voice()) cursor.next_voice();
                    else if (cursor.has_next_line())  cursor.next_line();  break;
    case KEY_RIGHT: if      (!cursor.at_end())        cursor.next();       break;
    case KEY_LEFT:  if      (cursor.has_prev())       cursor.prev();       break;
    case KEY_HOME:  if (got_home) cursor.home(); else cursor.home_voice(); break;
    case KEY_END:   if (got_end)  cursor.end();  else cursor.end_voice();  break;
    
    case KEY_REST:      cursor.insert_rest(note.exp, note.dots); break;
    case KEY_NEWLINE:   cursor.insert_newline(); break;
    case KEY_DELETE:    if (cursor.at_end()) break;
                        cursor.remove(); break;
    case KEY_BACKSPACE: if (!cursor.has_prev()) break;
                        cursor.prev(); cursor.remove(); break;
    
    // note value
    case KEY_LONGA:   note.exp = ScorePress::VALUE_BASE + 2; if (!insert_on_name) insert(cursor); break;
    case KEY_BREVE:   note.exp = ScorePress::VALUE_BASE + 1; if (!insert_on_name) insert(cursor); break;
    case KEY_WHOLE:   note.exp = ScorePress::VALUE_BASE;     if (!insert_on_name) insert(cursor); break;
    case KEY_HALF:    note.exp = ScorePress::VALUE_BASE - 1; if (!insert_on_name) insert(cursor); break;
    case KEY_QUARTER: note.exp = ScorePress::VALUE_BASE - 2; if (!insert_on_name) insert(cursor); break;
    case KEY_EIGHTH:  note.exp = ScorePress::VALUE_BASE - 3; if (!insert_on_name) insert(cursor); break;
    case KEY_16TH:    note.exp = ScorePress::VALUE_BASE - 4; if (!insert_on_name) insert(cursor); break;
    case KEY_32TH:    note.exp = ScorePress::VALUE_BASE - 5; if (!insert_on_name) insert(cursor); break;
    case KEY_64TH:    note.exp = ScorePress::VALUE_BASE - 6; if (!insert_on_name) insert(cursor); break;
    case KEY_128TH:   note.exp = ScorePress::VALUE_BASE - 7; if (!insert_on_name) insert(cursor); break;
    
    // note name
    case KEY_C: note.name = ScorePress::EditCursor::C; if (insert_on_name) insert(cursor); break;
    case KEY_D: note.name = ScorePress::EditCursor::D; if (insert_on_name) insert(cursor); break;
    case KEY_E: note.name = ScorePress::EditCursor::E; if (insert_on_name) insert(cursor); break;
    case KEY_F: note.name = ScorePress::EditCursor::F; if (insert_on_name) insert(cursor); break;
    case KEY_G: note.name = ScorePress::EditCursor::G; if (insert_on_name) insert(cursor); break;
    case KEY_A: note.name = ScorePress::EditCursor::A; if (insert_on_name) insert(cursor); break;
    case KEY_B: note.name = ScorePress::EditCursor::B; if (insert_on_name) insert(cursor); break;
    
    // accidentals
    case KEY_SHARP:         note.accidental = ScorePress::Accidental::sharp;         break;
    case KEY_FLAT:          note.accidental = ScorePress::Accidental::flat;          break;
    case KEY_DOUBLESHARP:   note.accidental = ScorePress::Accidental::double_sharp;  break;
    case KEY_DOUBLEFLAT:    note.accidental = ScorePress::Accidental::double_flat;   break;
    case KEY_HALFSHARP:     note.accidental = ScorePress::Accidental::half_sharp;    break;
    case KEY_HALFFLAT:      note.accidental = ScorePress::Accidental::half_flat;     break;
    case KEY_SHARPANDAHALF: note.accidental = ScorePress::Accidental::flat_andahalf; break;
    case KEY_FLATANDAHALF:  note.accidental = ScorePress::Accidental::flat_andahalf; break;
    
    // octave modification
    case KEY_8VA:
    case KEY_OCTAVEUP:   ++note.octave; break;
    case KEY_8VAB:
    case KEY_OCTAVEDOWN: --note.octave; break;
    
    // dot input
    case KEY_DOT:  note.dots = 1;     break;
    case KEY_2DOT: note.dots = 2;     break;
    case KEY_NDOT: mode = NDOT_INPUT; break;
    
    // note modification
    case KEY_STEMLENGTH: mode = STEMLENGTH_INPUT; break;
    case KEY_STEMDIR:    mode = STEMDIR_INPUT;    break;
    case KEY_CHROMATIC:  mode = CHROMATIC_INPUT;  break;
    case KEY_MOVE:       mode = MOVE_INPUT;       break;
    case KEY_ACCMOVE:    mode = ACCMOVE_INPUT;    break;
    case KEY_STAFFSHIFT: mode = STAFFSHIFT_INPUT; break;
    case KEY_NOBEAM:
    case KEY_AUTOBEAM:
    case KEY_CUTBEAM:
    case KEY_FORCEBEAM:
        if (cursor.has_prev())
        {
            cursor.prev();
            if (cursor.get_cursor()->is(ScorePress::Class::CHORD))
            {
                switch (code) {
                case KEY_NOBEAM:
                    static_cast<ScorePress::Chord&>(*cursor.get_cursor()).beam = ScorePress::Chord::NO_BEAM;
                    break;
                case KEY_AUTOBEAM:
                    static_cast<ScorePress::Chord&>(*cursor.get_cursor()).beam = ScorePress::Chord::AUTO_BEAM;
                    break;
                case KEY_FORCEBEAM:
                    static_cast<ScorePress::Chord&>(*cursor.get_cursor()).beam = ScorePress::Chord::FORCE_BEAM;
                    break;
                case KEY_CUTBEAM:
                    static_cast<ScorePress::Chord&>(*cursor.get_cursor()).beam = ScorePress::Chord::CUT_BEAM;
                    break;
                default: break;};
            };
            cursor.next();
            cursor.reengrave();
        };
        break;
    
    // mode modification
    case KEY_HEAD_MODE: head_input = (insert_head_hold || !head_input); break;
    
    // ignore other codes
    default: break;
    };
    
    // remember KEY_HOME
    got_home = (code == KEY_HOME);
    got_end  = (code == KEY_END);
    
    //cursor.dump();
}

void KeyListener::action_off(const ActionKey code)
{
    switch (code)
    {
    // octave modification
    case KEY_8VA:  --note.octave; break;
    case KEY_8VAB: ++note.octave; break;
    
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

void KeyListener::action_stemlength(const ActionKey code, ScorePress::EditCursor& cursor)
{
    if (cursor.at_end()) return;
    if (!cursor.ready()) return;
    if (!cursor.get_cursor()->is(ScorePress::Class::CHORD)) return;
    
    switch (code)
    {
    // cursor movement
    case KEY_UP:
        static_cast<ScorePress::Chord&>(*cursor.get_cursor()).stem_length += 500;
        break;
        
    case KEY_DOWN:
        static_cast<ScorePress::Chord&>(*cursor.get_cursor()).stem_length -= 500;
        break;
        
    // TODO: beam angle adjustment (stemlength key)
    case KEY_RIGHT: break;
    case KEY_LEFT:  break;
    
    case KEY_HOME:
        cursor.set_stem_length_auto();
        break;
    
    // ignore other codes (skip reengraving)
    default: return;
    };
    
    cursor.reengrave();
}

inline void _positive(int& i) {i = (i < 0) ? -i : i;}
inline void _negative(int& i) {i = (i > 0) ? -i : i;}

void KeyListener::action_stemdir(const ActionKey code, ScorePress::EditCursor& cursor)
{
    if (cursor.at_end()) return;
    if (!cursor.ready()) return;
    if (!cursor.get_cursor()->is(ScorePress::Class::CHORD)) return;
    
    switch (code)
    {
    // stem direction
    case KEY_UP:
        _positive(static_cast<ScorePress::Chord&>(*cursor.get_cursor()).stem_length);
        break;
    
    case KEY_DOWN:
        _negative(static_cast<ScorePress::Chord&>(*cursor.get_cursor()).stem_length);
        break;
    
    case KEY_HOME:
        cursor.set_stem_dir_auto();
        break;
    
    // ignore other codes (skip reengraving)
    default: return;
    };
    
    cursor.reengrave();
}

void KeyListener::action_chromatic(const ActionKey code, ScorePress::EditCursor& cursor)
{
    if (cursor.at_end()) return;
    if (!cursor.ready()) return;
    if (!cursor.get_cursor()->is(ScorePress::Class::CHORD)) return;
    ScorePress::Chord& chord = static_cast<ScorePress::Chord&>(*cursor.get_cursor());
    
    switch (code)
    {
    // shift direction
    case KEY_UP:
        for (ScorePress::HeadList::iterator head = chord.heads.begin(); head != chord.heads.end(); ++head)
            (*head)->tone += 1;
        cursor.set_accidental_auto();
        break;
    
    case KEY_DOWN:
        for (ScorePress::HeadList::iterator head = chord.heads.begin(); head != chord.heads.end(); ++head)
            (*head)->tone -= 1;
        cursor.set_accidental_auto();
        break;
    
    // ignore other codes (skip reengraving)
    default: return;
    };
    
    cursor.reengrave();
}

void KeyListener::action_move(const ActionKey code, ScorePress::EditCursor& cursor)
{
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
    
    cursor.reengrave();
}

#include <iostream>
void KeyListener::action_accmove(const ActionKey code, ScorePress::EditCursor& cursor)
{
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
    
    cursor.reengrave();
}

void KeyListener::action_staffshift(const ActionKey code, ScorePress::EditCursor& cursor)
{
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
    
    cursor.reengrave();
}

bool KeyListener::press(const Key key, ScorePress::EditCursor& cursor)
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
    case NORMAL:           action_on(codes[key], cursor);         break;
    case NDOT_INPUT:       /* see above */                        break;
    case STEMLENGTH_INPUT: action_stemlength(codes[key], cursor); break;
    case STEMDIR_INPUT:    action_stemdir(codes[key], cursor);    break;
    case CHROMATIC_INPUT:  action_chromatic(codes[key], cursor);  break;
    case MOVE_INPUT:       action_move(codes[key], cursor);       break;
    case ACCMOVE_INPUT:    action_accmove(codes[key], cursor);    break;
    case STAFFSHIFT_INPUT: action_staffshift(codes[key], cursor); break;
    };
    return true;
}

bool KeyListener::release(const Key key)
{
    if (!has_key(key)) return false;
    action_off(codes[key]);
    return true;
}

