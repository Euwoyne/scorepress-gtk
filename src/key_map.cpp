
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

#include "key_map.hh"

// key map setup
void KeyMap::assign(const ActionKey code, const Key key)
{
    reset(key);
    keys[code].push_back(key);
    codes[key] = code;
}

// reset all keys for action
void KeyMap::reset(const ActionKey code)
{
    for (std::list<Key>::iterator i = keys[code].begin(); i != keys[code].end(); ++i)
    {
        codes.erase(*i);
    };
    keys[code].clear();
}

// reset key mapping
void KeyMap::reset(const Key key)
{
    if (!has_key(key)) return;
    keys[codes[key]].remove(key);
    codes.erase(key);
}

