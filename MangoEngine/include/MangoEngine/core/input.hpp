#pragma once

#include "event.hpp"

extern int main();

namespace MangoEngine {
    constexpr u32 max_key_code = static_cast<u32>(Key::MAX_NUM);
    constexpr u32 max_mouse_button = static_cast<u32>(MouseButton::MAX_NUM);

    class XcbWindowSystem;

    struct InputState {
        ButtonState key_states[max_key_code];
        ButtonState mouse_button_states[max_mouse_button];
        u32 mouse_x;
        u32 mouse_y;
        i32 mouse_scroll;
    };

    class InputSystem {
        friend int ::main();
        friend XcbWindowSystem;
    public:
        Bool is_key_down(u32 key);
        Bool was_key_down(u32 key);
        Bool is_key_up(u32 key);
        Bool was_key_up(u32 key);
        Bool is_mouse_button_down(MouseButton button);
        Bool was_mouse_button_down(MouseButton button);
        Bool is_mouse_button_up(MouseButton button);
        Bool was_mouse_button_up(MouseButton button);
        Pos get_mouse_pos();
        Pos get_last_mouse_pos();
        u32 get_mouse_x();
        u32 get_last_mouse_x();
        u32 get_mouse_y();
        u32 get_last_mouse_y();
        i32 get_mouse_scroll();
        i32 get_last_mouse_scroll();

    private:
        void swap_state();

        InputState *states;
        u8 current_state = 0;

    declare_runtime_system(InputSystem)
    };
    declare_runtime_system_alias(InputSystem, input);
}
