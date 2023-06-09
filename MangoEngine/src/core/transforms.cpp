#include <MangoRHI/MangoRHI.hpp>
#include "MangoEngine/core/window.hpp"
#include "MangoEngine/imgui/imgui_renderer.hpp"

namespace MangoEngine {
    template<>
    spdlog::level::level_enum transform<LogLevel, spdlog::level::level_enum>(LogLevel level) {
        switch (level) {
        case LogLevel::eTrace:
            return spdlog::level::trace;
        case LogLevel::eDebug:
            return spdlog::level::debug;
        case LogLevel::eInfo:
            return spdlog::level::info;
        case LogLevel::eWarn:
            return spdlog::level::warn;
        case LogLevel::eError:
            return spdlog::level::err;
        case LogLevel::eFatal:
            return spdlog::level::critical;
        }
    }

    template<>
    MangoRHI::LogLevel transform<LogLevel, MangoRHI::LogLevel>(LogLevel level) {
        switch (level) {
        case LogLevel::eTrace:
            return MangoRHI::LogLevel::eTrace;
        case LogLevel::eDebug:
            return MangoRHI::LogLevel::eDebug;
        case LogLevel::eInfo:
            return MangoRHI::LogLevel::eInfo;
        case LogLevel::eWarn:
            return MangoRHI::LogLevel::eWarn;
        case LogLevel::eError:
            return MangoRHI::LogLevel::eError;
        case LogLevel::eFatal:
            return MangoRHI::LogLevel::eFatal;
        }
    }

    template<>
    MangoRHI::API transform<RenderAPI, MangoRHI::API>(RenderAPI api) {
        switch (api) {
        case RenderAPI::eNone:
            return MangoRHI::API::eNone;
        case RenderAPI::eOpenGL:
            return MangoRHI::API::eOpenGL;
        case RenderAPI::eVulkan:
            return MangoRHI::API::eVulkan;
        case RenderAPI::eDirectX:
            return MangoRHI::API::eDirectX;
        case RenderAPI::eMetal:
            return MangoRHI::API::eMetal;
        }
    }

