
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

#include "key_map.hh"

KeyMap::KeyMap() : actionMap(    on_assign_signal, on_reset_signal),
                   ndotMap(      on_assign_signal, on_reset_signal),
                   stemlengthMap(on_assign_signal, on_reset_signal),
                   stemdirMap(   on_assign_signal, on_reset_signal),
                   chromaticMap( on_assign_signal, on_reset_signal),
                   moveMap(      on_assign_signal, on_reset_signal),
                   accmoveMap(   on_assign_signal, on_reset_signal),
                   staffshiftMap(on_assign_signal, on_reset_signal),
                   beamMap(      on_assign_signal, on_reset_signal),
                   insert_on_name(true),
                   insert_head_hold(false) {}

void KeyMap::assign(const KeyMap::ActionId action, const unsigned int key, const unsigned int mods)
{
    switch (action.get_context())
    {
    case CTX_DEFAULT:    actionMap.assign(static_cast<ActionKey>(action.get_action()), key, mods); break;
    case CTX_HEAD:       break;
    case CTX_NDOT:       ndotMap.assign(static_cast<NDot>(action.get_action()), key, mods); break;
    case CTX_STEMLENGTH: stemlengthMap.assign(static_cast<StemLength>(action.get_action()), key, mods); break;
    case CTX_STEMDIR:    stemdirMap.assign(static_cast<StemDir>(action.get_action()), key, mods); break;
    case CTX_CHROMATIC:  chromaticMap.assign(static_cast<Chromatic>(action.get_action()), key, mods); break;
    case CTX_MOVE:       moveMap.assign(static_cast<Move>(action.get_action()), key, mods); break;
    case CTX_ACCMOVE:    accmoveMap.assign(static_cast<AccMove>(action.get_action()), key, mods); break;
    case CTX_STAFFSHIFT: staffshiftMap.assign(static_cast<StaffShift>(action.get_action()), key, mods); break;
    case CTX_BEAM:       beamMap.assign(static_cast<Beam>(action.get_action()), key, mods); break;
    };
}

void KeyMap::assign(const KeyMap::ActionId action, const KeyMap::Key key)
{
    switch (action.get_context())
    {
    case CTX_DEFAULT:    actionMap.assign(static_cast<ActionKey>(action.get_action()), key); break;
    case CTX_HEAD:       break;
    case CTX_NDOT:       ndotMap.assign(static_cast<NDot>(action.get_action()), key); break;
    case CTX_STEMLENGTH: stemlengthMap.assign(static_cast<StemLength>(action.get_action()), key); break;
    case CTX_STEMDIR:    stemdirMap.assign(static_cast<StemDir>(action.get_action()), key); break;
    case CTX_CHROMATIC:  chromaticMap.assign(static_cast<Chromatic>(action.get_action()), key); break;
    case CTX_MOVE:       moveMap.assign(static_cast<Move>(action.get_action()), key); break;
    case CTX_ACCMOVE:    accmoveMap.assign(static_cast<AccMove>(action.get_action()), key); break;
    case CTX_STAFFSHIFT: staffshiftMap.assign(static_cast<StaffShift>(action.get_action()), key); break;
    case CTX_BEAM:       beamMap.assign(static_cast<Beam>(action.get_action()), key); break;
    };
}

void KeyMap::reset(const KeyMap::ActionId action)
{
    switch (action.get_context())
    {
    case CTX_DEFAULT:    actionMap.reset(static_cast<ActionKey>(action.get_action())); break;
    case CTX_HEAD:       break;
    case CTX_NDOT:       ndotMap.reset(static_cast<NDot>(action.get_action())); break;
    case CTX_STEMLENGTH: stemlengthMap.reset(static_cast<StemLength>(action.get_action())); break;
    case CTX_STEMDIR:    stemdirMap.reset(static_cast<StemDir>(action.get_action())); break;
    case CTX_CHROMATIC:  chromaticMap.reset(static_cast<Chromatic>(action.get_action())); break;
    case CTX_MOVE:       moveMap.reset(static_cast<Move>(action.get_action())); break;
    case CTX_ACCMOVE:    accmoveMap.reset(static_cast<AccMove>(action.get_action())); break;
    case CTX_STAFFSHIFT: staffshiftMap.reset(static_cast<StaffShift>(action.get_action())); break;
    case CTX_BEAM:       beamMap.reset(static_cast<Beam>(action.get_action())); break;
    };
}

bool KeyMap::has_key(const Key& key, const Context& context) const
{
    switch (context)
    {
    case CTX_DEFAULT:    // has the same keys as CTX_HEAD
    case CTX_HEAD:       return actionMap.exist(key);
    case CTX_NDOT:       return ndotMap.exist(key);
    case CTX_STEMLENGTH: return stemlengthMap.exist(key);
    case CTX_STEMDIR:    return stemdirMap.exist(key);
    case CTX_CHROMATIC:  return chromaticMap.exist(key);
    case CTX_MOVE:       return moveMap.exist(key);
    case CTX_ACCMOVE:    return accmoveMap.exist(key);
    case CTX_STAFFSHIFT: return staffshiftMap.exist(key);
    case CTX_BEAM:       return beamMap.exist(key);
    default:             return false;
    }
}

