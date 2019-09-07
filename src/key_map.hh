
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

#ifndef KEY_MAP_HH
#define KEY_MAP_HH

#include <list>
#include <map>
#include <sigc++/sigc++.h>

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
    // action enumeration
    enum ActionKey {// cursor movement
                    KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT, KEY_HOME, KEY_END,
                    
                    // special action
                    KEY_NEWLINE, KEY_PAGEBREAK, KEY_NEWVOICE,
                    KEY_DELETE,  KEY_BACKSPACE, KEY_DELVOICE, KEY_DELBREAK,
                
                    // note value
                    KEY_LONGA, KEY_BREVE, KEY_WHOLE, KEY_HALF, KEY_QUARTER, KEY_EIGHTH,
                               KEY_16TH, KEY_32TH,  KEY_64TH, KEY_128TH, KEY_256TH,
                    
                    // note name
                    KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_A, KEY_B, KEY_REST, 
                    
                    // mode modification
                    KEY_HEAD_MODE,
                    
                    // accidentals
                    KEY_SHARP,  KEY_FLAT,  KEY_DOUBLESHARP,    KEY_DOUBLEFLAT,
                                           KEY_HALFSHARP,      KEY_HALFFLAT,
                                           KEY_SHARPANDAHALF,  KEY_FLATANDAHALF,

                    // octave modification
                    KEY_8VA, KEY_8VAB, KEY_OCTAVEUP, KEY_OCTAVEDOWN,
                    
                    // dot input
                    KEY_NDOT, KEY_1DOT, KEY_2DOT, KEY_3DOT, KEY_4DOT, KEY_5DOT, KEY_6DOT, KEY_7DOT, KEY_8DOT, KEY_9DOT,
                    
                    // note modification contexts
                    KEY_STEMLENGTH, KEY_STEMLENGTH_UP,
                                    KEY_STEMLENGTH_DOWN,
                                    KEY_STEMLENGTH_BEAMUP,
                                    KEY_STEMLENGTH_BEAMDOWN,
                                    KEY_STEMLENGTH_AUTO,
                    
                    KEY_STEMDIR,    KEY_STEMDIR_UP,
                                    KEY_STEMDIR_DOWN,
                                    KEY_STEMDIR_AUTO,
                    
                    KEY_CHROMATIC,  KEY_CHROMATIC_UP,
                                    KEY_CHROMATIC_DOWN,
                    
                    KEY_MOVE,       KEY_MOVE_UP,
                                    KEY_MOVE_DOWN,
                                    KEY_MOVE_LEFT, 
                                    KEY_MOVE_RIGHT,
                                    KEY_MOVE_HOME,
                    
                    KEY_ACCMOVE,    KEY_ACCMOVE_LEFT,
                                    KEY_ACCMOVE_RIGHT,
                                    KEY_ACCMOVE_HOME,
                    
                    KEY_STAFFSHIFT, KEY_STAFFSHIFT_UP,
                                    KEY_STAFFSHIFT_DOWN,
                                    KEY_STAFFSHIFT_HOME,
                    
                    KEY_BEAM,       KEY_BEAM_NONE,
                                    KEY_BEAM_AUTO,
                                    KEY_BEAM_FORCED,
                                    KEY_BEAM_CUT,
                    
                    // action count
                    KEY_COUNT};
    
    // context enumeration
    enum Context {CTX_DEFAULT, CTX_HEAD, CTX_NDOT, CTX_STEMLENGTH, CTX_STEMDIR, CTX_CHROMATIC, CTX_MOVE, CTX_ACCMOVE, CTX_STAFFSHIFT, CTX_BEAM};
    
    // context dependent actions
    enum NDot       {NDOT_0, NDOT_1, NDOT_2, NDOT_3, NDOT_4, NDOT_5, NDOT_6, NDOT_7, NDOT_8, NDOT_9, NDOT_COUNT};
    enum StemLength {STEMLENGTH_UP, STEMLENGTH_DOWN, STEMLENGTH_BEAMUP, STEMLENGTH_BEAMDOWN, STEMLENGTH_AUTO, STEMLENGTH_COUNT};
    enum StemDir    {   STEMDIR_UP,    STEMDIR_DOWN,                                            STEMDIR_AUTO,    STEMDIR_COUNT};
    enum Chromatic  { CHROMATIC_UP,  CHROMATIC_DOWN,                                                           CHROMATIC_COUNT};
    enum Move       {      MOVE_UP,       MOVE_DOWN,       MOVE_LEFT,         MOVE_RIGHT,          MOVE_HOME,       MOVE_COUNT};
    enum AccMove    {                                   ACCMOVE_LEFT,      ACCMOVE_RIGHT,       ACCMOVE_HOME,    ACCMOVE_COUNT};
    enum StaffShift {STAFFSHIFT_UP, STAFFSHIFT_DOWN,                                         STAFFSHIFT_HOME, STAFFSHIFT_COUNT};
    
    enum Beam       {BEAM_NONE, BEAM_AUTO, BEAM_FORCED, BEAM_CUT, BEAM_COUNT};
    
    // input method types
    enum InsertMethod     {INSERT_ON_NAME, INSERT_ON_VALUE};    // insert method (i.e. what specification inserts the note)
    enum HeadInsertMethod {HEAD_SWITCH,    HEAD_HOLD};          // head mode method (i.e. switched on press or on press/release)
    
 public:
    // action id (action identifier for any context)
    class ActionId
    {
     private:
        Context context;            // context
        int     action;             // value from the context dependent enum (casted)
        
     public:
        // constructors for each action-enum
        ActionId(const ActionKey);
        ActionId(const NDot);
        ActionId(const StemLength);
        ActionId(const StemDir);
        ActionId(const Chromatic);
        ActionId(const Move);
        ActionId(const AccMove);
        ActionId(const StaffShift);
        ActionId(const Beam);
        
        // assign operators
        ActionId& operator = (const ActionKey);
        ActionId& operator = (const NDot);
        ActionId& operator = (const StemLength);
        ActionId& operator = (const StemDir);
        ActionId& operator = (const Chromatic);
        ActionId& operator = (const Move);
        ActionId& operator = (const AccMove);
        ActionId& operator = (const StaffShift);
        ActionId& operator = (const Beam);
        
        // equality operators
        bool operator == (const ActionKey)  const;
        bool operator == (const NDot)       const;
        bool operator == (const StemLength) const;
        bool operator == (const StemDir)    const;
        bool operator == (const Chromatic)  const;
        bool operator == (const Move)       const;
        bool operator == (const AccMove)    const;
        bool operator == (const StaffShift) const;
        bool operator == (const Beam)       const;
        
        // comparison operators
        bool operator <  (const ActionId& x) const; // order definition  (for use in "std::map")
        bool operator == (const ActionId& x) const; // equality operator (for use in "std::map")
        
        // get data
        Context get_context() const;
        int     get_action()  const;
    };
    
    // key definition
    class Key
    {
     public:
        unsigned int key;   // key code
        unsigned int mods;  // modifier keys
        
        Key(unsigned int _key = 0, unsigned int _mods = 0); // constructor
        
        bool operator <  (const Key& x) const;  // order definition (for use in "std::map")
        bool operator == (const Key& x) const;  // equality operator (for use in "std::map")
    };
    
    // signal types
    typedef sigc::signal<void, ActionId, Key> on_assign_signal_T;
    typedef sigc::signal<void, ActionId>      on_reset_signal_T;
    
 private:
    // key mapping
    template <typename ActionT, const unsigned int COUNT> struct ContextMap
    {
     private:
        // internal key maps
        std::list<Key>         keys[COUNT]; // map action to all associated keys
        std::map<Key, ActionT> code;        // map key to the unique associated action
        
        // signals
        on_assign_signal_T& on_assign_signal;
        on_reset_signal_T&  on_reset_signal;
        
     public:
        // constructor
        ContextMap(on_assign_signal_T&, on_reset_signal_T&);
        
        // mapping setup
        void assign(const ActionT action, const unsigned int key, const unsigned int mods = 0);
        void assign(const ActionT action, const Key key);
        
        // existence check
        bool exist(const ActionT action) const;
        bool exist(const Key&    key)    const;
        
        // mapping resolving
        ActionT get_action(const Key& key) const;
        
        // reset methods
        void reset(const ActionT action);
        void reset(const Key&    key);
    };
    
    // all key mappings
    ContextMap<ActionKey,  KEY_COUNT>        actionMap;     // for general context
    ContextMap<NDot,       NDOT_COUNT>       ndotMap;       // in context of KEY_NDOT
    ContextMap<StemLength, STEMLENGTH_COUNT> stemlengthMap; // in context of KEY_STEMLENGTH
    ContextMap<StemDir,    STEMDIR_COUNT>    stemdirMap;    // in context of KEY_STEMDIR
    ContextMap<Chromatic,  CHROMATIC_COUNT>  chromaticMap;  // in context of KEY_CHROMATIC
    ContextMap<Move,       MOVE_COUNT>       moveMap;       // in context of KEY_MOVE
    ContextMap<AccMove,    ACCMOVE_COUNT>    accmoveMap;    // in context of KEY_ACCMOVE
    ContextMap<StaffShift, STAFFSHIFT_COUNT> staffshiftMap; // in context of KEY_STAFFSHIFT
    ContextMap<Beam,       BEAM_COUNT>       beamMap;       // in context of KEY_BEAM
    
    // input methods
    bool insert_on_name;            // input method (insert note on name- or on value-specification?)
    bool insert_head_hold;          // head insertion method (mode-switch / hold-modifier)
    
    // signals
    on_assign_signal_T on_assign_signal;
    on_reset_signal_T  on_reset_signal;
    
 public:
    // constructor
    KeyMap();
    
    // key map setup
    void assign(const ActionId   action, const unsigned int key, const unsigned int mods = 0);
    void assign(const ActionId   action, const Key key);
    void assign(const ActionKey  action, const unsigned int key, const unsigned int mods = 0);
    void assign(const ActionKey  action, const Key key);
    void assign(const NDot       action, const unsigned int key, const unsigned int mods = 0);
    void assign(const NDot       action, const Key key);
    void assign(const StemLength action, const unsigned int key, const unsigned int mods = 0);
    void assign(const StemLength action, const Key key);
    void assign(const StemDir    action, const unsigned int key, const unsigned int mods = 0);
    void assign(const StemDir    action, const Key key);
    void assign(const Chromatic  action, const unsigned int key, const unsigned int mods = 0);
    void assign(const Chromatic  action, const Key key);
    void assign(const Move       action, const unsigned int key, const unsigned int mods = 0);
    void assign(const Move       action, const Key key);
    void assign(const AccMove    action, const unsigned int key, const unsigned int mods = 0);
    void assign(const AccMove    action, const Key key);
    void assign(const StaffShift action, const unsigned int key, const unsigned int mods = 0);
    void assign(const StaffShift action, const Key key);
    void assign(const Beam       action, const unsigned int key, const unsigned int mods = 0);
    void assign(const Beam       action, const Key key);
    
    // action reset
    void reset(const ActionId   action);
    void reset(const ActionKey  action);
    void reset(const NDot       action);
    void reset(const StemLength action);
    void reset(const StemDir    action);
    void reset(const Chromatic  action);
    void reset(const Move       action);
    void reset(const AccMove    action);
    void reset(const StaffShift action);
    void reset(const Beam       action);
    
    // key mapping access
    bool has_key(const Key& key, const Context& context) const;
    
    template <typename ActionT>
    ActionT   get_action(const Key& key) const;
    ActionKey get_action(const Key& key) const;
    
    // method flag interface
    void set_insert_method(const InsertMethod method);
    void set_head_insert_method(const HeadInsertMethod mode);
    
    InsertMethod     get_insert_method()      const;
    HeadInsertMethod get_head_insert_method() const;
    
    bool inserts_on_name()  const;
    bool inserts_on_value() const;
    bool head_on_hold()     const;
    bool head_on_switch()   const;
    
    // signals
    sigc::signal<void, ActionId, Key> signal_on_assign();
    sigc::signal<void, ActionId>      signal_on_reset();
};

