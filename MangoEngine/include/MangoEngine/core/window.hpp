#pragma once

#include "../commons.hpp"

namespace MangoEngine {
    class WindowSystem {
    public:
        virtual Bool pull_events();
    protected:
        u32 x, y;
        u32 width, height;
        const char *title;

    declare_runtime_system(WindowSystem)
    };
    declare_runtime_system_alias(WindowSystem, window)
}