    template<>
    MouseButton transform<int, MouseButton>(int button) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            return MouseButton::eLeft;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            return MouseButton::eMiddle;
        case GLFW_MOUSE_BUTTON_RIGHT:
            return MouseButton::eRight;
        default:
            return MouseButton::MAX_NUM;
        }
    }

    #define case_key(glfw_key_name, key_name) \
    case GLFW_KEY_##glfw_key_name: \
        return Key::e##key_name;

    template<>
    Key transform<int, Key>(int key) {
        switch (key) {
        case_key(TAB, Tab)
        case_key(LEFT, LeftArrow)
        case_key(RIGHT, RightArrow)
        case_key(UP, UpArrow)
        case_key(DOWN, DownArrow)
        case_key(PAGE_UP, PageUp)
        case_key(PAGE_DOWN, PageDown)
        case_key(HOME, Home)
        case_key(END, End)
        case_key(INSERT, Insert)
        case_key(DELETE, Delete)
        case_key(BACKSPACE, Backspace)
        case_key(SPACE, Space)
        case_key(ENTER, Enter)
        case_key(ESCAPE, Escape)
        case_key(LEFT_CONTROL, LeftCtrl)
        case_key(LEFT_SHIFT, LeftShift)
        case_key(LEFT_ALT, LeftAlt)
        case_key(LEFT_SUPER, LeftSuper)
        case_key(RIGHT_CONTROL, RightCtrl)
        case_key(RIGHT_SHIFT, RightShift)
        case_key(RIGHT_ALT, RightAlt)
        case_key(RIGHT_SUPER, RightSuper)
        case_key(MENU, Menu)
        case_key(0, 0)
        case_key(1, 1)
        case_key(2, 2)
        case_key(3, 3)
        case_key(4, 4)
        case_key(5, 5)
        case_key(6, 6)
        case_key(7, 7)
        case_key(8, 8)
        case_key(9, 9)
        case_key(A, A)
        case_key(B, B)
        case_key(C, C)
        case_key(D, D)
        case_key(E, E)
        case_key(F, F)
        case_key(G, G)
        case_key(H, H)
        case_key(I, I)
        case_key(J, J)
        case_key(K, K)
        case_key(L, L)
        case_key(M, M)
        case_key(N, N)
        case_key(O, O)
        case_key(P, P)
        case_key(Q, Q)
        case_key(R, R)
        case_key(S, S)
        case_key(T, T)
        case_key(U, U)
        case_key(V, V)
        case_key(W, W)
        case_key(X, X)
        case_key(Y, Y)
        case_key(Z, Z)
        case_key(F1, F1)
        case_key(F2, F2)
        case_key(F3, F3)
        case_key(F4, F4)
        case_key(F5, F5)
        case_key(F6, F6)
        case_key(F7, F7)
        case_key(F8, F8)
        case_key(F9, F9)
        case_key(F10, F10)
        case_key(F11, F11)
        case_key(F12, F12)
        case_key(APOSTROPHE, Apostrophe)
        case_key(COMMA, Comma)
        case_key(MINUS, Minus)
        case_key(PERIOD, Dot)
        case_key(SLASH, Slash)
        case_key(SEMICOLON, Semicolon)
        case_key(EQUAL, Equal)
        case_key(LEFT_BRACKET, LeftBracket)
        case_key(RIGHT_BRACKET, RightBracket)
        case_key(BACKSLASH, Backslash)
        case_key(GRAVE_ACCENT, GraveAccent)
        case_key(CAPS_LOCK, CapsLock)
        case_key(SCROLL_LOCK, ScrollLock)
        case_key(NUM_LOCK, NumLock)
        case_key(PRINT_SCREEN, PrintScreen)
        case_key(PAUSE, Pause)
        default:
            return Key::MAX_NUM;
        }
    }

    #undef case_key

    #define case_key(key_name) \
    case Key::e##key_name: \
        return ImGuiKey_##key_name;

    template<>
    ImGuiKey transform<Key, ImGuiKey>(Key key) {
        switch (key) {
        case_key(Tab)
        case_key(LeftArrow)
        case_key(RightArrow)
        case_key(UpArrow)
        case_key(DownArrow)
        case_key(PageUp)
        case_key(PageDown)
        case_key(Home)
        case_key(End)
        case_key(Insert)
        case_key(Delete)
        case_key(Backspace)
        case_key(Space)
        case_key(Enter)
        case_key(Escape)
        case_key(LeftCtrl)
        case_key(LeftShift)
        case_key(LeftAlt)
        case_key(LeftSuper)
        case_key(RightCtrl)
        case_key(RightShift)
        case_key(RightAlt)
        case_key(RightSuper)
        case_key(Menu)
        case_key(0)
        case_key(1)
        case_key(2)
        case_key(3)
        case_key(4)
        case_key(5)
        case_key(6)
        case_key(7)
        case_key(8)
        case_key(9)
        case_key(A)
        case_key(B)
        case_key(C)
        case_key(D)
        case_key(E)
        case_key(F)
        case_key(G)
        case_key(H)
        case_key(I)
        case_key(J)
        case_key(K)
        case_key(L)
        case_key(M)
        case_key(N)
        case_key(O)
        case_key(P)
        case_key(Q)
        case_key(R)
        case_key(S)
        case_key(T)
        case_key(U)
        case_key(V)
        case_key(W)
        case_key(X)
        case_key(Y)
        case_key(Z)
        case_key(F1)
        case_key(F2)
        case_key(F3)
        case_key(F4)
        case_key(F5)
        case_key(F6)
        case_key(F7)
        case_key(F8)
        case_key(F9)
        case_key(F10)
        case_key(F11)
        case_key(F12)
        case_key(Apostrophe)
        case_key(Comma)
        case_key(Minus)
        case Key::eDot:
            return ImGuiKey_Period;
        case_key(Slash)
        case_key(Semicolon)
        case_key(Equal)
        case_key(LeftBracket)
        case_key(RightBracket)
        case_key(Backslash)
        case_key(GraveAccent)
        case_key(CapsLock)
        case_key(ScrollLock)
        case_key(NumLock)
        case_key(PrintScreen)
        case_key(Pause)
        case Key::MAX_NUM:
            return ImGuiKey_None;
        }
    }

    #undef case_key
}
