#include "MangoEngine/core/input.hpp"

namespace MangoEngine {
    implement_runtime_system_start(InputSystem, input)
        _instance.reset(new InputSystem());
    implement_runtime_system_end(InputSystem, input)

    InputSystem::InputSystem() {
        memset(states, 0, sizeof(states));
    }

    InputSystem::~InputSystem() = default;

    void InputSystem::swap_state() {
        current_state = !current_state;
        memcpy(&states[current_state], &states[!current_state], sizeof(states[0]));
    }

    Bool InputSystem::is_key_down(u32 key) {
        return states[current_state].key_states[key] == MG_DOWN ? MG_TRUE : MG_FALSE;
    }

    Bool InputSystem::was_key_down(u32 key) {
        return states[!current_state].key_states[key] == MG_DOWN ? MG_TRUE : MG_FALSE;
    }

    Bool InputSystem::is_key_up(u32 key) {
        return states[current_state].key_states[key] == MG_UP ? MG_TRUE : MG_FALSE;
    }

    Bool InputSystem::was_key_up(u32 key) {
        return states[!current_state].key_states[key] == MG_UP ? MG_TRUE : MG_FALSE;
    }

    Bool InputSystem::is_mouse_button_down(MouseButton button) {
        return states[current_state].mouse_button_states[static_cast<u32>(button)] == MG_DOWN ? MG_TRUE : MG_FALSE;
    }

    Bool InputSystem::was_mouse_button_down(MouseButton button) {
        return states[!current_state].mouse_button_states[static_cast<u32>(button)] == MG_DOWN ? MG_TRUE : MG_FALSE;
    }

    Bool InputSystem::is_mouse_button_up(MouseButton button) {
        return states[current_state].mouse_button_states[static_cast<u32>(button)] == MG_UP ? MG_TRUE : MG_FALSE;
    }

    Bool InputSystem::was_mouse_button_up(MouseButton button) {
        return states[!current_state].mouse_button_states[static_cast<u32>(button)] == MG_UP ? MG_TRUE : MG_FALSE;
    }

    Pos InputSystem::get_mouse_pos() {
        return { .x = states[current_state].mouse_x, .y = states[current_state].mouse_y };
    }

    Pos InputSystem::get_last_mouse_pos() {
        return { .x = states[!current_state].mouse_x, .y = states[!current_state].mouse_y };
    }

    u32 InputSystem::get_mouse_x() {
        return states[current_state].mouse_x;
    }

    u32 InputSystem::get_last_mouse_x() {
        return states[!current_state].mouse_x;
    }

    u32 InputSystem::get_mouse_y() {
        return states[current_state].mouse_y;
    }

    u32 InputSystem::get_last_mouse_y() {
        return states[!current_state].mouse_y;
    }

    i32 InputSystem::get_mouse_scroll() {
        return states[current_state].mouse_scroll;
    }

    i32 InputSystem::get_last_mouse_scroll() {
        return states[!current_state].mouse_scroll;
    }
}
