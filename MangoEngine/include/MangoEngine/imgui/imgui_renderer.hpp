#pragma once

#include "../commons.hpp"
#include "imgui/imgui.h"

namespace MangoEngine {
    class ImGuiBackend {
    public:
        virtual ~ImGuiBackend();
        virtual void begin_imgui();
        virtual void end_imgui();
    };

    template <RenderAPI api>
    ImGuiBackend *get_imgui_backend();

    class ImGuiRenderer {
    public:
        void begin_imgui();
        void end_imgui();

    private:
        std::unique_ptr<ImGuiBackend> backend;

    declare_runtime_system(ImGuiRenderer, ImGuiBackend *backend);
    };
    declare_runtime_system_alias(ImGuiRenderer, imgui_renderer)
}
