#pragma once

#include <cstdint>

namespace JE {

    struct KeyModifiers
    {
        bool Ctrl;
        bool Shift;
        bool Alt;
        bool Super;
    };


    namespace detail {

        static constexpr auto SCANCODE_MASK = 1U << 30U;
        constexpr auto        ScanCodeToKeyCode(uint32_t scanCode) { return scanCode | SCANCODE_MASK; }

        enum ScanCode {
            UNKNOWN = 0,

            CAPSLOCK = 57,

            F1  = 58,
            F2  = 59,
            F3  = 60,
            F4  = 61,
            F5  = 62,
            F6  = 63,
            F7  = 64,
            F8  = 65,
            F9  = 66,
            F10 = 67,
            F11 = 68,
            F12 = 69,

            PRINTSCREEN = 70,
            SCROLLLOCK  = 71,
            PAUSE       = 72,
            INSERT      = 73, /**< insert on PC, help on some Mac keyboards (but
                                                does send code 73, not 117) */
            HOME     = 74,
            PAGEUP   = 75,
            DELETE   = 76,
            END      = 77,
            PAGEDOWN = 78,
            RIGHT    = 79,
            LEFT     = 80,
            DOWN     = 81,
            UP       = 82,

            NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                */
            KP_DIVIDE   = 84,
            KP_MULTIPLY = 85,
            KP_MINUS    = 86,
            KP_PLUS     = 87,
            KP_ENTER    = 88,
            KP_1        = 89,
            KP_2        = 90,
            KP_3        = 91,
            KP_4        = 92,
            KP_5        = 93,
            KP_6        = 94,
            KP_7        = 95,
            KP_8        = 96,
            KP_9        = 97,
            KP_0        = 98,
            KP_PERIOD   = 99,
            APPLICATION = 101, /**< windows contextual menu, compose */
            POWER       = 102, /**< The USB document says this is a status flag,
                                *   not a physical key - but some Mac keyboards
                                *   do have a power key. */
            KP_EQUALS      = 103,
            F13            = 104,
            F14            = 105,
            F15            = 106,
            F16            = 107,
            F17            = 108,
            F18            = 109,
            F19            = 110,
            F20            = 111,
            F21            = 112,
            F22            = 113,
            F23            = 114,
            F24            = 115,
            EXECUTE        = 116,
            HELP           = 117,
            MENU           = 118,
            SELECT         = 119,
            STOP           = 120,
            AGAIN          = 121, /**< redo */
            UNDO           = 122,
            CUT            = 123,
            COPY           = 124,
            PASTE          = 125,
            FIND           = 126,
            MUTE           = 127,
            VOLUMEUP       = 128,
            VOLUMEDOWN     = 129,
            KP_COMMA       = 133,
            KP_EQUALSAS400 = 134,

            ALTERASE   = 153, /**< Erase-Eaze */
            SYSREQ     = 154,
            CANCEL     = 155,
            CLEAR      = 156,
            PRIOR      = 157,
            RETURN2    = 158,
            SEPARATOR  = 159,
            OUT        = 160,
            OPER       = 161,
            CLEARAGAIN = 162,
            CRSEL      = 163,
            EXSEL      = 164,

            KP_00              = 176,
            KP_000             = 177,
            THOUSANDSSEPARATOR = 178,
            DECIMALSEPARATOR   = 179,
            CURRENCYUNIT       = 180,
            CURRENCYSUBUNIT    = 181,
            KP_LEFTPAREN       = 182,
            KP_RIGHTPAREN      = 183,
            KP_LEFTBRACE       = 184,
            KP_RIGHTBRACE      = 185,
            KP_TAB             = 186,
            KP_BACKSPACE       = 187,
            KP_A               = 188,
            KP_B               = 189,
            KP_C               = 190,
            KP_D               = 191,
            KP_E               = 192,
            KP_F               = 193,
            KP_XOR             = 194,
            KP_POWER           = 195,
            KP_PERCENT         = 196,
            KP_LESS            = 197,
            KP_GREATER         = 198,
            KP_AMPERSAND       = 199,
            KP_DBLAMPERSAND    = 200,
            KP_VERTICALBAR     = 201,
            KP_DBLVERTICALBAR  = 202,
            KP_COLON           = 203,
            KP_HASH            = 204,
            KP_SPACE           = 205,
            KP_AT              = 206,
            KP_EXCLAM          = 207,
            KP_MEMSTORE        = 208,
            KP_MEMRECALL       = 209,
            KP_MEMCLEAR        = 210,
            KP_MEMADD          = 211,
            KP_MEMSUBTRACT     = 212,
            KP_MEMMULTIPLY     = 213,
            KP_MEMDIVIDE       = 214,
            KP_PLUSMINUS       = 215,
            KP_CLEAR           = 216,
            KP_CLEARENTRY      = 217,
            KP_BINARY          = 218,
            KP_OCTAL           = 219,
            KP_DECIMAL         = 220,
            KP_HEXADECIMAL     = 221,

