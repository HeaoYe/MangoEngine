#include "xcb_window.hpp"

#if defined (MANGO_USE_XCB_WINDOW)

#include "MangoEngine/core/event.hpp"
#include "MangoEngine/core/input.hpp"
#include "linux/input-event-codes.h"

namespace MangoEngine {
    #define case_keycode(keycode_name, key_name) \
    case KEY_##keycode_name: \
        return Key::e##key_name;

    Key keycode2key(xcb_keycode_t keycode) {
        switch (keycode - 8) {
        case_keycode(TAB, Tab)
        case_keycode(LEFT, LeftArrow)
        case_keycode(RIGHT, RightArrow)
        case_keycode(UP, UpArrow)
        case_keycode(DOWN, DownArrow)
        case_keycode(PAGEUP, PageUp)
        case_keycode(PAGEDOWN, PageDown)
        case_keycode(HOME, Home)
        case_keycode(END, End)
        case_keycode(INSERT, Insert)
        case_keycode(DELETE, Delete)
        case_keycode(BACKSPACE, Backspace)
        case_keycode(SPACE, Space)
        case_keycode(ENTER, Enter)
        case_keycode(ESC, Escape)
        case_keycode(LEFTCTRL, LeftCtrl)
        case_keycode(LEFTSHIFT, LeftShift)
        case_keycode(LEFTALT, LeftAlt)
        case_keycode(LEFTMETA, LeftSuper)
        case_keycode(RIGHTCTRL, RightCtrl)
        case_keycode(RIGHTSHIFT, RightShift)
        case_keycode(RIGHTALT, RightAlt)
        case_keycode(RIGHTMETA, RightSuper)
        case_keycode(MENU, Menu)
        case_keycode(0, 0)
        case_keycode(1, 1)
        case_keycode(2, 2)
        case_keycode(3, 3)
        case_keycode(4, 4)
        case_keycode(5, 5)
        case_keycode(6, 6)
        case_keycode(7, 7)
        case_keycode(8, 8)
        case_keycode(9, 9)
        case_keycode(A, A)
        case_keycode(B, B)
        case_keycode(C, C)
        case_keycode(D, D)
        case_keycode(E, E)
        case_keycode(F, F)
        case_keycode(G, G)
        case_keycode(H, H)
        case_keycode(I, I)
        case_keycode(J, J)
        case_keycode(K, K)
        case_keycode(L, L)
        case_keycode(M, M)
        case_keycode(N, N)
        case_keycode(O, O)
        case_keycode(P, P)
        case_keycode(Q, Q)
        case_keycode(R, R)
        case_keycode(S, S)
        case_keycode(T, T)
        case_keycode(U, U)
        case_keycode(V, V)
        case_keycode(W, W)
        case_keycode(X, X)
        case_keycode(Y, Y)
        case_keycode(Z, Z)
        case_keycode(F1, F1)
        case_keycode(F2, F2)
        case_keycode(F3, F3)
        case_keycode(F4, F4)
        case_keycode(F5, F5)
        case_keycode(F6, F6)
        case_keycode(F7, F7)
        case_keycode(F8, F8)
        case_keycode(F9, F9)
        case_keycode(F10, F10)
        case_keycode(F11, F11)
        case_keycode(F12, F12)
        case_keycode(APOSTROPHE, Apostrophe)
        case_keycode(COMMA, Comma)
        case_keycode(MINUS, Minus)
        case_keycode(DOT, Dot)
        case_keycode(SLASH, Slash)
        case_keycode(SEMICOLON, Semicolon)
        case_keycode(EQUAL, Equal)
        case_keycode(LEFTBRACE, LeftBracket)
        case_keycode(RIGHTBRACE, RightBracket)
        case_keycode(BACKSLASH, Backslash)
        case_keycode(GRAVE, GraveAccent)
        case_keycode(CAPSLOCK, CapsLock)
        case_keycode(SCROLLLOCK, ScrollLock)
        case_keycode(NUMLOCK, NumLock)
        case_keycode(SYSRQ, PrintScreen)
        case_keycode(PAUSE, Pause)
        default:
            return Key::MAX_NUM;
        }
    }

