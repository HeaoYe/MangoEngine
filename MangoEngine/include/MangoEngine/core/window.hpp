#pragma once

#include "../commons.hpp"

namespace MangoEngine {
    class WindowSystem {
    public:
        Bool pull_events();
        void *get_native_pointer() { return native_pointer; }
        u32 get_x() const { return x; }
        u32 get_y() const { return y; }
        u32 get_width() const { return width; }
        u32 get_height() const { return height; }

    private:
        u32 x, y;
        u32 width, height;
        const char *title;
        void *native_pointer;

    declare_runtime_system(WindowSystem)
    };
    declare_runtime_system_alias(WindowSystem, window_system)
}