// inline method implementations (Key)
inline KeyMap::Key::Key(unsigned int _key, unsigned int _mods) : key(_key), mods(_mods) {}

inline bool KeyMap::Key::operator <  (const KeyMap::Key& x) const {return key < x.key || (key <= x.key && mods < x.mods);}
inline bool KeyMap::Key::operator == (const KeyMap::Key& x) const {return key == x.key && mods == x.mods;}

// inline method implementations (ActionId)
inline KeyMap::ActionId::ActionId(const KeyMap::ActionKey  code) : context(CTX_DEFAULT),    action(static_cast<int>(code)) {}
inline KeyMap::ActionId::ActionId(const KeyMap::NDot       code) : context(CTX_NDOT),       action(static_cast<int>(code)) {}
inline KeyMap::ActionId::ActionId(const KeyMap::StemLength code) : context(CTX_STEMLENGTH), action(static_cast<int>(code)) {}
inline KeyMap::ActionId::ActionId(const KeyMap::StemDir    code) : context(CTX_STEMDIR),    action(static_cast<int>(code)) {}
inline KeyMap::ActionId::ActionId(const KeyMap::Chromatic  code) : context(CTX_CHROMATIC),  action(static_cast<int>(code)) {}
inline KeyMap::ActionId::ActionId(const KeyMap::Move       code) : context(CTX_MOVE),       action(static_cast<int>(code)) {}
inline KeyMap::ActionId::ActionId(const KeyMap::AccMove    code) : context(CTX_ACCMOVE),    action(static_cast<int>(code)) {}
inline KeyMap::ActionId::ActionId(const KeyMap::StaffShift code) : context(CTX_STAFFSHIFT), action(static_cast<int>(code)) {}
inline KeyMap::ActionId::ActionId(const KeyMap::Beam       code) : context(CTX_BEAM),       action(static_cast<int>(code)) {}

