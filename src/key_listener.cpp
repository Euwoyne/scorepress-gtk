
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

#include "key_listener.hh"
#include "controller.hh"

// insert current input-note
void KeyListener::insert(Controller& controller)
{
    if (context == KeyMap::CTX_HEAD)
        controller.get_cursor().insert_head(note);
    else
        controller.get_cursor().insert(note);
    note.dots = 0;
    note.accidental = ScorePress::Accidental::natural;
    controller.reengrave();
    if (context == KeyMap::CTX_HEAD) return;
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
KeyListener::KeyListener(const KeyMap& kmap) : keymap(kmap),
                                               context(KeyMap::CTX_DEFAULT),
                                               got_home(false),
                                               got_end(false),
                                               got_8va(false),
                                               got_8vab(false),
                                               stem_step(500),
                                               note_step(500),
                                               rest_step(500) {}

// signal handler (on key press)
bool KeyListener::press(const KeyMap::Key key, Controller& controller)
{
    if (!controller.has_cursor()) return false;
    
    try
    {
        switch (context)
        {
        case KeyMap::CTX_DEFAULT:
        case KeyMap::CTX_HEAD:       action_on(keymap.get_action<KeyMap::ActionKey> (key), controller); break;
        case KeyMap::CTX_NDOT:       action_on(keymap.get_action<KeyMap::NDot>      (key), controller); break;
        case KeyMap::CTX_STEMLENGTH: action_on(keymap.get_action<KeyMap::StemLength>(key), controller); break;
        case KeyMap::CTX_STEMDIR:    action_on(keymap.get_action<KeyMap::StemDir>   (key), controller); break;
        case KeyMap::CTX_CHROMATIC:  action_on(keymap.get_action<KeyMap::Chromatic> (key), controller); break;
        case KeyMap::CTX_MOVE:       action_on(keymap.get_action<KeyMap::Move>      (key), controller); break;
        case KeyMap::CTX_ACCMOVE:    action_on(keymap.get_action<KeyMap::AccMove>   (key), controller); break;
        case KeyMap::CTX_STAFFSHIFT: action_on(keymap.get_action<KeyMap::StaffShift>(key), controller); break;
        case KeyMap::CTX_BEAM:       action_on(keymap.get_action<KeyMap::Beam>      (key), controller); break;
        };
        return true;
    }
    catch (std::out_of_range&)
    {
        return false;
    };
}

// signal handler (on key release)
bool KeyListener::release(const KeyMap::Key key)
{
    try
    {
        action_off(keymap.get_action<KeyMap::ActionKey>(key));
        return true;
    }
    catch (std::out_of_range&)
    {
        return false;
    };
}

// DEFAULT context signal handler
void KeyListener::action_on(const KeyMap::ActionKey action, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    
    switch (action)
    {
    // cursor movement
    case KeyMap::KEY_UP:
        if      (cursor.has_prev_voice()) cursor.prev_voice();
        else if (cursor.has_prev_line())  cursor.prev_line();
        controller.redraw();
        break;
    
    case KeyMap::KEY_DOWN:
        if      (cursor.has_next_voice()) cursor.next_voice();
        else if (cursor.has_next_line())  cursor.next_line();
        controller.redraw();
        break;
    
    case KeyMap::KEY_RIGHT:
        if (cursor.at_end()) break;
        cursor.next();
        controller.redraw();
        break;
    
    case KeyMap::KEY_LEFT:
        if (!cursor.has_prev()) break;
        cursor.prev();
        controller.redraw();
        break;
    
    case KeyMap::KEY_HOME:
        if (got_home) cursor.home();
        else cursor.home_voice();
        got_home = false;
        controller.redraw();
        break;
                    
    case KeyMap::KEY_END:
        if (got_end) cursor.end();
        else cursor.end_voice(); 
        got_end  = false;
        controller.redraw();
        break;
    
    // special action
    case KeyMap::KEY_NEWLINE:
        cursor.insert_newline();
        controller.on_resize();
        cursor.next_line_home();
        controller.redraw();
        break;
    
    case KeyMap::KEY_PAGEBREAK:
        cursor.insert_pagebreak();
        controller.on_resize();
        cursor.next_line_home();
        controller.redraw();
        break;
    
    case KeyMap::KEY_NEWVOICE:
        //cursor.add_voice_below();   // TODO: new context for above/below (and maybe top/bottom)
        //cursor.update_voices();
        //controller.on_resize();
        break;
    
    case KeyMap::KEY_DELETE:
        if (cursor.at_end()) break;
        cursor.remove();
        controller.reengrave();
        break;
    
    case KeyMap::KEY_BACKSPACE:
        if (!cursor.has_prev()) break;
        cursor.prev();
        cursor.remove();
        controller.reengrave();
        break;
    
    case KeyMap::KEY_DELVOICE:
        cursor.remove_voice();
        controller.reengrave();
        break;
    
    case KeyMap::KEY_DELBREAK:
        cursor.remove_break();
        controller.reengrave();
        break;
    
    // note value
    case KeyMap::KEY_LONGA:   note.exp = ScorePress::VALUE_BASE + 2; if (keymap.inserts_on_value()) insert(controller); break;
    case KeyMap::KEY_BREVE:   note.exp = ScorePress::VALUE_BASE + 1; if (keymap.inserts_on_value()) insert(controller); break;
    case KeyMap::KEY_WHOLE:   note.exp = ScorePress::VALUE_BASE;     if (keymap.inserts_on_value()) insert(controller); break;
    case KeyMap::KEY_HALF:    note.exp = ScorePress::VALUE_BASE - 1; if (keymap.inserts_on_value()) insert(controller); break;
    case KeyMap::KEY_QUARTER: note.exp = ScorePress::VALUE_BASE - 2; if (keymap.inserts_on_value()) insert(controller); break;
    case KeyMap::KEY_EIGHTH:  note.exp = ScorePress::VALUE_BASE - 3; if (keymap.inserts_on_value()) insert(controller); break;
    case KeyMap::KEY_16TH:    note.exp = ScorePress::VALUE_BASE - 4; if (keymap.inserts_on_value()) insert(controller); break;
    case KeyMap::KEY_32TH:    note.exp = ScorePress::VALUE_BASE - 5; if (keymap.inserts_on_value()) insert(controller); break;
    case KeyMap::KEY_64TH:    note.exp = ScorePress::VALUE_BASE - 6; if (keymap.inserts_on_value()) insert(controller); break;
    case KeyMap::KEY_128TH:   note.exp = ScorePress::VALUE_BASE - 7; if (keymap.inserts_on_value()) insert(controller); break;
    
    case KeyMap::KEY_REST:
        insert_rest(controller);
        break;
    
    // note name
    case KeyMap::KEY_C: note.name = ScorePress::EditCursor::C; if (keymap.inserts_on_name()) insert(controller); break;
    case KeyMap::KEY_D: note.name = ScorePress::EditCursor::D; if (keymap.inserts_on_name()) insert(controller); break;
    case KeyMap::KEY_E: note.name = ScorePress::EditCursor::E; if (keymap.inserts_on_name()) insert(controller); break;
    case KeyMap::KEY_F: note.name = ScorePress::EditCursor::F; if (keymap.inserts_on_name()) insert(controller); break;
    case KeyMap::KEY_G: note.name = ScorePress::EditCursor::G; if (keymap.inserts_on_name()) insert(controller); break;
    case KeyMap::KEY_A: note.name = ScorePress::EditCursor::A; if (keymap.inserts_on_name()) insert(controller); break;
    case KeyMap::KEY_B: note.name = ScorePress::EditCursor::B; if (keymap.inserts_on_name()) insert(controller); break;
    
    // accidentals
    case KeyMap::KEY_SHARP:         note.accidental = ScorePress::Accidental::sharp;         break;
    case KeyMap::KEY_FLAT:          note.accidental = ScorePress::Accidental::flat;          break;
    case KeyMap::KEY_DOUBLESHARP:   note.accidental = ScorePress::Accidental::double_sharp;  break;
    case KeyMap::KEY_DOUBLEFLAT:    note.accidental = ScorePress::Accidental::double_flat;   break;
    case KeyMap::KEY_HALFSHARP:     note.accidental = ScorePress::Accidental::half_sharp;    break;
    case KeyMap::KEY_HALFFLAT:      note.accidental = ScorePress::Accidental::half_flat;     break;
    case KeyMap::KEY_SHARPANDAHALF: note.accidental = ScorePress::Accidental::flat_andahalf; break;
    case KeyMap::KEY_FLATANDAHALF:  note.accidental = ScorePress::Accidental::flat_andahalf; break;
    
    // octave modification
    case KeyMap::KEY_8VA:      got_8va = true;      // continue with OCTAVEUP
    case KeyMap::KEY_OCTAVEUP: ++note.octave;
                               break;
    
    case KeyMap::KEY_8VAB:       got_8vab = true;   // continue with OCTAVEDOWN
    case KeyMap::KEY_OCTAVEDOWN: --note.octave;
                                 break;
    
    // dot input
    case KeyMap::KEY_NDOT: context = KeyMap::CTX_NDOT; break;
    case KeyMap::KEY_1DOT: note.dots = 1; break;
    case KeyMap::KEY_2DOT: note.dots = 2; break;
    case KeyMap::KEY_3DOT: note.dots = 3; break;
    case KeyMap::KEY_4DOT: note.dots = 4; break;
    case KeyMap::KEY_5DOT: note.dots = 5; break;
    case KeyMap::KEY_6DOT: note.dots = 6; break;
    case KeyMap::KEY_7DOT: note.dots = 7; break;
    case KeyMap::KEY_8DOT: note.dots = 8; break;
    case KeyMap::KEY_9DOT: note.dots = 9; break;
    
    // stem-length mode
    case KeyMap::KEY_STEMLENGTH:          context = KeyMap::CTX_STEMLENGTH; break;
    case KeyMap::KEY_STEMLENGTH_UP:       action_on(KeyMap::STEMLENGTH_UP,       controller); break;
    case KeyMap::KEY_STEMLENGTH_DOWN:     action_on(KeyMap::STEMLENGTH_DOWN,     controller); break;
    case KeyMap::KEY_STEMLENGTH_BEAMUP:   action_on(KeyMap::STEMLENGTH_BEAMUP,   controller); break;
    case KeyMap::KEY_STEMLENGTH_BEAMDOWN: action_on(KeyMap::STEMLENGTH_BEAMDOWN, controller); break;
    case KeyMap::KEY_STEMLENGTH_AUTO:     action_on(KeyMap::STEMLENGTH_AUTO,     controller); break;
    
    // stem-direction mode
    case KeyMap::KEY_STEMDIR:      context = KeyMap::CTX_STEMDIR; break;
    case KeyMap::KEY_STEMDIR_UP:   action_on(KeyMap::STEMDIR_UP,   controller); break;
    case KeyMap::KEY_STEMDIR_DOWN: action_on(KeyMap::STEMDIR_DOWN, controller); break;
    case KeyMap::KEY_STEMDIR_AUTO: action_on(KeyMap::STEMDIR_AUTO, controller); break;
    
    // chromatic note shift
    case KeyMap::KEY_CHROMATIC:      context = KeyMap::CTX_CHROMATIC; break;
    case KeyMap::KEY_CHROMATIC_UP:   action_on(KeyMap::CHROMATIC_UP,   controller); break;
    case KeyMap::KEY_CHROMATIC_DOWN: action_on(KeyMap::CHROMATIC_DOWN, controller); break;
    
    // note-object movement
    case KeyMap::KEY_MOVE:       context = KeyMap::CTX_MOVE; break;
    case KeyMap::KEY_MOVE_UP:    action_on(KeyMap::MOVE_UP,    controller); break;
    case KeyMap::KEY_MOVE_DOWN:  action_on(KeyMap::MOVE_DOWN,  controller); break;
    case KeyMap::KEY_MOVE_LEFT:  action_on(KeyMap::MOVE_LEFT,  controller); break;
    case KeyMap::KEY_MOVE_RIGHT: action_on(KeyMap::MOVE_RIGHT, controller); break;
    case KeyMap::KEY_MOVE_HOME:  action_on(KeyMap::MOVE_HOME,  controller); break;
    
    // accidental movement
    case KeyMap::KEY_ACCMOVE:       context = KeyMap::CTX_ACCMOVE; break;
    case KeyMap::KEY_ACCMOVE_LEFT:  action_on(KeyMap::ACCMOVE_LEFT,  controller); break;
    case KeyMap::KEY_ACCMOVE_RIGHT: action_on(KeyMap::ACCMOVE_RIGHT, controller); break;
    case KeyMap::KEY_ACCMOVE_HOME:  action_on(KeyMap::ACCMOVE_HOME,  controller); break;
    
    // staff shifting
    case KeyMap::KEY_STAFFSHIFT:      context = KeyMap::CTX_STAFFSHIFT; break;
    case KeyMap::KEY_STAFFSHIFT_UP:   action_on(KeyMap::STAFFSHIFT_UP,   controller); break;
    case KeyMap::KEY_STAFFSHIFT_DOWN: action_on(KeyMap::STAFFSHIFT_DOWN, controller); break;
    case KeyMap::KEY_STAFFSHIFT_HOME: action_on(KeyMap::STAFFSHIFT_HOME, controller); break;
    
    // beam modification
    case KeyMap::KEY_BEAM:        context = KeyMap::CTX_BEAM; break;
    case KeyMap::KEY_BEAM_NONE:   action_on(KeyMap::BEAM_NONE,   controller);  break;
    case KeyMap::KEY_BEAM_AUTO:   action_on(KeyMap::BEAM_AUTO,   controller);  break;
    case KeyMap::KEY_BEAM_FORCED: action_on(KeyMap::BEAM_FORCED, controller); break;
    case KeyMap::KEY_BEAM_CUT:    action_on(KeyMap::BEAM_CUT,    controller);   break;
    
    // head insertion mode
    case KeyMap::KEY_HEAD_MODE:
        context = ((context == KeyMap::CTX_HEAD) ? KeyMap::CTX_DEFAULT : KeyMap::CTX_HEAD);
        break;
    
    // ignore key-count indicator (shouldn't be set anyway)
    case KeyMap::KEY_COUNT:
        controller.log_warn("Got illegal action id KEY_COUNT. (class: KeyListener)");
        break;
    };
}

// NDOT context signal handler
void KeyListener::action_on(const KeyMap::NDot action, Controller& controller)
{
    // check key
    switch (action)
    {
    case KeyMap::NDOT_0: note.dots = 1; break;
    case KeyMap::NDOT_1: note.dots = 1; break;
    case KeyMap::NDOT_2: note.dots = 2; break;
    case KeyMap::NDOT_3: note.dots = 3; break;
    case KeyMap::NDOT_4: note.dots = 4; break;
    case KeyMap::NDOT_5: note.dots = 5; break;
    case KeyMap::NDOT_6: note.dots = 6; break;
    case KeyMap::NDOT_7: note.dots = 7; break;
    case KeyMap::NDOT_8: note.dots = 8; break;
    case KeyMap::NDOT_9: note.dots = 9; break;
    case KeyMap::NDOT_COUNT:            return;
    };
    
    // signal changes
    controller.reengrave();
}

// STEMLENGTH context signal handler
void KeyListener::action_on(const KeyMap::StemLength action, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    
    // check cursor validity
    if (cursor.at_end()) return;
    if (!cursor.ready()) return;
    if (!cursor.get_cursor()->is(ScorePress::Class::CHORD)) return;
    
    // check key
    switch (action)
    {
    case KeyMap::STEMLENGTH_UP:       cursor.add_stem_length( stem_step); break;
    case KeyMap::STEMLENGTH_DOWN:     cursor.add_stem_length(-stem_step); break;
    case KeyMap::STEMLENGTH_BEAMUP:   cursor.add_stem_slope( stem_step);  break;
    case KeyMap::STEMLENGTH_BEAMDOWN: cursor.add_stem_slope(-stem_step);  break;
    case KeyMap::STEMLENGTH_AUTO:     cursor.set_stem_type(ScorePress::Chord::STEM_VOICE);
                                      cursor.set_slope_type(ScorePress::Chord::SLOPE_AUTO); break;
    case KeyMap::STEMLENGTH_COUNT:    return;
    };
    
    // signal changes
    controller.reengrave();
}

// STEMDIR context signal handler
void KeyListener::action_on(const KeyMap::StemDir action, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    
    // check cursor validity
    if (cursor.at_end()) return;
    if (!cursor.ready()) return;
    if (!cursor.get_cursor()->is(ScorePress::Class::CHORD)) return;
    
    // check key
    switch (action)
    {
    case KeyMap::STEMDIR_UP:    cursor.set_stem_dir(false); break;
    case KeyMap::STEMDIR_DOWN:  cursor.set_stem_dir(true);  break;
    case KeyMap::STEMDIR_AUTO:  cursor.set_stem_type(ScorePress::Chord::STEM_AUTO); break;
    case KeyMap::STEMDIR_COUNT: return;
    };
    
    // signal changes
    controller.reengrave();
}

// CHROMATIC context signal handler
void KeyListener::action_on(const KeyMap::Chromatic action, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    
    // check cursor validity
    if (cursor.at_end()) return;
    if (!cursor.ready()) return;
    if (!cursor.get_cursor()->is(ScorePress::Class::CHORD)) return;
    ScorePress::Chord& chord = static_cast<ScorePress::Chord&>(*cursor.get_cursor());
    
    // check key
    switch (action)
    {
    case KeyMap::CHROMATIC_UP:
        for (ScorePress::HeadList::iterator head = chord.heads.begin(); head != chord.heads.end(); ++head)
            ++(*head)->tone;
        cursor.set_accidental_auto();
        break;
    
    case KeyMap::CHROMATIC_DOWN:
        for (ScorePress::HeadList::iterator head = chord.heads.begin(); head != chord.heads.end(); ++head)
            --(*head)->tone;
        cursor.set_accidental_auto();
        break;
    
    case KeyMap::CHROMATIC_COUNT: return;
    };
    
    // signal changes
    controller.reengrave();
}

// MOVE context signal handler
void KeyListener::action_on(const KeyMap::Move action, Controller& controller)
{
    // check cursor validity
    ScorePress::EditCursor& cursor = controller.get_cursor();
    if (!cursor.ready()) return;
    if (cursor.at_end()) return;
    
    // check key
    switch (action)
    {
    case KeyMap::MOVE_UP:
        if (cursor.get_cursor()->is(ScorePress::Class::REST))
            static_cast<ScorePress::Rest&>(*cursor.get_cursor()).offset_y -= rest_step;
        else
            return;
        break;
    
    case KeyMap::MOVE_DOWN:
        if (cursor.get_cursor()->is(ScorePress::Class::REST))
            static_cast<ScorePress::Rest&>(*cursor.get_cursor()).offset_y += rest_step;
        else
            return;
        break;
    
    case KeyMap::MOVE_LEFT:
        if (cursor.get_cursor()->is(ScorePress::Class::VISIBLEOBJECT))
            cursor.get_cursor()->get_visible().offset_x -= note_step;
        else
            return;
        break;
    
    case KeyMap::MOVE_RIGHT:
        if (cursor.get_cursor()->is(ScorePress::Class::VISIBLEOBJECT))
            cursor.get_cursor()->get_visible().offset_x += note_step;
        else
            return;
        break;
    
    case KeyMap::MOVE_HOME:
        if (cursor.get_cursor()->is(ScorePress::Class::VISIBLEOBJECT))
        {
            cursor.get_cursor()->get_visible().offset_x = 0;
            if (cursor.get_cursor()->is(ScorePress::Class::REST))
                static_cast<ScorePress::Rest&>(*cursor.get_cursor()).offset_y = 0;
            break;
        };
        return;
    
    case KeyMap::MOVE_COUNT: return;
    };
    
    // signal changes
    controller.reengrave();
}

// ACCMOVE context signal handler
void KeyListener::action_on(const KeyMap::AccMove action, Controller& controller)
{
    // check cursor validity
    ScorePress::EditCursor& cursor = controller.get_cursor();
    if (cursor.at_end()) return;
    
    // check key
    switch (action)
    {
    case KeyMap::ACCMOVE_LEFT:  cursor.get_cursor()->acc_offset -= note_step; break;
    case KeyMap::ACCMOVE_RIGHT: cursor.get_cursor()->acc_offset += note_step; break;
    case KeyMap::ACCMOVE_HOME:  cursor.get_cursor()->acc_offset  = 0;         break;
    case KeyMap::ACCMOVE_COUNT: return;
    };
    
    // signal changes
    controller.reengrave();
}

// STAFFSHIFT context signal handler
void KeyListener::action_on(const KeyMap::StaffShift action, Controller& controller)
{
    ScorePress::EditCursor& cursor = controller.get_cursor();
    
    // check cursor validity
    if (cursor.at_end()) return;
    if (!cursor.get_cursor()->is(ScorePress::Class::NOTEOBJECT)) return;
    
    // check key
    switch (action)
    {
    case KeyMap::STAFFSHIFT_UP:
        static_cast<ScorePress::NoteObject&>(*cursor.get_cursor()).staff_shift -= 1;
        break;
    
    case KeyMap::STAFFSHIFT_DOWN:
        static_cast<ScorePress::NoteObject&>(*cursor.get_cursor()).staff_shift += 1;
        break;
    
    case KeyMap::STAFFSHIFT_HOME:
        static_cast<ScorePress::NoteObject&>(*cursor.get_cursor()).staff_shift = 0;
        break;
    
    case KeyMap::STAFFSHIFT_COUNT: return;
    };
    
    // signal changes
    controller.reengrave();
}

// BEAM context signal handler
void KeyListener::action_on(const KeyMap::Beam action, Controller& controller)
{
    // get cursor to the nearest previous chord
    ScorePress::Cursor cursor = controller.get_cursor().get_cursor();
    if (!cursor.has_prev()) return;
    do --cursor; while (cursor.has_prev() && !cursor->is(ScorePress::Class::CHORD));
    if (!cursor->is(ScorePress::Class::CHORD)) return;
    
    // if we have got a chord, set the beam property
    ScorePress::Chord& chord = static_cast<ScorePress::Chord&>(*cursor);
    switch (action)
    {
    case KeyMap::BEAM_NONE:   chord.beam = ScorePress::Chord::BEAM_NONE;   break;
    case KeyMap::BEAM_AUTO:   chord.beam = ScorePress::Chord::BEAM_AUTO;   break;
    case KeyMap::BEAM_FORCED: chord.beam = ScorePress::Chord::BEAM_FORCED; break;
    case KeyMap::BEAM_CUT:    chord.beam = ScorePress::Chord::BEAM_CUT;    break;
    case KeyMap::BEAM_COUNT:  return;
    };
    
    // signal changes
    controller.reengrave();
}

// key release signal handler
void KeyListener::action_off(const KeyMap::ActionKey action)
{
    switch (action)
    {
    // octave modification
    case KeyMap::KEY_8VA:
        if (!got_8va) break;
        --note.octave;
        got_8va = false;
        break;
    
    case KeyMap::KEY_8VAB:
        if (!got_8vab) break;
        ++note.octave;
        got_8vab = false;
        break;
    
    // mode modification
    case KeyMap::KEY_NDOT:       if (context == KeyMap::CTX_NDOT)       context = KeyMap::CTX_DEFAULT; break;
    case KeyMap::KEY_STEMLENGTH: if (context == KeyMap::CTX_STEMLENGTH) context = KeyMap::CTX_DEFAULT; break;
    case KeyMap::KEY_STEMDIR:    if (context == KeyMap::CTX_STEMDIR)    context = KeyMap::CTX_DEFAULT; break;
    case KeyMap::KEY_CHROMATIC:  if (context == KeyMap::CTX_CHROMATIC)  context = KeyMap::CTX_DEFAULT; break;
    case KeyMap::KEY_MOVE:       if (context == KeyMap::CTX_MOVE)       context = KeyMap::CTX_DEFAULT; break;
    case KeyMap::KEY_ACCMOVE:    if (context == KeyMap::CTX_ACCMOVE)    context = KeyMap::CTX_DEFAULT; break;
    case KeyMap::KEY_STAFFSHIFT: if (context == KeyMap::CTX_STAFFSHIFT) context = KeyMap::CTX_DEFAULT; break;
    case KeyMap::KEY_BEAM:       if (context == KeyMap::CTX_BEAM)       context = KeyMap::CTX_DEFAULT; break;
    case KeyMap::KEY_HEAD_MODE:  if (context == KeyMap::CTX_HEAD && keymap.head_on_hold())
                                                                        context = KeyMap::CTX_DEFAULT; break;
    // ignore other codes
    default: break;
    };
}

