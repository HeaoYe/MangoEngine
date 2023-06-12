#pragma once

#include "../../commons.hpp"
#include "../../core/event.hpp"
#include "imgui/imgui.h"

namespace MangoEngine {
    class ImGuiBackend {
    public:
        virtual ~ImGuiBackend();
        virtual void begin_imgui();
        virtual void end_imgui();
        virtual ImTextureID get_scene_texture();
    };

    template <RenderAPI api>
    ImGuiBackend *get_imgui_backend();

    class ImGuiRenderer {
    public:
        void begin_imgui();
        void end_imgui();
        ImTextureID get_scene_texture();

    private:
        std::unique_ptr<ImGuiBackend> backend;

    declare_runtime_system(ImGuiRenderer);
    };
    declare_runtime_system_alias(ImGuiRenderer, imgui_renderer)
}