            LCTRL  = 224,
            LSHIFT = 225,
            LALT   = 226, /**< alt, option */
            LSuper = 227, /**< windows, command (apple), meta */
            RCTRL  = 228,
            RSHIFT = 229,
            RALT   = 230, /**< alt gr, option */
            RSuper = 231, /**< windows, command (apple), meta */

            MODE = 257, /**< I'm not sure if this is really not covered
                         *   by any of the above, but since there's a
                         *   special KMOD_MODE for it I'm adding it here
                         */

            AUDIONEXT      = 258,
            AUDIOPREV      = 259,
            AUDIOSTOP      = 260,
            AUDIOPLAY      = 261,
            AUDIOMUTE      = 262,
            MEDIASELECT    = 263,
            WWW            = 264,
            MAIL           = 265,
            CALCULATOR     = 266,
            COMPUTER       = 267,
            AC_SEARCH      = 268,
            AC_HOME        = 269,
            AC_BACK        = 270,
            AC_FORWARD     = 271,
            AC_STOP        = 272,
            AC_REFRESH     = 273,
            AC_BOOKMARKS   = 274,
            BRIGHTNESSDOWN = 275,
            BRIGHTNESSUP   = 276,
            DISPLAYSWITCH  = 277, /**< display mirroring/dual display
                                                    switch, video mode switch */
            KBDILLUMTOGGLE = 278,
            KBDILLUMDOWN   = 279,
            KBDILLUMUP     = 280,
            EJECT          = 281,
            SLEEP          = 282,

            APP1 = 283,
            APP2 = 284,

            AUDIOREWIND      = 285,
            AUDIOFASTFORWARD = 286,

            SDL_NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
                                         for array bounds */
        };
    }// namespace detail


    enum class KeyCode {
        Unknown = 0,

        Return     = '\r',
        Escape     = '\x1B',
        Backspace  = '\b',
        Tab        = '\t',
        Space      = ' ',
        Exclaim    = '!',
        QuotedBL   = '"',
        Hash       = '#',
        Percent    = '%',
        Dollar     = '$',
        Ampersand  = '&',
        Quote      = '\'',
        Leftparen  = '(',
        Rightparen = ')',
        Asterisk   = '*',
        Plus       = '+',
        Comma      = ',',
        Minus      = '-',
        Period     = '.',
        Slash      = '/',
        Zero       = '0',
        One        = '1',
        Two        = '2',
        Three      = '3',
        Four       = '4',
        Five       = '5',
        Six        = '6',
        Seven      = '7',
        Eight      = '8',
        Nine       = '9',
        Colon      = ':',
        Semicolon  = ';',
        Less       = '<',
        Equals     = '=',
        Greater    = '>',
        Question   = '?',
        At         = '@',

        /*
           Skip uppercase letters
         */

        Leftbracket  = '[',
        Backslash    = '\\',
        Rightbracket = ']',
        Caret        = '^',
        Underscore   = '_',
        Backquote    = '`',
        a            = 'a',
        b            = 'b',
        c            = 'c',
        d            = 'd',
        e            = 'e',
        f            = 'f',
        g            = 'g',
        h            = 'h',
        i            = 'i',
        j            = 'j',
        k            = 'k',
        l            = 'l',
        m            = 'm',
        n            = 'n',
        o            = 'o',
        p            = 'p',
        q            = 'q',
        r            = 'r',
        s            = 's',
        t            = 't',
        u            = 'u',
        v            = 'v',
        w            = 'w',
        x            = 'x',
        y            = 'y',
        z            = 'z',

        Capslock = detail::ScanCodeToKeyCode(detail::ScanCode::CAPSLOCK),

        F1  = detail::ScanCodeToKeyCode(detail::ScanCode::F1),
        F2  = detail::ScanCodeToKeyCode(detail::ScanCode::F2),
        F3  = detail::ScanCodeToKeyCode(detail::ScanCode::F3),
        F4  = detail::ScanCodeToKeyCode(detail::ScanCode::F4),
        F5  = detail::ScanCodeToKeyCode(detail::ScanCode::F5),
        F6  = detail::ScanCodeToKeyCode(detail::ScanCode::F6),
        F7  = detail::ScanCodeToKeyCode(detail::ScanCode::F7),
        F8  = detail::ScanCodeToKeyCode(detail::ScanCode::F8),
        F9  = detail::ScanCodeToKeyCode(detail::ScanCode::F9),
        F10 = detail::ScanCodeToKeyCode(detail::ScanCode::F10),
        F11 = detail::ScanCodeToKeyCode(detail::ScanCode::F11),
        F12 = detail::ScanCodeToKeyCode(detail::ScanCode::F12),

