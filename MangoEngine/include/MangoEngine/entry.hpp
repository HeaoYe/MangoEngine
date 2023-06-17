#pragma once

#include "commons.hpp"
#include "MangoEngine.hpp"

int main() {
    auto *application = MangoEngine::create_application();

    MangoEngine::engine_config = new MangoEngine::EngineConfig();
    application->generate_engine_config(MangoEngine::engine_config);

    #if defined (MANGO_DEBUG)
    auto level = MangoEngine::LogLevel::eDebug;
    #else
    auto level = MangoEngine::LogLevel::eInfo;
    #endif

    MangoEngine::LoggerSystem::Initialize();
    MangoEngine::logger_system->set_level(level);
    MangoEngine::EventSystem::Initialize();
    MangoEngine::InputSystem::Initialize();
    MangoEngine::WindowSystem::Initialize();
    MangoEngine::RenderSystem::Initialize();
    MangoEngine::CameraSystem::Initialize();
    MangoEngine::ImGuiRenderer::Initialize();

    MangoEngine::Result res;
    res = application->initialize();
    if (res != MangoEngine::Result::eSuccess) {
        MG_FATAL("Failed Initialize Application.")
        return -1;
    }

    MangoEngine::event_system->dispatch<MangoEngine::WindowResizedEvent>({
        .width = MangoEngine::window_system->get_width(),
        .height = MangoEngine::window_system->get_height(),
    });

    while (MangoEngine::window_system->pull_events() != MangoEngine::MG_FALSE) {
        application->on_update(ImGui::GetIO().DeltaTime);
        if (MangoEngine::render_system->acquire() == MangoEngine::Result::eSuccess) {
            MangoEngine::render_system->begin_render();
            application->on_draw_frame();
            MangoEngine::render_system->end_render();

            MangoEngine::imgui_renderer->begin_imgui();
            application->on_draw_imgui();
            MangoEngine::imgui_renderer->end_imgui();

            MangoEngine::render_system->present();
        }
        MangoEngine::input_system->swap_state();
    }

    res = application->quit();
    if (res != MangoEngine::Result::eSuccess) {
        MG_FATAL("Failed Quit Application.")
        return -1;
    }

    MangoEngine::ImGuiRenderer::Quit();
    MangoEngine::CameraSystem::Quit();
    MangoEngine::RenderSystem::Quit();
    MangoEngine::WindowSystem::Quit();
    MangoEngine::InputSystem::Quit();
    MangoEngine::EventSystem::Quit();
    MangoEngine::LoggerSystem::Quit();
}
