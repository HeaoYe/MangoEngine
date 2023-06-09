#include "MangoEngine/core/event.hpp"

namespace MangoEngine {
    implement_event(WindowResized) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "width: " << width << ", height: " << height;
        return ss.str();
    }

    implement_event(WindowMoved) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "x: " << x << ", y: " << y;
        return ss.str();
    }

    implement_event(WindowClosed) {
        std::stringstream ss;
        ss << get_type_name();
        return ss.str();
    }

    implement_event(WindowFocusd) {
        return get_type_name();
    }

    implement_event(WindowLostFocus) {
        return get_type_name();
    }

    implement_event(KeyPressed) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "key: " << MangoEngine::to_string(key);
        return ss.str();
    }

    implement_event(KeyReleased) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "key: " << MangoEngine::to_string(key);
        return ss.str();
    }

    implement_event(MousePressed) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "button: " << (u32)button;
        return ss.str();
    }

    implement_event(MouseReleased) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "button: " << (u32)button;
        return ss.str();
    }

    implement_event(MouseMoved) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "x: " << x << ", y: " << y;
        return ss.str();
    }

    implement_event(MouseScroll) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "delta: " << delta;
        return ss.str();
    }

    implement_runtime_system_start(EventSystem, event_system)
        _instance.reset(new EventSystem);
    implement_runtime_system_end(EventSystem, event_system)

    EventSystem::EventSystem() = default;

    EventSystem::~EventSystem() = default;

    #define key2string(key_name) \
    case Key::e##key_name: \
        return #key_name;
    #define key2string_(key_name, str) \
    case Key::e##key_name: \
        return str;

    std::string to_string(Key key) {
        switch (key) {
        key2string(Tab)
        key2string(LeftArrow)
        key2string(RightArrow)
        key2string(UpArrow)
        key2string(DownArrow)
        key2string(PageUp)
        key2string(PageDown)
        key2string(Home)
        key2string(End)
        key2string(Insert)
        key2string(Delete)
        key2string(Backspace)
        key2string(Space)
        key2string(Enter)
        key2string(Escape)
        key2string(LeftCtrl)
        key2string(LeftShift)
        key2string(LeftAlt)
        key2string(LeftSuper)
        key2string(RightCtrl)
        key2string(RightShift)
        key2string(RightAlt)
        key2string(RightSuper)
        key2string(Menu)
        key2string(0)
        key2string(1)
        key2string(2)
        key2string(3)
        key2string(4)
        key2string(5)
        key2string(6)
        key2string(7)
        key2string(8)
        key2string(9)
        key2string(A)
        key2string(B)
        key2string(C)
        key2string(D)
        key2string(E)
        key2string(F)
        key2string(G)
        key2string(H)
        key2string(I)
        key2string(J)
        key2string(K)
        key2string(L)
        key2string(M)
        key2string(N)
        key2string(O)
        key2string(P)
        key2string(Q)
        key2string(R)
        key2string(S)
        key2string(T)
        key2string(U)
        key2string(V)
        key2string(W)
        key2string(X)
        key2string(Y)
        key2string(Z)
        key2string(F1)
        key2string(F2)
        key2string(F3)
        key2string(F4)
        key2string(F5)
        key2string(F6)
        key2string(F7)
        key2string(F8)
        key2string(F9)
        key2string(F10)
        key2string(F11)
        key2string(F12)
        key2string_(Apostrophe, "'")
        key2string_(Comma, ",")
        key2string_(Minus, "-")
        key2string_(Dot, ".")
        key2string_(Slash, "/")
        key2string_(Semicolon, ";")
        key2string_(Equal, "=")
        key2string_(LeftBracket, "[")
        key2string_(RightBracket, "]")
        key2string_(Backslash, "\\")
        key2string_(GraveAccent, "`")
        key2string(CapsLock)
        key2string(ScrollLock)
        key2string(NumLock)
        key2string(PrintScreen)
        key2string(Pause)
        default:
            return "Unknown Key";
        }
    }

    #undef key2string

    Bool is_char(Key key) {
        return (static_cast<u32>(Key::e0) <= static_cast<u32>(key) &&
            static_cast<u32>(key) <= static_cast<u32>(Key::eZ)) ||
            (static_cast<u32>(Key::eApostrophe) <= static_cast<u32>(key) &&
            static_cast<u32>(key) <= static_cast<u32>(Key::eGraveAccent));
    }

    char to_char(Key key) {
        switch (key) {
        key2string_(0, '0')
        key2string_(1, '1')
        key2string_(2, '2')
        key2string_(3, '3')
        key2string_(4, '4')
        key2string_(5, '5')
        key2string_(6, '6')
        key2string_(7, '7')
        key2string_(8, '8')
        key2string_(9, '9')
        key2string_(A, 'A')
        key2string_(B, 'B')
        key2string_(C, 'C')
        key2string_(D, 'D')
        key2string_(E, 'E')
        key2string_(F, 'F')
        key2string_(G, 'G')
        key2string_(H, 'H')
        key2string_(I, 'I')
        key2string_(J, 'J')
        key2string_(K, 'K')
        key2string_(L, 'L')
        key2string_(M, 'M')
        key2string_(N, 'N')
        key2string_(O, 'O')
        key2string_(P, 'P')
        key2string_(Q, 'Q')
        key2string_(R, 'R')
        key2string_(S, 'S')
        key2string_(T, 'T')
        key2string_(U, 'U')
        key2string_(V, 'V')
        key2string_(W, 'W')
        key2string_(X, 'X')
        key2string_(Y, 'Y')
        key2string_(Z, 'Z')
        key2string_(Apostrophe, '\'')
        key2string_(Comma, ',')
        key2string_(Minus, '-')
        key2string_(Dot, '.')
        key2string_(Slash, '/')
        key2string_(Semicolon, ';')
        key2string_(Equal, '=')
        key2string_(LeftBracket, '[')
        key2string_(RightBracket, ']')
        key2string_(Backslash, '\\')
        key2string_(GraveAccent, '`')
        default:
            return '?';
        }
    }

    #undef key2string_
}