        PrintScreen = detail::ScanCodeToKeyCode(detail::ScanCode::PRINTSCREEN),
        ScrollLock  = detail::ScanCodeToKeyCode(detail::ScanCode::SCROLLLOCK),
        Pause       = detail::ScanCodeToKeyCode(detail::ScanCode::PAUSE),
        Insert      = detail::ScanCodeToKeyCode(detail::ScanCode::INSERT),
        Home        = detail::ScanCodeToKeyCode(detail::ScanCode::HOME),
        PageUp      = detail::ScanCodeToKeyCode(detail::ScanCode::PAGEUP),
        Delete      = '\x7F',
        End         = detail::ScanCodeToKeyCode(detail::ScanCode::END),
        PageDown    = detail::ScanCodeToKeyCode(detail::ScanCode::PAGEDOWN),
        Right       = detail::ScanCodeToKeyCode(detail::ScanCode::RIGHT),
        Left        = detail::ScanCodeToKeyCode(detail::ScanCode::LEFT),
        Down        = detail::ScanCodeToKeyCode(detail::ScanCode::DOWN),
        Up          = detail::ScanCodeToKeyCode(detail::ScanCode::UP),

        NumLockClear   = detail::ScanCodeToKeyCode(detail::ScanCode::NUMLOCKCLEAR),
        KeyPadDivide   = detail::ScanCodeToKeyCode(detail::ScanCode::KP_DIVIDE),
        KeyPadMultiply = detail::ScanCodeToKeyCode(detail::ScanCode::KP_MULTIPLY),
        KeyPadMinus    = detail::ScanCodeToKeyCode(detail::ScanCode::KP_MINUS),
        KeyPadPlus     = detail::ScanCodeToKeyCode(detail::ScanCode::KP_PLUS),
        KeyPadEnter    = detail::ScanCodeToKeyCode(detail::ScanCode::KP_ENTER),
        KeyPad1        = detail::ScanCodeToKeyCode(detail::ScanCode::KP_1),
        KeyPad2        = detail::ScanCodeToKeyCode(detail::ScanCode::KP_2),
        KeyPad3        = detail::ScanCodeToKeyCode(detail::ScanCode::KP_3),
        KeyPad4        = detail::ScanCodeToKeyCode(detail::ScanCode::KP_4),
        KeyPad5        = detail::ScanCodeToKeyCode(detail::ScanCode::KP_5),
        KeyPad6        = detail::ScanCodeToKeyCode(detail::ScanCode::KP_6),
        KeyPad7        = detail::ScanCodeToKeyCode(detail::ScanCode::KP_7),
        KeyPad8        = detail::ScanCodeToKeyCode(detail::ScanCode::KP_8),
        KeyPad9        = detail::ScanCodeToKeyCode(detail::ScanCode::KP_9),
        KeyPad0        = detail::ScanCodeToKeyCode(detail::ScanCode::KP_0),
        KeyPadPeriod   = detail::ScanCodeToKeyCode(detail::ScanCode::KP_PERIOD),

        Application       = detail::ScanCodeToKeyCode(detail::ScanCode::APPLICATION),
        Power             = detail::ScanCodeToKeyCode(detail::ScanCode::POWER),
        KeyPadEquals      = detail::ScanCodeToKeyCode(detail::ScanCode::KP_EQUALS),
        F13               = detail::ScanCodeToKeyCode(detail::ScanCode::F13),
        F14               = detail::ScanCodeToKeyCode(detail::ScanCode::F14),
        F15               = detail::ScanCodeToKeyCode(detail::ScanCode::F15),
        F16               = detail::ScanCodeToKeyCode(detail::ScanCode::F16),
        F17               = detail::ScanCodeToKeyCode(detail::ScanCode::F17),
        F18               = detail::ScanCodeToKeyCode(detail::ScanCode::F18),
        F19               = detail::ScanCodeToKeyCode(detail::ScanCode::F19),
        F20               = detail::ScanCodeToKeyCode(detail::ScanCode::F20),
        F21               = detail::ScanCodeToKeyCode(detail::ScanCode::F21),
        F22               = detail::ScanCodeToKeyCode(detail::ScanCode::F22),
        F23               = detail::ScanCodeToKeyCode(detail::ScanCode::F23),
        F24               = detail::ScanCodeToKeyCode(detail::ScanCode::F24),
        Execute           = detail::ScanCodeToKeyCode(detail::ScanCode::EXECUTE),
        Help              = detail::ScanCodeToKeyCode(detail::ScanCode::HELP),
        Menu              = detail::ScanCodeToKeyCode(detail::ScanCode::MENU),
        Select            = detail::ScanCodeToKeyCode(detail::ScanCode::SELECT),
        Stop              = detail::ScanCodeToKeyCode(detail::ScanCode::STOP),
        Again             = detail::ScanCodeToKeyCode(detail::ScanCode::AGAIN),
        Undo              = detail::ScanCodeToKeyCode(detail::ScanCode::UNDO),
        Cut               = detail::ScanCodeToKeyCode(detail::ScanCode::CUT),
        Copy              = detail::ScanCodeToKeyCode(detail::ScanCode::COPY),
        Paste             = detail::ScanCodeToKeyCode(detail::ScanCode::PASTE),
        Find              = detail::ScanCodeToKeyCode(detail::ScanCode::FIND),
        Mute              = detail::ScanCodeToKeyCode(detail::ScanCode::MUTE),
        VolumeUp          = detail::ScanCodeToKeyCode(detail::ScanCode::VOLUMEUP),
        VolumeDown        = detail::ScanCodeToKeyCode(detail::ScanCode::VOLUMEDOWN),
        KeyPadComma       = detail::ScanCodeToKeyCode(detail::ScanCode::KP_COMMA),
        KeyPadEqualsAs400 = detail::ScanCodeToKeyCode(detail::ScanCode::KP_EQUALSAS400),

