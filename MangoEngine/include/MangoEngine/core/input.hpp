#pragma once

#include "event.hpp"

extern int main();

namespace MangoEngine {
    struct InputState {
        ButtonState key_states[static_cast<u32>(Key::MAX_NUM)];
        ButtonState mouse_button_states[static_cast<u32>(MouseButton::MAX_NUM)];
        u32 mouse_x, mouse_y;
        i32 mouse_scroll_x, mouse_scroll_y;
    };

    class WindowSystem;

    class InputSystem {
        friend WindowSystem;
        friend int ::main();
    public:
        Bool is_key_down(Key key);
        Bool was_key_down(Key key);
        Bool is_key_up(Key key);
        Bool was_key_up(Key key);
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
        i32 get_mouse_scroll_x();
        i32 get_last_mouse_scroll_x();
        i32 get_mouse_scroll_y();
        i32 get_last_mouse_scroll_y();

    private:
        void swap_state();

    private:
        InputState *states;
        u8 current_state = 0;

    declare_runtime_system(InputSystem)
    };
    declare_runtime_system_alias(InputSystem, input_system);
}
