#pragma once

#include "../commons.hpp"
#include <MangoRHI/MangoRHI.hpp>

namespace MangoEngine {
    template<RenderAPI api>
    extern void *get_context_info();

    class RenderSystem {
    public:
        void set_bg_color(f32 r, f32 g, f32 b, f32 a);
        Result begin_render();
        Result end_render();

        MangoRHI::Context &get_context();

    private:
        MangoRHI::Context &context;

    declare_runtime_system(RenderSystem)
    };
    declare_runtime_system_alias(RenderSystem, render_system)
}
