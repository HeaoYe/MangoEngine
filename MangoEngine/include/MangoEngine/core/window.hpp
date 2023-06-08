#pragma once

#include "../commons.hpp"

namespace MangoEngine {
    class Window {
    public:
        virtual Bool pull_events();
    protected:
        u32 x { 0 }, y { 0 };
        u32 width { 320 }, height { 320 };
        const char *title = "";

    declare_runtime_system(Window)
    };
}
