#pragma once

#include "JEngine3D/Core/KeyCodes.hpp"
#include "JEngine3D/Core/MouseButtons.hpp"
#include <imgui.h>

namespace JE {

    inline auto MouseButtonToImGuiButton(MouseButton button) -> int
    {
        if (button == MouseButton::Left) { return 0; }
        if (button == MouseButton::Right) { return 1; }
        if (button == MouseButton::Middle) { return 2; }
        if (button == MouseButton::X1) { return 3; }
        if (button == MouseButton::X2) { return 4; }

        return -1;
    };


    inline auto JEngine3DKeyCodeToImGuiCode(KeyCode code) -> ImGuiKey
    {
        switch (code) {
            case KeyCode::Tab: return ImGuiKey_Tab;
            case KeyCode::Left: return ImGuiKey_LeftArrow;
            case KeyCode::Right: return ImGuiKey_RightArrow;
            case KeyCode::Up: return ImGuiKey_UpArrow;
            case KeyCode::Down: return ImGuiKey_DownArrow;
            case KeyCode::PageUp: return ImGuiKey_PageUp;
            case KeyCode::PageDown: return ImGuiKey_PageDown;
            case KeyCode::Home: return ImGuiKey_Home;
            case KeyCode::End: return ImGuiKey_End;
            case KeyCode::Insert: return ImGuiKey_Insert;
            case KeyCode::Delete: return ImGuiKey_Delete;
            case KeyCode::Backspace: return ImGuiKey_Backspace;
            case KeyCode::Space: return ImGuiKey_Space;
            case KeyCode::Return: return ImGuiKey_Enter;
            case KeyCode::Escape: return ImGuiKey_Escape;
            case KeyCode::Quote: return ImGuiKey_Apostrophe;
            case KeyCode::Comma: return ImGuiKey_Comma;
            case KeyCode::Minus: return ImGuiKey_Minus;
            case KeyCode::Period: return ImGuiKey_Period;
            case KeyCode::Slash: return ImGuiKey_Slash;
            case KeyCode::Semicolon: return ImGuiKey_Semicolon;
            case KeyCode::Equals: return ImGuiKey_Equal;
            case KeyCode::Leftbracket: return ImGuiKey_LeftBracket;
            case KeyCode::Backslash: return ImGuiKey_Backslash;
            case KeyCode::Rightbracket: return ImGuiKey_RightBracket;
            case KeyCode::Backquote: return ImGuiKey_GraveAccent;
            case KeyCode::Capslock: return ImGuiKey_CapsLock;
            case KeyCode::ScrollLock: return ImGuiKey_ScrollLock;
            case KeyCode::NumLockClear: return ImGuiKey_NumLock;
            case KeyCode::PrintScreen: return ImGuiKey_PrintScreen;
            case KeyCode::Pause: return ImGuiKey_Pause;
            case KeyCode::KeyPad0: return ImGuiKey_Keypad0;
            case KeyCode::KeyPad1: return ImGuiKey_Keypad1;
            case KeyCode::KeyPad2: return ImGuiKey_Keypad2;
            case KeyCode::KeyPad3: return ImGuiKey_Keypad3;
            case KeyCode::KeyPad4: return ImGuiKey_Keypad4;
            case KeyCode::KeyPad5: return ImGuiKey_Keypad5;
            case KeyCode::KeyPad6: return ImGuiKey_Keypad6;
            case KeyCode::KeyPad7: return ImGuiKey_Keypad7;
            case KeyCode::KeyPad8: return ImGuiKey_Keypad8;
            case KeyCode::KeyPad9: return ImGuiKey_Keypad9;
            case KeyCode::KeyPadPeriod: return ImGuiKey_KeypadDecimal;
            case KeyCode::KeyPadDivide: return ImGuiKey_KeypadDivide;
            case KeyCode::KeyPadMultiply: return ImGuiKey_KeypadMultiply;
            case KeyCode::KeyPadMinus: return ImGuiKey_KeypadSubtract;
            case KeyCode::KeyPadPlus: return ImGuiKey_KeypadAdd;
            case KeyCode::KeyPadEnter: return ImGuiKey_KeypadEnter;
            case KeyCode::KeyPadEquals: return ImGuiKey_KeypadEqual;
            case KeyCode::LCtrl: return ImGuiKey_LeftCtrl;
            case KeyCode::LShift: return ImGuiKey_LeftShift;
            case KeyCode::LAlt: return ImGuiKey_LeftAlt;
            case KeyCode::LSuper: return ImGuiKey_LeftSuper;
            case KeyCode::RCtrl: return ImGuiKey_RightCtrl;
            case KeyCode::RShift: return ImGuiKey_RightShift;
            case KeyCode::RAlt: return ImGuiKey_RightAlt;
            case KeyCode::RSuper: return ImGuiKey_RightSuper;
            case KeyCode::Application: return ImGuiKey_Menu;
            case KeyCode::Zero: return ImGuiKey_0;
            case KeyCode::One: return ImGuiKey_1;
            case KeyCode::Two: return ImGuiKey_2;
            case KeyCode::Three: return ImGuiKey_3;
            case KeyCode::Four: return ImGuiKey_4;
            case KeyCode::Five: return ImGuiKey_5;
            case KeyCode::Six: return ImGuiKey_6;
            case KeyCode::Seven: return ImGuiKey_7;
            case KeyCode::Eight: return ImGuiKey_8;
            case KeyCode::Nine: return ImGuiKey_9;
            case KeyCode::a: return ImGuiKey_A;
            case KeyCode::b: return ImGuiKey_B;
            case KeyCode::c: return ImGuiKey_C;
            case KeyCode::d: return ImGuiKey_D;
            case KeyCode::e: return ImGuiKey_E;
            case KeyCode::f: return ImGuiKey_F;
            case KeyCode::g: return ImGuiKey_G;
            case KeyCode::h: return ImGuiKey_H;
            case KeyCode::i: return ImGuiKey_I;
            case KeyCode::j: return ImGuiKey_J;
            case KeyCode::k: return ImGuiKey_K;
            case KeyCode::l: return ImGuiKey_L;
            case KeyCode::m: return ImGuiKey_M;
            case KeyCode::n: return ImGuiKey_N;
            case KeyCode::o: return ImGuiKey_O;
            case KeyCode::p: return ImGuiKey_P;
            case KeyCode::q: return ImGuiKey_Q;
            case KeyCode::r: return ImGuiKey_R;
            case KeyCode::s: return ImGuiKey_S;
            case KeyCode::t: return ImGuiKey_T;
            case KeyCode::u: return ImGuiKey_U;
            case KeyCode::v: return ImGuiKey_V;
            case KeyCode::w: return ImGuiKey_W;
            case KeyCode::x: return ImGuiKey_X;
            case KeyCode::y: return ImGuiKey_Y;
            case KeyCode::z: return ImGuiKey_Z;
            case KeyCode::F1: return ImGuiKey_F1;
            case KeyCode::F2: return ImGuiKey_F2;
            case KeyCode::F3: return ImGuiKey_F3;
            case KeyCode::F4: return ImGuiKey_F4;
            case KeyCode::F5: return ImGuiKey_F5;
            case KeyCode::F6: return ImGuiKey_F6;
            case KeyCode::F7: return ImGuiKey_F7;
            case KeyCode::F8: return ImGuiKey_F8;
            case KeyCode::F9: return ImGuiKey_F9;
            case KeyCode::F10: return ImGuiKey_F10;
            case KeyCode::F11: return ImGuiKey_F11;
            case KeyCode::F12: return ImGuiKey_F12;
            default: return ImGuiKey_None;
        }
    }

}// namespace JE