        AltErase   = detail::ScanCodeToKeyCode(detail::ScanCode::ALTERASE),
        SysReq     = detail::ScanCodeToKeyCode(detail::ScanCode::SYSREQ),
        Cancel     = detail::ScanCodeToKeyCode(detail::ScanCode::CANCEL),
        Clear      = detail::ScanCodeToKeyCode(detail::ScanCode::CLEAR),
        Prior      = detail::ScanCodeToKeyCode(detail::ScanCode::PRIOR),
        Return2    = detail::ScanCodeToKeyCode(detail::ScanCode::RETURN2),
        Separator  = detail::ScanCodeToKeyCode(detail::ScanCode::SEPARATOR),
        Out        = detail::ScanCodeToKeyCode(detail::ScanCode::OUT),
        Oper       = detail::ScanCodeToKeyCode(detail::ScanCode::OPER),
        ClearAgain = detail::ScanCodeToKeyCode(detail::ScanCode::CLEARAGAIN),
        CrSel      = detail::ScanCodeToKeyCode(detail::ScanCode::CRSEL),
        ExSel      = detail::ScanCodeToKeyCode(detail::ScanCode::EXSEL),

        KeyPad00             = detail::ScanCodeToKeyCode(detail::ScanCode::KP_00),
        KeyPad000            = detail::ScanCodeToKeyCode(detail::ScanCode::KP_000),
        ThousandSeparator    = detail::ScanCodeToKeyCode(detail::ScanCode::THOUSANDSSEPARATOR),
        DecimalSeparator     = detail::ScanCodeToKeyCode(detail::ScanCode::DECIMALSEPARATOR),
        CurrencyUnit         = detail::ScanCodeToKeyCode(detail::ScanCode::CURRENCYUNIT),
        CurrencySubUnit      = detail::ScanCodeToKeyCode(detail::ScanCode::CURRENCYSUBUNIT),
        KeyPadLeftParen      = detail::ScanCodeToKeyCode(detail::ScanCode::KP_LEFTPAREN),
        KeyPadRightParen     = detail::ScanCodeToKeyCode(detail::ScanCode::KP_RIGHTPAREN),
        KeyPadLeftBrace      = detail::ScanCodeToKeyCode(detail::ScanCode::KP_LEFTBRACE),
        KeyPadRightBrace     = detail::ScanCodeToKeyCode(detail::ScanCode::KP_RIGHTBRACE),
        KeyPadTab            = detail::ScanCodeToKeyCode(detail::ScanCode::KP_TAB),
        KeyPadBackSpace      = detail::ScanCodeToKeyCode(detail::ScanCode::KP_BACKSPACE),
        KeyPadA              = detail::ScanCodeToKeyCode(detail::ScanCode::KP_A),
        KeyPadB              = detail::ScanCodeToKeyCode(detail::ScanCode::KP_B),
        KeyPadC              = detail::ScanCodeToKeyCode(detail::ScanCode::KP_C),
        KeyPadD              = detail::ScanCodeToKeyCode(detail::ScanCode::KP_D),
        KeyPadE              = detail::ScanCodeToKeyCode(detail::ScanCode::KP_E),
        KeyPadF              = detail::ScanCodeToKeyCode(detail::ScanCode::KP_F),
        KeyPadXOR            = detail::ScanCodeToKeyCode(detail::ScanCode::KP_XOR),
        KeyPadPower          = detail::ScanCodeToKeyCode(detail::ScanCode::KP_POWER),
        KeyPadPercent        = detail::ScanCodeToKeyCode(detail::ScanCode::KP_PERCENT),
        KeyPadLess           = detail::ScanCodeToKeyCode(detail::ScanCode::KP_LESS),
        KeyPadGreater        = detail::ScanCodeToKeyCode(detail::ScanCode::KP_GREATER),
        KeyPadAmpersand      = detail::ScanCodeToKeyCode(detail::ScanCode::KP_AMPERSAND),
        KeyPadDblAmpersand   = detail::ScanCodeToKeyCode(detail::ScanCode::KP_DBLAMPERSAND),
        KeyPadVerticalBar    = detail::ScanCodeToKeyCode(detail::ScanCode::KP_VERTICALBAR),
        KeyPadDblVerticalBar = detail::ScanCodeToKeyCode(detail::ScanCode::KP_DBLVERTICALBAR),
        KeyPadColon          = detail::ScanCodeToKeyCode(detail::ScanCode::KP_COLON),
        KeyPadHash           = detail::ScanCodeToKeyCode(detail::ScanCode::KP_HASH),
        KeyPadSpace          = detail::ScanCodeToKeyCode(detail::ScanCode::KP_SPACE),
        KeyPadAt             = detail::ScanCodeToKeyCode(detail::ScanCode::KP_AT),
        KeyPadExclam         = detail::ScanCodeToKeyCode(detail::ScanCode::KP_EXCLAM),
        KeyPadMemStore       = detail::ScanCodeToKeyCode(detail::ScanCode::KP_MEMSTORE),
        KeyPadMemRecal       = detail::ScanCodeToKeyCode(detail::ScanCode::KP_MEMRECALL),
        KeyPadMemClear       = detail::ScanCodeToKeyCode(detail::ScanCode::KP_MEMCLEAR),
        KeyPadMemAdd         = detail::ScanCodeToKeyCode(detail::ScanCode::KP_MEMADD),
        KeyPadMemSubtract    = detail::ScanCodeToKeyCode(detail::ScanCode::KP_MEMSUBTRACT),
        KeyPadMemMultiply    = detail::ScanCodeToKeyCode(detail::ScanCode::KP_MEMMULTIPLY),
        KeyPadMemDivide      = detail::ScanCodeToKeyCode(detail::ScanCode::KP_MEMDIVIDE),
        KeyPadPlusMinus      = detail::ScanCodeToKeyCode(detail::ScanCode::KP_PLUSMINUS),
        KeyPadClear          = detail::ScanCodeToKeyCode(detail::ScanCode::KP_CLEAR),
        KeyPadClearEntry     = detail::ScanCodeToKeyCode(detail::ScanCode::KP_CLEARENTRY),
        KeyPadBinary         = detail::ScanCodeToKeyCode(detail::ScanCode::KP_BINARY),
        KeyPadOctal          = detail::ScanCodeToKeyCode(detail::ScanCode::KP_OCTAL),
        KeyPadDecimal        = detail::ScanCodeToKeyCode(detail::ScanCode::KP_DECIMAL),
        KeyPadHexDecimal     = detail::ScanCodeToKeyCode(detail::ScanCode::KP_HEXADECIMAL),

