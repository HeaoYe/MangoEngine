#include "MangoEngine/core/input.hpp"

namespace MangoEngine {
    implement_runtime_system(InputSystem, input_system)

    InputSystem::InputSystem() {
        states = new InputState[2]();
    }

    InputSystem::~InputSystem() {
        delete [] states;
    };

    void InputSystem::swap_state() {
        current_state = !current_state;
        memcpy(&states[current_state], &states[!current_state], sizeof(states[0]));
    }

    Bool InputSystem::is_key_down(Key key) {
        return states[current_state].key_states[static_cast<u32>(key)] == MG_DOWN ? MG_TRUE : MG_FALSE;
    }

    Bool InputSystem::was_key_down(Key key) {
        return states[!current_state].key_states[static_cast<u32>(key)] == MG_DOWN ? MG_TRUE : MG_FALSE;
    }

    Bool InputSystem::is_key_up(Key key) {
        return states[current_state].key_states[static_cast<u32>(key)] == MG_UP ? MG_TRUE : MG_FALSE;
    }

    Bool InputSystem::was_key_up(Key key) {
        return states[!current_state].key_states[static_cast<u32>(key)] == MG_UP ? MG_TRUE : MG_FALSE;
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

    glm::uvec2 InputSystem::get_mouse_pos() {
        return { states[current_state].mouse_x, states[current_state].mouse_y };
    }

    glm::uvec2 InputSystem::get_last_mouse_pos() {
        return { states[!current_state].mouse_x, states[!current_state].mouse_y };
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

    i32 InputSystem::get_mouse_scroll_x() {
        return states[current_state].mouse_scroll_x;
    }

    i32 InputSystem::get_last_mouse_scroll_x() {
        return states[!current_state].mouse_scroll_x;
    }

    i32 InputSystem::get_mouse_scroll_y() {
        return states[current_state].mouse_scroll_y;
    }

    i32 InputSystem::get_last_mouse_scroll_y() {
        return states[!current_state].mouse_scroll_y;
    }
}
