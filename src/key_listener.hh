
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

#ifndef KEY_LISTENER_HH
#define KEY_LISTENER_HH

#include "key_map.hh"
#include <scorepress/basetypes.hh>      // spohh_t, spohw_t
#include <scorepress/edit_cursor.hh>    // EditCursor

class Controller;   // Controller class prototype

//
//     class KeyListener
//    ===================
//
// Receives keystrokes from "View" classes (i.e. "MainWnd" and "ScoreWidget")
// and triggers the necessary changes in the "Engine" and calls necessary
// signal-handlers in "Controller" to update the view.
//
class KeyListener : public KeyMap
{
 public:
    // input mode
    enum Mode {NORMAL, HEAD, NDOT, STEMLENGTH, STEMDIR, CHROMATIC, MOVE, ACCMOVE, STAFFSHIFT};
    
 private:
    // input note status
    ScorePress::EditCursor::InputNote note;
    
    // current edit mode and flags
    Mode mode;              // current mode (for keys used in pairs)
    bool got_home;          // just read    KEY_HOME (consequent home-stoke results in movement to line-front)
    bool got_end;           // just read    KEY_END  (consequent end-stoke results in movement to line-end)
    bool got_8va;           // just pressed KEY_8VA  (wait for release)
    bool got_8vab;          // just pressed KEY_8VAB (wait for release)
    
 private:
    // insert helper functions
    void insert(     Controller& controller);   // insert current input-note
    void insert_rest(Controller& controller);   // insert rest
    
 public:
    // modification steps
    ScorePress::spohh_t stem_step;              // step for stem-length modification
    ScorePress::spohw_t note_step;              // step for x-offset modification
    ScorePress::spohw_t rest_step;              // step for y-offset modification
    
    // constructor
    KeyListener();
    
    // signal handlers
    bool press(  const Key key, Controller& controller);    // on key press
    bool release(const Key key);                            // on key release
    
    // signal handler helpers (mode dependant)
    void action_off(        const ActionKey code);                          // key release
    
    void action_on(         const ActionKey code, Controller& controller);  // mode: NORMAL
    void action_stemlength( const ActionKey code, Controller& controller);  // mode: STEMLENGTH_INPUT
    void action_stemdir(    const ActionKey code, Controller& controller);  // mode: STEMDIR_INPUT
    void action_chromatic(  const ActionKey code, Controller& controller);  // mode: CHROMATIC_INPUT
    void action_move(       const ActionKey code, Controller& controller);  // mode: MOVE_INPUT
    void action_accmove(    const ActionKey code, Controller& controller);  // mode: ACCMOVE_INPUT
    void action_staffshift( const ActionKey code, Controller& controller);  // mode: STAFFSHIFT_INPUT
};

#endif

