#include "MangoEngine/render/imgui/imgui_renderer.hpp"
#include "MangoEngine/core/window.hpp"
#include "MangoEngine/render/render.hpp"
#include "imgui/backends/imgui_impl_glfw.h"
#include <GLFW/glfw3.h>

namespace MangoEngine {
    ImGuiBackend::~ImGuiBackend() = default;
    void ImGuiBackend::begin_imgui() {}
    void ImGuiBackend::end_imgui() {}
    ImTextureID ImGuiBackend::get_scene_texture() { return nullptr; }

    implement_runtime_system(ImGuiRenderer, imgui_renderer)

    ImGuiRenderer::ImGuiRenderer() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;
        ImGui::StyleColorsClassic();
        auto &style = ImGui::GetStyle();
        style.WindowMinSize = { 160, 160 };
        style.WindowRounding = 2;

        int width, height;
        glfwGetFramebufferSize(static_cast<GLFWwindow *>(window_system->get_native_pointer()), &width, &height);
        io.DisplaySize.x = static_cast<float>(width);
        io.DisplaySize.y = static_cast<float>(height);
        auto *font_configs = new ImFontConfig[2]();
        font_configs[0].GlyphExtraSpacing = { 0.25f, 0.0f };
        font_configs[0].RasterizerMultiply = 1.2f;
        font_configs[0].MergeMode = false;
        font_configs[1].GlyphExtraSpacing = { 1.0f, 0.0f };
        font_configs[1].RasterizerMultiply = 1.2f;
        font_configs[1].MergeMode = true;
        io.Fonts->AddFontFromFileTTF("assets/fonts/JetBrainsMonoNerdFontMono-Regular.ttf", 22.0f, &font_configs[0], io.Fonts->GetGlyphRangesDefault());
        io.Fonts->AddFontFromFileTTF("assets/fonts/Source Han Sans CN Regular.otf", 18.5f, &font_configs[1], io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

        switch (engine_config->api) {
        case RenderAPI::eOpenGL:
            ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow *>(window_system->get_native_pointer()), true);
            break;
        case RenderAPI::eVulkan:
            ImGui_ImplGlfw_InitForVulkan(static_cast<GLFWwindow *>(window_system->get_native_pointer()), true);
            break;
        default:
            ImGui_ImplGlfw_InitForOther(static_cast<GLFWwindow *>(window_system->get_native_pointer()), true);
        }

        switch (engine_config->api) {
        case RenderAPI::eNone:
            MG_FATAL("Not impl for None yet.")
            break;
        case RenderAPI::eOpenGL:
            MG_FATAL("Not impl for OpenGL yet.")
            break;
        case RenderAPI::eVulkan:
            this->backend.reset(get_imgui_backend<RenderAPI::eVulkan>());
            break;
        case RenderAPI::eDirectX:
            MG_FATAL("Not impl for DirectX yet.")
            break;
        case RenderAPI::eMetal:
            MG_FATAL("Not impl for Metal yet.")
            break;
        }
    }

    ImGuiRenderer::~ImGuiRenderer() {
        backend.reset();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiRenderer::begin_imgui() {
        backend->begin_imgui();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiRenderer::end_imgui() {
        ImGui::EndFrame();
        backend->end_imgui();

        // ImGuiIO& io = ImGui::GetIO();
        // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            // ImGui::UpdatePlatformWindows();
            // ImGui::RenderPlatformWindowsDefault();
        // }
    }

    ImTextureID ImGuiRenderer::get_scene_texture() {
        return backend->get_scene_texture();
    }
}