inline KeyMap::ActionId& KeyMap::ActionId::operator = (const KeyMap::ActionKey  code) {context = CTX_DEFAULT;    action = static_cast<int>(code); return *this;}
inline KeyMap::ActionId& KeyMap::ActionId::operator = (const KeyMap::NDot       code) {context = CTX_NDOT;       action = static_cast<int>(code); return *this;}
inline KeyMap::ActionId& KeyMap::ActionId::operator = (const KeyMap::StemLength code) {context = CTX_STEMLENGTH; action = static_cast<int>(code); return *this;}
inline KeyMap::ActionId& KeyMap::ActionId::operator = (const KeyMap::StemDir    code) {context = CTX_STEMDIR;    action = static_cast<int>(code); return *this;}
inline KeyMap::ActionId& KeyMap::ActionId::operator = (const KeyMap::Chromatic  code) {context = CTX_CHROMATIC;  action = static_cast<int>(code); return *this;}
inline KeyMap::ActionId& KeyMap::ActionId::operator = (const KeyMap::Move       code) {context = CTX_MOVE;       action = static_cast<int>(code); return *this;}
inline KeyMap::ActionId& KeyMap::ActionId::operator = (const KeyMap::AccMove    code) {context = CTX_ACCMOVE;    action = static_cast<int>(code); return *this;}
inline KeyMap::ActionId& KeyMap::ActionId::operator = (const KeyMap::StaffShift code) {context = CTX_STAFFSHIFT; action = static_cast<int>(code); return *this;}
inline KeyMap::ActionId& KeyMap::ActionId::operator = (const KeyMap::Beam       code) {context = CTX_BEAM;       action = static_cast<int>(code); return *this;}

