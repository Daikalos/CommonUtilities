#include <CommonUtilities/Input/Keyboard.h>

#include <Windows.h>

using namespace CommonUtilities;

Keyboard::Key Keyboard::VirtualKeyToCUKey(int aVirtualKey)
{
    switch (aVirtualKey)
    {
        case 'A':           return Keyboard::A;
        case 'B':           return Keyboard::B;
        case 'C':           return Keyboard::C;
        case 'D':           return Keyboard::D;
        case 'E':           return Keyboard::E;
        case 'F':           return Keyboard::F;
        case 'G':           return Keyboard::G;
        case 'H':           return Keyboard::H;
        case 'I':           return Keyboard::I;
        case 'J':           return Keyboard::J;
        case 'K':           return Keyboard::K;
        case 'L':           return Keyboard::L;
        case 'M':           return Keyboard::M;
        case 'N':           return Keyboard::N;
        case 'O':           return Keyboard::O;
        case 'P':           return Keyboard::P;
        case 'Q':           return Keyboard::Q;
        case 'R':           return Keyboard::R;
        case 'S':           return Keyboard::S;
        case 'T':           return Keyboard::T;
        case 'U':           return Keyboard::U;
        case 'V':           return Keyboard::V;
        case 'W':           return Keyboard::W;
        case 'X':           return Keyboard::X;
        case 'Y':           return Keyboard::Y;
        case 'Z':           return Keyboard::Z;
        case '0':           return Keyboard::Num0;
        case '1':           return Keyboard::Num1;
        case '2':           return Keyboard::Num2;
        case '3':           return Keyboard::Num3;
        case '4':           return Keyboard::Num4;
        case '5':           return Keyboard::Num5;
        case '6':           return Keyboard::Num6;
        case '7':           return Keyboard::Num7;
        case '8':           return Keyboard::Num8;
        case '9':           return Keyboard::Num9;
        case VK_ESCAPE:     return Keyboard::Escape;
        case VK_LCONTROL:   return Keyboard::LControl;
        case VK_LSHIFT:     return Keyboard::LShift;
        case VK_LMENU:      return Keyboard::LAlt;
        case VK_LWIN:       return Keyboard::LSystem;
        case VK_RCONTROL:   return Keyboard::RControl;
        case VK_RSHIFT:     return Keyboard::RShift;
        case VK_RMENU:      return Keyboard::RAlt;
        case VK_RWIN:       return Keyboard::RSystem;
        case VK_APPS:       return Keyboard::Menu;
        case VK_OEM_4:      return Keyboard::LBracket;
        case VK_OEM_6:      return Keyboard::RBracket;
        case VK_OEM_1:      return Keyboard::Semicolon;
        case VK_OEM_COMMA:  return Keyboard::Comma;
        case VK_OEM_PERIOD: return Keyboard::Period;
        case VK_OEM_7:      return Keyboard::Apostrophe;
        case VK_OEM_2:      return Keyboard::Slash;
        case VK_OEM_5:      return Keyboard::Backslash;
        case VK_OEM_3:      return Keyboard::Grave;
        case VK_OEM_PLUS:   return Keyboard::Equal;
        case VK_OEM_MINUS:  return Keyboard::Hyphen;
        case VK_SPACE:      return Keyboard::Space;
        case VK_RETURN:     return Keyboard::Enter;
        case VK_BACK:       return Keyboard::Backspace;
        case VK_TAB:        return Keyboard::Tab;
        case VK_LEFT:       return Keyboard::Left;
        case VK_RIGHT:      return Keyboard::Right;
        case VK_UP:         return Keyboard::Up;
        case VK_DOWN:       return Keyboard::Down;
        case VK_PRIOR:      return Keyboard::PageUp;
        case VK_NEXT:       return Keyboard::PageDown;
        case VK_END:        return Keyboard::End;
        case VK_HOME:       return Keyboard::Home;
        case VK_INSERT:     return Keyboard::Insert;
        case VK_DELETE:     return Keyboard::Delete;
        case VK_ADD:        return Keyboard::Add;
        case VK_SUBTRACT:   return Keyboard::Subtract;
        case VK_MULTIPLY:   return Keyboard::Multiply;
        case VK_DIVIDE:     return Keyboard::Divide;
        case VK_NUMPAD0:    return Keyboard::Numpad0;
        case VK_NUMPAD1:    return Keyboard::Numpad1;
        case VK_NUMPAD2:    return Keyboard::Numpad2;
        case VK_NUMPAD3:    return Keyboard::Numpad3;
        case VK_NUMPAD4:    return Keyboard::Numpad4;
        case VK_NUMPAD5:    return Keyboard::Numpad5;
        case VK_NUMPAD6:    return Keyboard::Numpad6;
        case VK_NUMPAD7:    return Keyboard::Numpad7;
        case VK_NUMPAD8:    return Keyboard::Numpad8;
        case VK_NUMPAD9:    return Keyboard::Numpad9;
        case VK_F1:         return Keyboard::F1;
        case VK_F2:         return Keyboard::F2;
        case VK_F3:         return Keyboard::F3;
        case VK_F4:         return Keyboard::F4;
        case VK_F5:         return Keyboard::F5;
        case VK_F6:         return Keyboard::F6;
        case VK_F7:         return Keyboard::F7;
        case VK_F8:         return Keyboard::F8;
        case VK_F9:         return Keyboard::F9;
        case VK_F10:        return Keyboard::F10;
        case VK_F11:        return Keyboard::F11;
        case VK_F12:        return Keyboard::F12;
        case VK_F13:        return Keyboard::F13;
        case VK_F14:        return Keyboard::F14;
        case VK_F15:        return Keyboard::F15;
        case VK_PAUSE:      return Keyboard::Pause;
        default:            return Keyboard::None;
    }
}