    #undef case_keycode

    XcbWindowSystem::XcbWindowSystem() : WindowSystem() {
        int screen_idx;
        connection = xcb_connect(nullptr, &screen_idx);
        const xcb_setup_t *setup = xcb_get_setup(connection);
        xcb_screen_iterator_t iter  = xcb_setup_roots_iterator(setup);
        for (u32 i = 0; i < screen_idx; i ++) {
            xcb_screen_next(&iter);
        }
        screen = iter.data;

        u32 value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
        u32 value_list[] = {
            screen->black_pixel,
            XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
            XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
            XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_FOCUS_CHANGE
        };

        window = xcb_generate_id(connection);
        xcb_create_window(connection, screen->root_depth, window, screen->root, static_cast<u16>(x), static_cast<u16>(y), static_cast<u16>(width), static_cast<u16>(height), XCB_COPY_FROM_PARENT, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, value_mask, value_list);

        xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(title), title);

        xcb_intern_atom_reply_t *wm_delete_reply = xcb_intern_atom_reply(connection, xcb_intern_atom(connection, 1, strlen("WM_DELETE_WINDOW"), "WM_DELETE_WINDOW"), NULL);
        xcb_intern_atom_reply_t *wm_protocols_reply = xcb_intern_atom_reply(connection, xcb_intern_atom(connection, 1, strlen("WM_PROTOCOLS"), "WM_PROTOCOLS"), NULL);

        wm_delete_window = wm_delete_reply->atom;
        wm_protocols = wm_protocols_reply->atom;
        free(wm_delete_reply);
        free(wm_protocols_reply);
        xcb_change_property(
            connection,
            XCB_PROP_MODE_REPLACE,
            window,
            wm_protocols, XCB_ATOM_ATOM, 32,
            1, &wm_delete_window
        );

        u32 kb_value_list[] = {
            XCB_AUTO_REPEAT_MODE_OFF, 0
        };
        xcb_change_keyboard_control(connection, XCB_KB_AUTO_REPEAT_MODE, kb_value_list);

