#pragma once

#include "../commons.hpp"
#include "command.hpp"

namespace MangoEngine {
    template<RenderAPI api>
    extern void *get_context_info();

    class RenderSystem {
    public:
        void set_bg_color(f32 r, f32 g, f32 b, f32 a);
        Result acquire();
        Result present();

        void begin_render();
        void end_render();

        RenderCommand &get_render_command() { return *render_command; }

    private:
        MangoRHI::Viewport viewport;
        std::unique_ptr<RenderCommand> render_command;

    declare_runtime_system(RenderSystem)
    };
    declare_runtime_system_alias(RenderSystem, render_system)
}