        LCtrl  = detail::ScanCodeToKeyCode(detail::ScanCode::LCTRL),
        LShift = detail::ScanCodeToKeyCode(detail::ScanCode::LSHIFT),
        LAlt   = detail::ScanCodeToKeyCode(detail::ScanCode::LALT),
        LSuper = detail::ScanCodeToKeyCode(detail::ScanCode::LSuper),
        RCtrl  = detail::ScanCodeToKeyCode(detail::ScanCode::RCTRL),
        RShift = detail::ScanCodeToKeyCode(detail::ScanCode::RSHIFT),
        RAlt   = detail::ScanCodeToKeyCode(detail::ScanCode::RALT),
        RSuper = detail::ScanCodeToKeyCode(detail::ScanCode::RSuper),

        Mode = detail::ScanCodeToKeyCode(detail::ScanCode::MODE),

        AudioNext    = detail::ScanCodeToKeyCode(detail::ScanCode::AUDIONEXT),
        AudioPrev    = detail::ScanCodeToKeyCode(detail::ScanCode::AUDIOPREV),
        AudioStop    = detail::ScanCodeToKeyCode(detail::ScanCode::AUDIOSTOP),
        AudioPlay    = detail::ScanCodeToKeyCode(detail::ScanCode::AUDIOPLAY),
        AudioMute    = detail::ScanCodeToKeyCode(detail::ScanCode::AUDIOMUTE),
        MediaSelect  = detail::ScanCodeToKeyCode(detail::ScanCode::MEDIASELECT),
        WWW          = detail::ScanCodeToKeyCode(detail::ScanCode::WWW),
        Mail         = detail::ScanCodeToKeyCode(detail::ScanCode::MAIL),
        Calculator   = detail::ScanCodeToKeyCode(detail::ScanCode::CALCULATOR),
        Computer     = detail::ScanCodeToKeyCode(detail::ScanCode::COMPUTER),
        AC_Search    = detail::ScanCodeToKeyCode(detail::ScanCode::AC_SEARCH),
        AC_Home      = detail::ScanCodeToKeyCode(detail::ScanCode::AC_HOME),
        AC_Back      = detail::ScanCodeToKeyCode(detail::ScanCode::AC_BACK),
        AC_Forward   = detail::ScanCodeToKeyCode(detail::ScanCode::AC_FORWARD),
        AC_Stop      = detail::ScanCodeToKeyCode(detail::ScanCode::AC_STOP),
        AC_Refresh   = detail::ScanCodeToKeyCode(detail::ScanCode::AC_REFRESH),
        AC_Bookmarks = detail::ScanCodeToKeyCode(detail::ScanCode::AC_BOOKMARKS),

