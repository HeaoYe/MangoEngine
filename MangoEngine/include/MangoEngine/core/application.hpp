#pragma once

#include "../commons.hpp"

namespace MangoEngine {
    class Application {
    public:
        virtual Result initialize() = 0;
        virtual Result on_draw_frame() = 0;
        virtual Result on_draw_imgui() = 0;
        virtual Result on_update(f32 dt) = 0;
        virtual Result quit() = 0;
    };

    extern Application *create_application();
}
