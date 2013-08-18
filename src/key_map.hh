
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

#ifndef KEY_MAP_HH
#define KEY_MAP_HH

#include <list>
#include <map>

class KeyMap
{
 public:    
    class Key
    {
     public:
        unsigned int key;
        bool         ctrl;
        
        Key(unsigned int _key = 0, bool _ctrl = false) : key(_key), ctrl(_ctrl) {};
        inline bool operator< (const Key& x) const {return key < x.key || (key <= x.key && ctrl < x.ctrl);};
        inline bool operator==(const Key& x) const {return key == x.key && ctrl == x.ctrl;};
    };
    
    enum ActionKey {// cursor movement
                    KEY_UP,   KEY_DOWN,    KEY_RIGHT,  KEY_LEFT, KEY_HOME, KEY_END,
                    KEY_REST, KEY_NEWLINE, KEY_DELETE, KEY_BACKSPACE, KEY_DELVOICE,
                    
                    // note value
                    KEY_LONGA,  KEY_BREVE, KEY_WHOLE, KEY_HALF, KEY_QUARTER, KEY_EIGHTH,
                                KEY_16TH,  KEY_32TH,  KEY_64TH, KEY_128TH,
                    
                    // note name
                    KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_A, KEY_B,
                    
                    // accidentals
                    KEY_SHARP,  KEY_FLAT,  KEY_DOUBLESHARP,    KEY_DOUBLEFLAT,
                                           KEY_HALFSHARP,      KEY_HALFFLAT,
                                           KEY_SHARPANDAHALF,  KEY_FLATANDAHALF,
                    
                    // octave modification
                    KEY_8VA, KEY_8VAB, KEY_OCTAVEUP, KEY_OCTAVEDOWN,
                    
                    // dot input
                    KEY_DOT, KEY_2DOT, KEY_NDOT,
                    
                    // note modification
                    KEY_STEMLENGTH, KEY_STEMDIR,   KEY_CHROMATIC, KEY_MOVE, KEY_ACCMOVE, KEY_STAFFSHIFT,
                    KEY_NOBEAM,     KEY_AUTOBEAM,  KEY_FORCEBEAM, KEY_CUTBEAM,
                    
                    // mode modification
                    KEY_HEAD_MODE,
                    
                    // action count
                    KEY__COUNT__};
    
    enum InsertMethod     {INSERT_ON_NAME, INSERT_ON_VALUE};
    enum HeadInsertMethod {HEAD_SWITCH, HEAD_HOLD};
    
 protected:
    typedef std::list<Key> KeyList;
    typedef std::map<Key, ActionKey> ActionMap;
    
    KeyList   keys[KEY__COUNT__];   // map codes to keys
    ActionMap codes;                // map keys to action codes
    Key       digitkeys[10];        // map digit keys
    
    bool insert_on_name;            // input method (insert note on name- or on value-specification?)
    bool insert_head_hold;          // head insertion method (mode-switch / hold-modifier)
    
 public:
    KeyMap();
    
    void assign(const ActionKey action, const unsigned int key, const bool ctrl = false);
    void assign(const ActionKey action, const Key key);
    void reset(const ActionKey action);
    void reset(const Key key);
    
    void set_digit_key(const unsigned int digit, const Key key);
    
          bool      has_key(const Key& key)           const;
    const ActionKey get_code(const Key& key)          const;
    const KeyList&  get_keys(const ActionKey& action) const;

    void set_insert_method(const InsertMethod method);
    void set_head_insert_method(const HeadInsertMethod mode);
    
    InsertMethod     get_insert_method()      const;
    HeadInsertMethod get_head_insert_method() const;
};

inline void KeyMap::assign(const ActionKey action, const unsigned int key, const bool ctrl)
    {assign(action, Key(key, ctrl));}

inline void KeyMap::set_digit_key(const unsigned int digit, const Key key)
    {if (digit < 10) digitkeys[digit] = key;}

inline bool KeyMap::has_key(const Key& key) const
    {return codes.count(key);}
inline const KeyMap::ActionKey KeyMap::get_code(const Key& key) const
    {return codes.find(key)->second;}
inline const KeyMap::KeyList&  KeyMap::get_keys(const ActionKey& action) const
    {return keys[action];}

inline void KeyMap::set_insert_method(const InsertMethod method)
    {insert_on_name = (method == INSERT_ON_NAME);}
inline void KeyMap::set_head_insert_method(const HeadInsertMethod mode)
    {insert_head_hold = (mode == HEAD_HOLD);}

inline KeyMap::InsertMethod KeyMap::get_insert_method() const
    {return (insert_on_name ? INSERT_ON_NAME : INSERT_ON_VALUE);}
inline KeyMap::HeadInsertMethod KeyMap::get_head_insert_method() const
    {return (insert_head_hold ? HEAD_HOLD : HEAD_SWITCH);}
 
#endif