inline bool KeyMap::ActionId::operator == (const KeyMap::ActionKey  code) const {return (context == CTX_DEFAULT     && action == static_cast<int>(code));}
inline bool KeyMap::ActionId::operator == (const KeyMap::NDot       code) const {return (context == CTX_NDOT        && action == static_cast<int>(code));}
inline bool KeyMap::ActionId::operator == (const KeyMap::StemLength code) const {return (context == CTX_STEMLENGTH  && action == static_cast<int>(code));}
inline bool KeyMap::ActionId::operator == (const KeyMap::StemDir    code) const {return (context == CTX_STEMDIR     && action == static_cast<int>(code));}
inline bool KeyMap::ActionId::operator == (const KeyMap::Chromatic  code) const {return (context == CTX_CHROMATIC   && action == static_cast<int>(code));}
inline bool KeyMap::ActionId::operator == (const KeyMap::Move       code) const {return (context == CTX_MOVE        && action == static_cast<int>(code));}
inline bool KeyMap::ActionId::operator == (const KeyMap::AccMove    code) const {return (context == CTX_ACCMOVE     && action == static_cast<int>(code));}
inline bool KeyMap::ActionId::operator == (const KeyMap::StaffShift code) const {return (context == CTX_STAFFSHIFT  && action == static_cast<int>(code));}
inline bool KeyMap::ActionId::operator == (const KeyMap::Beam       code) const {return (context == CTX_BEAM        && action == static_cast<int>(code));}