        BrightnessDown = detail::ScanCodeToKeyCode(detail::ScanCode::BRIGHTNESSDOWN),
        BrightnessUp   = detail::ScanCodeToKeyCode(detail::ScanCode::BRIGHTNESSUP),
        DisplaySwitch  = detail::ScanCodeToKeyCode(detail::ScanCode::DISPLAYSWITCH),
        KBDILLUMTOGGLE = detail::ScanCodeToKeyCode(detail::ScanCode::KBDILLUMTOGGLE),
        KBDILLUMDOWN   = detail::ScanCodeToKeyCode(detail::ScanCode::KBDILLUMDOWN),
        KBDILLUMUP     = detail::ScanCodeToKeyCode(detail::ScanCode::KBDILLUMUP),
        Eject          = detail::ScanCodeToKeyCode(detail::ScanCode::EJECT),
        Sleep          = detail::ScanCodeToKeyCode(detail::ScanCode::SLEEP),
        APP1           = detail::ScanCodeToKeyCode(detail::ScanCode::APP1),
        APP2           = detail::ScanCodeToKeyCode(detail::ScanCode::APP2),

        AudioRewind      = detail::ScanCodeToKeyCode(detail::ScanCode::AUDIOREWIND),
        AudioFastForward = detail::ScanCodeToKeyCode(detail::ScanCode::AUDIOFASTFORWARD)
    };

