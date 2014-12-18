
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

#ifndef KEY_MAP_HH
#define KEY_MAP_HH

#include <list>
#include <map>

//
//     class KeyMap
//    ==============
//
// Defines all key-activated actions and maps key-codes to
// these action-codes.
//
class KeyMap
{
 public:
    // key definition
    class Key
    {
     public:
        unsigned int key;   // key code
        bool         ctrl;  // "ctrl" key flag
        
        Key(unsigned int _key = 0, bool _ctrl = false); // constructor
        
        bool operator <  (const Key& x) const;  // order definition (for use in "std::map")
        bool operator == (const Key& x) const;  // equality operator (for use in "std::map")
    };
    
    // action enumeration
    enum ActionKey {// cursor movement
                    KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT, KEY_HOME, KEY_END,
                    
                    // special action
                    KEY_NEWLINE, KEY_PAGEBREAK, KEY_NEWVOICE,
                    KEY_DELETE,  KEY_BACKSPACE, KEY_DELVOICE, KEY_DELBREAK,
                
                    // note value
                    KEY_LONGA, KEY_BREVE, KEY_WHOLE, KEY_HALF, KEY_QUARTER, KEY_EIGHTH,
                               KEY_16TH, KEY_32TH,  KEY_64TH, KEY_128TH,
                    
                    // note name
                    KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_A, KEY_B, KEY_REST, 

                    // accidentals
                    KEY_SHARP,  KEY_FLAT,  KEY_DOUBLESHARP,    KEY_DOUBLEFLAT,
                                           KEY_HALFSHARP,      KEY_HALFFLAT,
                                           KEY_SHARPANDAHALF,  KEY_FLATANDAHALF,

                    // octave modification
                    KEY_8VA, KEY_8VAB, KEY_OCTAVEUP, KEY_OCTAVEDOWN,
                    
                    // dot input
                    KEY_DOT, KEY_2DOT, KEY_NDOT,
                    
                    // note modification
                    KEY_STEMLENGTH, KEY_STEMDIR, KEY_CHROMATIC, KEY_MOVE, KEY_ACCMOVE, KEY_STAFFSHIFT,
                    KEY_NOBEAM, KEY_AUTOBEAM, KEY_FORCEBEAM, KEY_CUTBEAM,
                    
                    // mode modification
                    KEY_HEAD_MODE,
                    
                    // action count
                    KEY_COUNT};
    
    // input methods
    enum InsertMethod     {INSERT_ON_NAME, INSERT_ON_VALUE};    // insert method (i.e. what specification inserts the note)
    enum HeadInsertMethod {HEAD_SWITCH,    HEAD_HOLD};          // head mode method (i.e. switched on press or on press/release)
    
 protected:
    typedef std::list<Key>           KeyList;   // key list
    typedef std::map<Key, ActionKey> ActionMap; // action map (maps action-codes to keys)
    
    KeyList   keys[KEY_COUNT];      // map codes to keys
    ActionMap codes;                // map keys to action codes
    Key       digitkeys[10];        // map digit keys
    
    bool insert_on_name;            // input method (insert note on name- or on value-specification?)
    bool insert_head_hold;          // head insertion method (mode-switch / hold-modifier)
    
 public:
    // constructor
    KeyMap();
    
    // key map setup
    void assign(const ActionKey    action, const unsigned int key, const bool ctrl = false);
    void assign(const ActionKey    action, const Key key);
    void assign(const unsigned int digit,  const unsigned int key, const bool ctrl = false);
    void assign(const unsigned int digit,  const Key key);
    
    // reset methods
    void reset(const ActionKey action);
    void reset(const Key key);
    
    // key mapping access
          bool      has_key( const Key& key)          const;
          ActionKey get_code(const Key& key)          const;
    const KeyList&  get_keys(const ActionKey& action) const;
    
    // method flag interface
    void set_insert_method(const InsertMethod method);
    void set_head_insert_method(const HeadInsertMethod mode);
    
    InsertMethod     get_insert_method()      const;
    HeadInsertMethod get_head_insert_method() const;
};

// inline method implementations
inline KeyMap::Key::Key(unsigned int _key, bool _ctrl) : key(_key), ctrl(_ctrl) {}

inline bool KeyMap::Key::operator <  (const KeyMap::Key& x) const {return key < x.key || (key <= x.key && ctrl < x.ctrl);}
inline bool KeyMap::Key::operator == (const KeyMap::Key& x) const {return key == x.key && ctrl == x.ctrl;}

inline KeyMap::KeyMap() : insert_on_name(true), insert_head_hold(false) {}

inline void KeyMap::assign(const ActionKey    action,
                           const unsigned int key,
                           const bool         ctrl) {assign(action, Key(key, ctrl));}

inline void KeyMap::assign(const unsigned int digit,
                           const unsigned int key,
                           const bool         ctrl) {if (digit < 10) digitkeys[digit] = Key(key, ctrl);}

inline void KeyMap::assign(const unsigned int digit, const Key key) {if (digit < 10) digitkeys[digit] = key;}

inline       bool              KeyMap::has_key( const Key& key)     const {return codes.count(key);}
inline       KeyMap::ActionKey KeyMap::get_code(const Key& key)     const {return codes.find(key)->second;}
inline const KeyMap::KeyList&  KeyMap::get_keys(const ActionKey& a) const {return keys[a];}

inline void KeyMap::set_insert_method(     const InsertMethod     mode)   {insert_on_name   = (mode == INSERT_ON_NAME);}
inline void KeyMap::set_head_insert_method(const HeadInsertMethod mode)   {insert_head_hold = (mode == HEAD_HOLD);}

inline KeyMap::InsertMethod     KeyMap::get_insert_method()      const    {return (insert_on_name ? INSERT_ON_NAME : INSERT_ON_VALUE);}
inline KeyMap::HeadInsertMethod KeyMap::get_head_insert_method() const    {return (insert_head_hold ? HEAD_HOLD : HEAD_SWITCH);}

#endif

