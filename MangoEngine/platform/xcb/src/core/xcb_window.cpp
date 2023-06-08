#include "MangoEngine/core/window.hpp"

#if defined (MANGO_USE_XCB_WINDOW)

#include <xcb/xcb.h>

namespace MangoEngine {
    class X11Window : public Window {
    public:
        X11Window() {
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
                XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY
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

        ~X11Window() override {
            u32 kb_value_list[] = {
                XCB_AUTO_REPEAT_MODE_ON, 0
            };
            xcb_change_keyboard_control(connection, XCB_KB_AUTO_REPEAT_MODE, kb_value_list);
            xcb_flush(connection);
            xcb_flush(connection);

            xcb_destroy_window(connection, window);
            xcb_disconnect(connection);
        }

        Bool pull_events() override {
            xcb_generic_event_t *event;
            xcb_client_message_event_t *cm;
            Bool alive = MG_TRUE;

            while ((event = xcb_poll_for_event(connection)) != 0) {
                switch (event->response_type & ~0x80) {
                    case XCB_KEY_PRESS: {
                        xcb_key_press_event_t *key_press_event = (xcb_key_press_event_t *)event;
                        break;
                    }
                    case XCB_KEY_RELEASE: {
                        xcb_key_release_event_t *key_release_event = (xcb_key_release_event_t *)event;
                        break;
                    }
                    case XCB_BUTTON_PRESS: {
                        xcb_button_press_event_t *btn_press_event = (xcb_button_press_event_t *)event;
                        break;
                    }
                    case XCB_BUTTON_RELEASE: {
                        xcb_button_release_event_t *btn_release_event = (xcb_button_release_event_t *)event;
                        break;
                    }
                    case XCB_MOTION_NOTIFY: {
                        xcb_motion_notify_event_t *motion_notify_event = (xcb_motion_notify_event_t *)event;
                        break;
                    }
                    case XCB_CONFIGURE_NOTIFY: {
                        xcb_configure_notify_event_t *configure_notify_event = (xcb_configure_notify_event_t *)event;
                        MG_WARN("{} {} {} {}", configure_notify_event->x, configure_notify_event->y, configure_notify_event->width, configure_notify_event->height)
                        break;
                    }
                    case XCB_CLIENT_MESSAGE: {
                        cm = (xcb_client_message_event_t *)event;
                        if (cm->data.data32[0] == wm_delete_window) {
                            alive = MG_FALSE;
                        }
                    } break;
                    default: break;
                }
                free(event);
            }
            return alive;
        }

    private:
        xcb_connection_t *connection;
        xcb_screen_t *screen;
        xcb_window_t window;
        xcb_atom_t wm_delete_window;
        xcb_atom_t wm_protocols;

    no_copy_and_move_construction(X11Window)
    };

    implement_runtime_system(Window)
        _instance.reset(new X11Window());
    }
}

#endif