        xcb_map_window(connection, window);
        int result = xcb_flush(connection);
        if (result < 0) {
            MG_ERROR("Faild create xcb window")
        }
    }

    XcbWindowSystem::~XcbWindowSystem() {
        u32 kb_value_list[] = {
            XCB_AUTO_REPEAT_MODE_ON, 0
        };
        xcb_change_keyboard_control(connection, XCB_KB_AUTO_REPEAT_MODE, kb_value_list);
        xcb_flush(connection);
        xcb_flush(connection);

        xcb_destroy_window(connection, window);
        xcb_disconnect(connection);
    }

    Bool XcbWindowSystem::pull_events() {
        xcb_generic_event_t *event;
        xcb_client_message_event_t *cm;
        Bool alive = MG_TRUE;

        while ((event = xcb_poll_for_event(connection)) != 0) {
            switch (event->response_type & ~0x80) {
            case XCB_KEY_PRESS: {
                xcb_key_press_event_t *key_press_event = (xcb_key_press_event_t *)event;
                Key key = keycode2key(key_press_event->detail);
                if (key == Key::MAX_NUM) {
                    break;
                }
                input->states[input->current_state].key_states[static_cast<u32>(key)] = MG_DOWN;
                event_system->dispatch<KeyPressedEvent>({ .key = key });
                break;
            }
            case XCB_KEY_RELEASE: {
                xcb_key_release_event_t *key_release_event = (xcb_key_release_event_t *)event;
                Key key = keycode2key(key_release_event->detail);
                if (key == Key::MAX_NUM) {
                    break;
                }
                input->states[input->current_state].key_states[static_cast<u32>(key)] = MG_UP;
                event_system->dispatch<KeyReleasedEvent>({ .key = key });
                break;
            }
            case XCB_BUTTON_PRESS: {
                xcb_button_press_event_t *btn_press_event = (xcb_button_press_event_t *)event;
                switch (btn_press_event->detail) {
                case 1:
                    input->states[input->current_state].mouse_button_states[0] = MG_DOWN;
                    event_system->dispatch<MousePressedEvent>({ .button = MouseButton::eLeft });
                    break;
                case 2:
                    input->states[input->current_state].mouse_button_states[1] = MG_DOWN;
                    event_system->dispatch<MousePressedEvent>({ .button = MouseButton::eMiddle });
                    break;
                case 3:
                    input->states[input->current_state].mouse_button_states[2] = MG_DOWN;
                    event_system->dispatch<MousePressedEvent>({ .button = MouseButton::eRight });
                    break;
                case 4:
                    input->states[input->current_state].mouse_scroll = 1;
                    event_system->dispatch<MouseScrollEvent>({ .delta = 1 });
                    break;
                case 5:
                    input->states[input->current_state].mouse_scroll = -1;
                    event_system->dispatch<MouseScrollEvent>({ .delta = -1 });
                    break;
                default:
                    break;
                }
                break;
            }
            case XCB_BUTTON_RELEASE: {
                xcb_button_release_event_t *btn_release_event = (xcb_button_release_event_t *)event;
                switch (btn_release_event->detail) {
                case 1:
                    input->states[input->current_state].mouse_button_states[0] = MG_UP;
                    event_system->dispatch<MouseReleasedEvent>({ .button = MouseButton::eLeft });
                    break;
                case 2:
                    input->states[input->current_state].mouse_button_states[1] = MG_UP;
                    event_system->dispatch<MouseReleasedEvent>({ .button = MouseButton::eMiddle });
                    break;
                case 3:
                    input->states[input->current_state].mouse_button_states[2] = MG_UP;
                    event_system->dispatch<MouseReleasedEvent>({ .button = MouseButton::eRight });
                    break;
                default:
                    break;
                }
                break;
            }
            case XCB_FOCUS_IN: {
                event_system->dispatch<WindowFocusdEvent>({});
                break;
            }
            case XCB_FOCUS_OUT: {
                event_system->dispatch<WindowLostFocusEvent>({});
                break;
            }
            case XCB_MOTION_NOTIFY: {
                xcb_motion_notify_event_t *motion_notify_event = (xcb_motion_notify_event_t *)event;
                input->states[input->current_state].mouse_x = static_cast<u32>(motion_notify_event->event_x);
                input->states[input->current_state].mouse_y = static_cast<u32>(motion_notify_event->event_y);
                event_system->dispatch<MouseMovedEvent>({ .x = static_cast<u32>(motion_notify_event->event_x), .y = static_cast<u32>(motion_notify_event->event_y) });
                break;
            }
            case XCB_CONFIGURE_NOTIFY: {
                xcb_configure_notify_event_t *configure_notify_event = (xcb_configure_notify_event_t *)event;
                if (width != configure_notify_event->width || height != configure_notify_event->height) {
                    width = static_cast<u32>(configure_notify_event->width);
                    height = static_cast<u32>(configure_notify_event->height);
                    event_system->dispatch<WindowResizedEvent>({ .width = width, .height = height });
                }
                if ((x != configure_notify_event->x || y != configure_notify_event->y) && (configure_notify_event->x != 0 || configure_notify_event->y != 0)) {
                    x = static_cast<u32>(configure_notify_event->x);
                    y = static_cast<u32>(configure_notify_event->y);
                    event_system->dispatch<WindowMovedEvent>({ .x = x, .y = y });
                }
                break;
            }
            case XCB_CLIENT_MESSAGE: {
                cm = (xcb_client_message_event_t *)event;
                if (cm->data.data32[0] == wm_delete_window) {
                    alive = MG_FALSE;
                    event_system->dispatch<WindowClosedEvent>({});
                }
                break;
            }
            default:
                break;
            }
            free(event);
        }
        return alive;
    }

    implement_runtime_system_start(WindowSystem, window)
        _instance.reset(new XcbWindowSystem());
    implement_runtime_system_end(WindowSystem, window)
}

#endif