    inline auto JEngine3DKeyCodeToString(KeyCode keyCode) -> const char*
    {
        switch (keyCode) {
            case KeyCode::Return: return "Return";
            case KeyCode::Escape: return "Escape";
            case KeyCode::Backspace: return "Backspace";
            case KeyCode::Tab: return "Tab";
            case KeyCode::Space: return "Space";
            case KeyCode::Exclaim: return "Exclaim";
            case KeyCode::QuotedBL: return "QuotedBL";
            case KeyCode::Hash: return "Hash";
            case KeyCode::Percent: return "Percent";
            case KeyCode::Dollar: return "Dollar";
            case KeyCode::Ampersand: return "Ampersand";
            case KeyCode::Quote: return "Quote";
            case KeyCode::Leftparen: return "Leftparen";
            case KeyCode::Rightparen: return "Rightparen";
            case KeyCode::Asterisk: return "Asterisk";
            case KeyCode::Plus: return "Plus";
            case KeyCode::Comma: return "Comma";
            case KeyCode::Minus: return "Minus";
            case KeyCode::Period: return "Period";
            case KeyCode::Slash: return "Slash";
            case KeyCode::Zero: return "Zero";
            case KeyCode::One: return "One";
            case KeyCode::Two: return "Two";
            case KeyCode::Three: return "Three";
            case KeyCode::Four: return "Four";
            case KeyCode::Five: return "Five";
            case KeyCode::Six: return "Six";
            case KeyCode::Seven: return "Seven";
            case KeyCode::Eight: return "Eight";
            case KeyCode::Nine: return "Nine";
            case KeyCode::Colon: return "Colon";
            case KeyCode::Semicolon: return "Semicolon";
            case KeyCode::Less: return "Less";
            case KeyCode::Equals: return "Equals";
            case KeyCode::Greater: return "Greater";
            case KeyCode::Question: return "Question";
            case KeyCode::At: return "At";
            case KeyCode::Leftbracket: return "Leftbracket";
            case KeyCode::Backslash: return "Backslash";
            case KeyCode::Rightbracket: return "Rightbracket";
            case KeyCode::Caret: return "Caret";
            case KeyCode::Underscore: return "Underscore";
            case KeyCode::Backquote: return "Backquote";
            case KeyCode::a: return "a";
            case KeyCode::b: return "b";
            case KeyCode::c: return "c";
            case KeyCode::d: return "d";
            case KeyCode::e: return "e";
            case KeyCode::f: return "f";
            case KeyCode::g: return "g";
            case KeyCode::h: return "h";
            case KeyCode::i: return "i";
            case KeyCode::j: return "j";
            case KeyCode::k: return "k";
            case KeyCode::l: return "l";
            case KeyCode::m: return "m";
            case KeyCode::n: return "n";
            case KeyCode::o: return "o";
            case KeyCode::p: return "p";
            case KeyCode::q: return "q";
            case KeyCode::r: return "r";
            case KeyCode::s: return "s";
            case KeyCode::t: return "t";
            case KeyCode::u: return "u";
            case KeyCode::v: return "v";
            case KeyCode::w: return "w";
            case KeyCode::x: return "x";
            case KeyCode::y: return "y";
            case KeyCode::z: return "z";
            case KeyCode::Capslock: return "Capslock";
            case KeyCode::F1: return "F1";
            case KeyCode::F2: return "F2";
            case KeyCode::F3: return "F3";
            case KeyCode::F4: return "F4";
            case KeyCode::F5: return "F5";
            case KeyCode::F6: return "F6";
            case KeyCode::F7: return "F7";
            case KeyCode::F8: return "F8";
            case KeyCode::F9: return "F9";
            case KeyCode::F10: return "F10";
            case KeyCode::F11: return "F11";
            case KeyCode::F12: return "F12";
            case KeyCode::PrintScreen: return "PrintScreen";
            case KeyCode::ScrollLock: return "ScrollLock";
            case KeyCode::Pause: return "Pause";
            case KeyCode::Insert: return "Insert";
            case KeyCode::Home: return "Home";
            case KeyCode::PageUp: return "PageUp";
            case KeyCode::Delete: return "Delete";
            case KeyCode::End: return "End";
            case KeyCode::PageDown: return "PageDown";
            case KeyCode::Right: return "Right";
            case KeyCode::Left: return "Left";
            case KeyCode::Down: return "Down";
            case KeyCode::Up: return "Up";
            case KeyCode::NumLockClear: return "NumLockClear";
            case KeyCode::KeyPadDivide: return "KeyPadDivide";
            case KeyCode::KeyPadMultiply: return "KeyPadMultiply";
            case KeyCode::KeyPadMinus: return "KeyPadMinus";
            case KeyCode::KeyPadPlus: return "KeyPadPlus";
            case KeyCode::KeyPadEnter: return "KeyPadEnter";
            case KeyCode::KeyPad1: return "KeyPad1";
            case KeyCode::KeyPad2: return "KeyPad2";
            case KeyCode::KeyPad3: return "KeyPad3";
            case KeyCode::KeyPad4: return "KeyPad4";
            case KeyCode::KeyPad5: return "KeyPad5";
            case KeyCode::KeyPad6: return "KeyPad6";
            case KeyCode::KeyPad7: return "KeyPad7";
            case KeyCode::KeyPad8: return "KeyPad8";
            case KeyCode::KeyPad9: return "KeyPad9";
            case KeyCode::KeyPad0: return "KeyPad0";
            case KeyCode::KeyPadPeriod: return "KeyPadPeriod";
            case KeyCode::Application: return "Application";
            case KeyCode::Power: return "Power";
            case KeyCode::KeyPadEquals: return "KeyPadEquals";
            case KeyCode::F13: return "F13";
            case KeyCode::F14: return "F14";
            case KeyCode::F15: return "F15";
            case KeyCode::F16: return "F16";
            case KeyCode::F17: return "F17";
            case KeyCode::F18: return "F18";
            case KeyCode::F19: return "F19";
            case KeyCode::F20: return "F20";
            case KeyCode::F21: return "F21";
            case KeyCode::F22: return "F22";
            case KeyCode::F23: return "F23";
            case KeyCode::F24: return "F24";
            case KeyCode::Execute: return "Execute";
            case KeyCode::Help: return "Help";
            case KeyCode::Menu: return "Menu";
            case KeyCode::Select: return "Select";
            case KeyCode::Stop: return "Stop";
            case KeyCode::Again: return "Again";
            case KeyCode::Undo: return "Undo";
            case KeyCode::Cut: return "Cut";
            case KeyCode::Copy: return "Copy";
            case KeyCode::Paste: return "Paste";
            case KeyCode::Find: return "Find";
            case KeyCode::Mute: return "Mute";
            case KeyCode::VolumeUp: return "VolumeUp";
            case KeyCode::VolumeDown: return "VolumeDown";
            case KeyCode::KeyPadComma: return "KeyPadComma";
            case KeyCode::KeyPadEqualsAs400: return "KeyPadEqualsAs400";
            case KeyCode::AltErase: return "AltErase";
            case KeyCode::SysReq: return "SysReq";
            case KeyCode::Cancel: return "Cancel";
            case KeyCode::Clear: return "Clear";
            case KeyCode::Prior: return "Prior";
            case KeyCode::Return2: return "Return2";
            case KeyCode::Separator: return "Separator";
            case KeyCode::Out: return "Out";
            case KeyCode::Oper: return "Oper";
            case KeyCode::ClearAgain: return "ClearAgain";
            case KeyCode::CrSel: return "CrSel";
            case KeyCode::ExSel: return "ExSel";
            case KeyCode::KeyPad00: return "KeyPad00";
            case KeyCode::KeyPad000: return "KeyPad000";
            case KeyCode::ThousandSeparator: return "ThousandSeparator";
            case KeyCode::DecimalSeparator: return "DecimalSeparator";
            case KeyCode::CurrencyUnit: return "CurrencyUnit";
            case KeyCode::CurrencySubUnit: return "CurrencySubUnit";
            case KeyCode::KeyPadLeftParen: return "KeyPadLeftParen";
            case KeyCode::KeyPadRightParen: return "KeyPadRightParen";
            case KeyCode::KeyPadLeftBrace: return "KeyPadLeftBrace";
            case KeyCode::KeyPadRightBrace: return "KeyPadRightBrace";
            case KeyCode::KeyPadTab: return "KeyPadTab";
            case KeyCode::KeyPadBackSpace: return "KeyPadBackSpace";
            case KeyCode::KeyPadA: return "KeyPadA";
            case KeyCode::KeyPadB: return "KeyPadB";
            case KeyCode::KeyPadC: return "KeyPadC";
            case KeyCode::KeyPadD: return "KeyPadD";
            case KeyCode::KeyPadE: return "KeyPadE";
            case KeyCode::KeyPadF: return "KeyPadF";
            case KeyCode::KeyPadXOR: return "KeyPadXOR";
            case KeyCode::KeyPadPower: return "KeyPadPower";
            case KeyCode::KeyPadPercent: return "KeyPadPercent";
            case KeyCode::KeyPadLess: return "KeyPadLess";
            case KeyCode::KeyPadGreater: return "KeyPadGreater";
            case KeyCode::KeyPadAmpersand: return "KeyPadAmpersand";
            case KeyCode::KeyPadDblAmpersand: return "KeyPadDblAmpersand";
            case KeyCode::KeyPadVerticalBar: return "KeyPadVerticalBar";
            case KeyCode::KeyPadDblVerticalBar: return "KeyPadDblVerticalBar";
            case KeyCode::KeyPadColon: return "KeyPadColon";
            case KeyCode::KeyPadHash: return "KeyPadHash";
            case KeyCode::KeyPadSpace: return "KeyPadSpace";
            case KeyCode::KeyPadAt: return "KeyPadAt";
            case KeyCode::KeyPadExclam: return "KeyPadExclam";
            case KeyCode::KeyPadMemStore: return "KeyPadMemStore";
            case KeyCode::KeyPadMemRecal: return "KeyPadMemRecal";
            case KeyCode::KeyPadMemClear: return "KeyPadMemClear";
            case KeyCode::KeyPadMemAdd: return "KeyPadMemAdd";
            case KeyCode::KeyPadMemSubtract: return "KeyPadMemSubtract";
            case KeyCode::KeyPadMemMultiply: return "KeyPadMemMultiply";
            case KeyCode::KeyPadMemDivide: return "KeyPadMemDivide";
            case KeyCode::KeyPadPlusMinus: return "KeyPadPlusMinus";
            case KeyCode::KeyPadClear: return "KeyPadClear";
            case KeyCode::KeyPadClearEntry: return "KeyPadClearEntry";
            case KeyCode::KeyPadBinary: return "KeyPadBinary";
            case KeyCode::KeyPadOctal: return "KeyPadOctal";
            case KeyCode::KeyPadDecimal: return "KeyPadDecimal";
            case KeyCode::KeyPadHexDecimal: return "KeyPadHexDecimal";
            case KeyCode::LCtrl: return "LCtrl";
            case KeyCode::LShift: return "LShift";
            case KeyCode::LAlt: return "LAlt";
            case KeyCode::LSuper: return "LSuper";
            case KeyCode::RCtrl: return "RCtrl";
            case KeyCode::RShift: return "RShift";
            case KeyCode::RAlt: return "RAlt";
            case KeyCode::RSuper: return "RSuper";
            case KeyCode::Mode: return "Mode";
            case KeyCode::AudioNext: return "AudioNext";
            case KeyCode::AudioPrev: return "AudioPrev";
            case KeyCode::AudioStop: return "AudioStop";
            case KeyCode::AudioPlay: return "AudioPlay";
            case KeyCode::AudioMute: return "AudioMute";
            case KeyCode::MediaSelect: return "MediaSelect";
            case KeyCode::WWW: return "WWW";
            case KeyCode::Mail: return "Mail";
            case KeyCode::Calculator: return "Calculator";
            case KeyCode::Computer: return "Computer";
            case KeyCode::AC_Search: return "AC_Search";
            case KeyCode::AC_Home: return "AC_Home";
            case KeyCode::AC_Back: return "AC_Back";
            case KeyCode::AC_Forward: return "AC_Forward";
            case KeyCode::AC_Stop: return "AC_Stop";
            case KeyCode::AC_Refresh: return "AC_Refresh";
            case KeyCode::AC_Bookmarks: return "AC_Bookmarks";
            case KeyCode::BrightnessDown: return "BrightnessDown";
            case KeyCode::BrightnessUp: return "BrightnessUp";
            case KeyCode::DisplaySwitch: return "DisplaySwitch";
            case KeyCode::KBDILLUMTOGGLE: return "KBDILLUMTOGGLE";
            case KeyCode::KBDILLUMDOWN: return "KBDILLUMDOWN";
            case KeyCode::KBDILLUMUP: return "KBDILLUMUP";
            case KeyCode::Eject: return "Eject";
            case KeyCode::Sleep: return "Sleep";
            case KeyCode::APP1: return "APP1";
            case KeyCode::APP2: return "APP2";
            case KeyCode::AudioRewind: return "AudioRewind";
            case KeyCode::AudioFastForward: return "AudioFastForward";

            default: return "Unknown";
        }
    }

}// namespace JE