int Keyboard::CUKeyToVirtualKey(Keyboard::Key aKey)
{
    switch (aKey)
    {
        case Keyboard::A:           return 'A';
        case Keyboard::B:           return 'B';
        case Keyboard::C:           return 'C';
        case Keyboard::D:           return 'D';
        case Keyboard::E:           return 'E';
        case Keyboard::F:           return 'F';
        case Keyboard::G:           return 'G';
        case Keyboard::H:           return 'H';
        case Keyboard::I:           return 'I';
        case Keyboard::J:           return 'J';
        case Keyboard::K:           return 'K';
        case Keyboard::L:           return 'L';
        case Keyboard::M:           return 'M';
        case Keyboard::N:           return 'N';
        case Keyboard::O:           return 'O';
        case Keyboard::P:           return 'P';
        case Keyboard::Q:           return 'Q';
        case Keyboard::R:           return 'R';
        case Keyboard::S:           return 'S';
        case Keyboard::T:           return 'T';
        case Keyboard::U:           return 'U';
        case Keyboard::V:           return 'V';
        case Keyboard::W:           return 'W';
        case Keyboard::X:           return 'X';
        case Keyboard::Y:           return 'Y';
        case Keyboard::Z:           return 'Z';
        case Keyboard::Num0:        return '0';
        case Keyboard::Num1:        return '1';
        case Keyboard::Num2:        return '2';
        case Keyboard::Num3:        return '3';
        case Keyboard::Num4:        return '4';
        case Keyboard::Num5:        return '5';
        case Keyboard::Num6:        return '6';
        case Keyboard::Num7:        return '7';
        case Keyboard::Num8:        return '8';
        case Keyboard::Num9:        return '9';
        case Keyboard::Escape:      return VK_ESCAPE;
        case Keyboard::LControl:    return VK_LCONTROL;
        case Keyboard::LShift:      return VK_LSHIFT;
        case Keyboard::LAlt:        return VK_LMENU;
        case Keyboard::LSystem:     return VK_LWIN;
        case Keyboard::RControl:    return VK_RCONTROL;
        case Keyboard::RShift:      return VK_RSHIFT;
        case Keyboard::RAlt:        return VK_RMENU;
        case Keyboard::RSystem:     return VK_RWIN;
        case Keyboard::Menu:        return VK_APPS;
        case Keyboard::LBracket:    return VK_OEM_4;
        case Keyboard::RBracket:    return VK_OEM_6;
        case Keyboard::Semicolon:   return VK_OEM_1;
        case Keyboard::Comma:       return VK_OEM_COMMA;
        case Keyboard::Period:      return VK_OEM_PERIOD;
        case Keyboard::Apostrophe:  return VK_OEM_7;
        case Keyboard::Slash:       return VK_OEM_2;
        case Keyboard::Backslash:   return VK_OEM_5;
        case Keyboard::Grave:       return VK_OEM_3;
        case Keyboard::Equal:       return VK_OEM_PLUS;
        case Keyboard::Hyphen:      return VK_OEM_MINUS;
        case Keyboard::Space:       return VK_SPACE;
        case Keyboard::Enter:       return VK_RETURN;
        case Keyboard::Backspace:   return VK_BACK;
        case Keyboard::Tab:         return VK_TAB;
        case Keyboard::Left:        return VK_LEFT;
        case Keyboard::Right:       return VK_RIGHT;
        case Keyboard::Up:          return VK_UP;
        case Keyboard::Down:        return VK_DOWN;
        case Keyboard::PageUp:      return VK_PRIOR;
        case Keyboard::PageDown:    return VK_NEXT;
        case Keyboard::End:         return VK_END;
        case Keyboard::Home:        return VK_HOME;
        case Keyboard::Insert:      return VK_INSERT;
        case Keyboard::Delete:      return VK_DELETE;
        case Keyboard::Add:         return VK_ADD;
        case Keyboard::Subtract:    return VK_SUBTRACT;
        case Keyboard::Multiply:    return VK_MULTIPLY;
        case Keyboard::Divide:      return VK_DIVIDE;
        case Keyboard::Numpad0:     return VK_NUMPAD0;
        case Keyboard::Numpad1:     return VK_NUMPAD1;
        case Keyboard::Numpad2:     return VK_NUMPAD2;
        case Keyboard::Numpad3:     return VK_NUMPAD3;
        case Keyboard::Numpad4:     return VK_NUMPAD4;
        case Keyboard::Numpad5:     return VK_NUMPAD5;
        case Keyboard::Numpad6:     return VK_NUMPAD6;
        case Keyboard::Numpad7:     return VK_NUMPAD7;
        case Keyboard::Numpad8:     return VK_NUMPAD8;
        case Keyboard::Numpad9:     return VK_NUMPAD9;
        case Keyboard::F1:          return VK_F1;
        case Keyboard::F2:          return VK_F2;
        case Keyboard::F3:          return VK_F3;
        case Keyboard::F4:          return VK_F4;
        case Keyboard::F5:          return VK_F5;
        case Keyboard::F6:          return VK_F6;
        case Keyboard::F7:          return VK_F7;
        case Keyboard::F8:          return VK_F8;
        case Keyboard::F9:          return VK_F9;
        case Keyboard::F10:         return VK_F10;
        case Keyboard::F11:         return VK_F11;
        case Keyboard::F12:         return VK_F12;
        case Keyboard::F13:         return VK_F13;
        case Keyboard::F14:         return VK_F14;
        case Keyboard::F15:         return VK_F15;
        case Keyboard::Pause:       return VK_PAUSE;
        default:                    return 0;
    }
}

bool Keyboard::IsKeyPressed(Key aKey)
{
    const int virtualKey = CUKeyToVirtualKey(aKey);
    return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}
