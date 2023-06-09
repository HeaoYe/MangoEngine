#pragma once

#include "../commons.hpp"

namespace MangoEngine {
    class Application {
    public:
        virtual void generate_engine_config(EngineConfig *engine_config) = 0;
        virtual Result initialize() = 0;
        virtual Result on_draw_frame() = 0;
        virtual Result on_draw_imgui() = 0;
        virtual Result on_update(f32 dt) = 0;
        virtual Result quit() = 0;
    };

    extern Application *create_application();
}
