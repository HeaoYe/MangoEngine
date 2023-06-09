#include "MangoEngine/imgui/imgui_renderer.hpp"

namespace MangoEngine {
    ImGuiBackend::~ImGuiBackend() = default;
    void ImGuiBackend::begin_imgui() {}
    void ImGuiBackend::end_imgui() {}

    implement_runtime_system_start(ImGuiRenderer, imgui_renderer)
        switch (engine_config->api) {
        case RenderAPI::eNone:
            MG_FATAL("Not impl for None yet.")
            break;
        case RenderAPI::eOpenGL:
            MG_FATAL("Not impl for OpenGL yet.")
            break;
        case RenderAPI::eVulkan:
            _instance.reset(new ImGuiRenderer(get_imgui_backend<RenderAPI::eVulkan>()));
            break;
        case RenderAPI::eDirectX:
            MG_FATAL("Not impl for DirectX yet.")
            break;
        case RenderAPI::eMetal:
            MG_FATAL("Not impl for Metal yet.")
            break;
        }
    implement_runtime_system_end(ImGuiRenderer, imgui_renderer)

    ImGuiRenderer::ImGuiRenderer(ImGuiBackend *backend) {
        this->backend.reset(backend);
    }

    ImGuiRenderer::~ImGuiRenderer() {
        backend.reset();
    }

    void ImGuiRenderer::begin_imgui() {
        backend->begin_imgui();
    }

    void ImGuiRenderer::end_imgui() {
        backend->end_imgui();
    }
}
