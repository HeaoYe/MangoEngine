#pragma once

#include "commons.hpp"
#include "MangoEngine.hpp"

int main() {
    #if defined (MANGO_DEBUG)
    auto level = MangoEngine::LogLevel::eDebug;
    #else
    auto level = MangoEngine::LogLevel::eInfo;
    #endif

    MangoEngine::engine_config = new MangoEngine::EngineConfig();
    MangoEngine::generate_engine_config(MangoEngine::engine_config);

    MangoEngine::LoggerSystem::Initialize();
    MangoEngine::logger_system->set_level(level);
    MangoEngine::EventSystem::Initialize();
    MangoEngine::InputSystem::Initialize();
    MangoEngine::WindowSystem::Initialize();
    MangoEngine::RenderSystem::Initialize();
    MangoEngine::ImGuiRenderer::Initialize();

    MangoEngine::Result res;
    auto *application = MangoEngine::create_application();
    res = application->initialize();
    if (res != MangoEngine::Result::eSuccess) {
        MG_FATAL("Failed Initialize Application.")
        return -1;
    }

    while (MangoEngine::window_system->pull_events() != MangoEngine::MG_FALSE) {
        application->on_update();
        if (MangoEngine::render_system->begin_render() == MangoEngine::Result::eSuccess) {
            application->on_draw_frame();
            MangoEngine::imgui_renderer->begin_imgui();
            application->on_draw_imgui();
            MangoEngine::imgui_renderer->end_imgui();
            MangoEngine::render_system->end_render();
        }
        MangoEngine::input_system->swap_state();
    }

    res = application->quit();
    if (res != MangoEngine::Result::eSuccess) {
        MG_FATAL("Failed Quit Application.")
        return -1;
    }

    MangoEngine::ImGuiRenderer::Quit();
    MangoEngine::RenderSystem::Quit();
    MangoEngine::WindowSystem::Quit();
    MangoEngine::InputSystem::Quit();
    MangoEngine::EventSystem::Quit();
    MangoEngine::LoggerSystem::Quit();
}