inline KeyMap::Context KeyMap::ActionId::get_context() const {return context;}
inline int             KeyMap::ActionId::get_action()  const {return action;}

// template implementations (ContextMap)
template <typename T, const unsigned int C>
KeyMap::ContextMap<T,C>::ContextMap(on_assign_signal_T& sig1, on_reset_signal_T& sig2)
    : on_assign_signal(sig1), on_reset_signal(sig2) {}

template <typename T, const unsigned int C>
void KeyMap::ContextMap<T,C>::assign(const T action, const unsigned int key, const unsigned int mods)
{
    assign(action, Key(key, mods));
}

template <typename T, const unsigned int C>
void KeyMap::ContextMap<T,C>::assign(const T action, const Key key)
{
    reset(key);
    keys[action].push_back(key);
    code[key] = action;
    on_assign_signal.emit(action, key);
}

template <typename T, const unsigned int C>
bool KeyMap::ContextMap<T,C>::exist(const T action) const
{
    return !keys[action].empty();
}

template <typename T, const unsigned int C>
bool KeyMap::ContextMap<T,C>::exist(const Key& key) const
{
    return code.count(key);
}

template <typename T, const unsigned int C>
T KeyMap::ContextMap<T,C>::get_action(const Key& key) const
{
    return code.at(key);
}

template <typename T, const unsigned int C>
void KeyMap::ContextMap<T,C>::reset(const T action)
{
    for (std::list<Key>::iterator i = keys[action].begin(); i != keys[action].end(); ++i)
        code.erase(*i);
    keys[action].clear();
    on_reset_signal.emit(action);
}

template <typename T, const unsigned int C>
void KeyMap::ContextMap<T,C>::reset(const Key& key)
{
    if (!exist(key)) return;
    const T action = code[key];
    keys[action].remove(key);
    code.erase(key);
    on_reset_signal.emit(action);
}

// inline method implementations (KeyMap)
inline void KeyMap::assign(const KeyMap::ActionKey  action, const unsigned int key, const unsigned int mods) {actionMap.assign(action, key, mods);}
inline void KeyMap::assign(const KeyMap::ActionKey  action, const KeyMap::Key  key)                          {actionMap.assign(action, key);}
inline void KeyMap::assign(const KeyMap::NDot       action, const unsigned int key, const unsigned int mods) {ndotMap.assign(action, key, mods);}
inline void KeyMap::assign(const KeyMap::NDot       action, const KeyMap::Key  key)                          {ndotMap.assign(action, key);}
inline void KeyMap::assign(const KeyMap::StemLength action, const unsigned int key, const unsigned int mods) {stemlengthMap.assign(action, key, mods);}
inline void KeyMap::assign(const KeyMap::StemLength action, const KeyMap::Key  key)                          {stemlengthMap.assign(action, key);}
inline void KeyMap::assign(const KeyMap::StemDir    action, const unsigned int key, const unsigned int mods) {stemdirMap.assign(action, key, mods);}
inline void KeyMap::assign(const KeyMap::StemDir    action, const KeyMap::Key  key)                          {stemdirMap.assign(action, key);}
inline void KeyMap::assign(const KeyMap::Chromatic  action, const unsigned int key, const unsigned int mods) {chromaticMap.assign(action, key, mods);}
inline void KeyMap::assign(const KeyMap::Chromatic  action, const KeyMap::Key  key)                          {chromaticMap.assign(action, key);}
inline void KeyMap::assign(const KeyMap::Move       action, const unsigned int key, const unsigned int mods) {moveMap.assign(action, key, mods);}
inline void KeyMap::assign(const KeyMap::Move       action, const KeyMap::Key  key)                          {moveMap.assign(action, key);}
inline void KeyMap::assign(const KeyMap::AccMove    action, const unsigned int key, const unsigned int mods) {accmoveMap.assign(action, key, mods);}
inline void KeyMap::assign(const KeyMap::AccMove    action, const KeyMap::Key  key)                          {accmoveMap.assign(action, key);}
inline void KeyMap::assign(const KeyMap::StaffShift action, const unsigned int key, const unsigned int mods) {staffshiftMap.assign(action, key, mods);}
inline void KeyMap::assign(const KeyMap::StaffShift action, const KeyMap::Key  key)                          {staffshiftMap.assign(action, key);}
inline void KeyMap::assign(const KeyMap::Beam       action, const unsigned int key, const unsigned int mods) {beamMap.assign(action, key, mods);}
inline void KeyMap::assign(const KeyMap::Beam       action, const KeyMap::Key  key)                          {beamMap.assign(action, key);}

