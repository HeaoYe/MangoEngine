#include "MangoEngine/core/window.hpp"

#if defined (MANGO_USE_XCB_WINDOW)

#include <xcb/xcb.h>

namespace MangoEngine {
    class XcbWindowSystem : public WindowSystem {
    public:
        XcbWindowSystem();
        ~XcbWindowSystem() override;
        Bool pull_events() override;

        xcb_connection_t *get_connection() const { return connection; }
        xcb_window_t get_window() const { return window; }

    private:
        xcb_connection_t *connection;
        xcb_screen_t *screen;
        xcb_window_t window;
        xcb_atom_t wm_delete_window;
        xcb_atom_t wm_protocols;

    no_copy_and_move_construction(XcbWindowSystem)
    };
}

#endif
