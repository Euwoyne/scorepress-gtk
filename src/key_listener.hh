
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
#include <scorepress/engine.hh>

class Controller;

class KeyListener : public KeyMap
{
 public:
    enum Mode {NORMAL, NDOT_INPUT, STEMLENGTH_INPUT, STEMDIR_INPUT, CHROMATIC_INPUT, MOVE_INPUT, ACCMOVE_INPUT, STAFFSHIFT_INPUT};
    
 private:
    ScorePress::EditCursor::InputNote note;
    
    Mode mode;              // current mode (for keys used in pairs)
    bool head_input;        // flag for head-insert-mode
    bool got_home;          // just read KEY_HOME (consequent home-stoke results in movement to line-front)
    bool got_end;           // just read KEY_END  (consequent end-stoke results in movement to line-end)
    bool got_octaveup;
    bool got_octavedown;
    
 private:
    void insert(Controller& controller);
    void insert_rest(Controller& controller);
    
 public:
    KeyListener();
    
    void action_on(         const ActionKey code, Controller& controller);
    void action_off(        const ActionKey code);
    void action_stemlength( const ActionKey code, Controller& controller);
    void action_stemdir(    const ActionKey code, Controller& controller);
    void action_chromatic(  const ActionKey code, Controller& controller);
    void action_move(       const ActionKey code, Controller& controller);
    void action_accmove(    const ActionKey code, Controller& controller);
    void action_staffshift( const ActionKey code, Controller& controller);
    
    bool press(const Key key, Controller& controller);
    bool release(const Key key);
};

#endif