inline void KeyMap::reset(const KeyMap::ActionKey  action) {actionMap.reset(action);}
inline void KeyMap::reset(const KeyMap::NDot       action) {ndotMap.reset(action);}
inline void KeyMap::reset(const KeyMap::StemLength action) {stemlengthMap.reset(action);}
inline void KeyMap::reset(const KeyMap::StemDir    action) {stemdirMap.reset(action);}
inline void KeyMap::reset(const KeyMap::Chromatic  action) {chromaticMap.reset(action);}
inline void KeyMap::reset(const KeyMap::Move       action) {moveMap.reset(action);}
inline void KeyMap::reset(const KeyMap::AccMove    action) {accmoveMap.reset(action);}
inline void KeyMap::reset(const KeyMap::StaffShift action) {staffshiftMap.reset(action);}
inline void KeyMap::reset(const KeyMap::Beam       action) {beamMap.reset(action);}

inline             KeyMap::ActionKey  KeyMap::get_action                    (const KeyMap::Key& key) const {return actionMap.get_action(key);}
template <typename T>      T          KeyMap::get_action                    (const KeyMap::Key& key) const {return actionMap.get_action(key);}
template <> inline KeyMap::ActionKey  KeyMap::get_action<KeyMap::ActionKey> (const KeyMap::Key& key) const {return actionMap.get_action(key);}
template <> inline KeyMap::NDot       KeyMap::get_action<KeyMap::NDot>      (const KeyMap::Key& key) const {return ndotMap.get_action(key);}
template <> inline KeyMap::StemLength KeyMap::get_action<KeyMap::StemLength>(const KeyMap::Key& key) const {return stemlengthMap.get_action(key);}
template <> inline KeyMap::StemDir    KeyMap::get_action<KeyMap::StemDir>   (const KeyMap::Key& key) const {return stemdirMap.get_action(key);}
template <> inline KeyMap::Chromatic  KeyMap::get_action<KeyMap::Chromatic> (const KeyMap::Key& key) const {return chromaticMap.get_action(key);}
template <> inline KeyMap::Move       KeyMap::get_action<KeyMap::Move>      (const KeyMap::Key& key) const {return moveMap.get_action(key);}
template <> inline KeyMap::AccMove    KeyMap::get_action<KeyMap::AccMove>   (const KeyMap::Key& key) const {return accmoveMap.get_action(key);}
template <> inline KeyMap::StaffShift KeyMap::get_action<KeyMap::StaffShift>(const KeyMap::Key& key) const {return staffshiftMap.get_action(key);}
template <> inline KeyMap::Beam       KeyMap::get_action<KeyMap::Beam>      (const KeyMap::Key& key) const {return beamMap.get_action(key);}

inline void KeyMap::set_insert_method(     const InsertMethod     mode)   {insert_on_name   = (mode == INSERT_ON_NAME);}
inline void KeyMap::set_head_insert_method(const HeadInsertMethod mode)   {insert_head_hold = (mode == HEAD_HOLD);}

inline KeyMap::InsertMethod     KeyMap::get_insert_method()      const    {return (insert_on_name ? INSERT_ON_NAME : INSERT_ON_VALUE);}
inline KeyMap::HeadInsertMethod KeyMap::get_head_insert_method() const    {return (insert_head_hold ? HEAD_HOLD : HEAD_SWITCH);}

inline bool KeyMap::inserts_on_name()  const {return  insert_on_name;}
inline bool KeyMap::inserts_on_value() const {return !insert_on_name;}
inline bool KeyMap::head_on_hold()     const {return  insert_head_hold;}
inline bool KeyMap::head_on_switch()   const {return !insert_head_hold;}

inline sigc::signal<void, KeyMap::ActionId, KeyMap::Key> KeyMap::signal_on_assign() {return on_assign_signal;}
inline sigc::signal<void, KeyMap::ActionId>              KeyMap::signal_on_reset()  {return on_reset_signal;}

#endif

