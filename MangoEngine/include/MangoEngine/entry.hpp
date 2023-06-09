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

    MangoEngine::LoggerSystem::Initialize(level);
    MangoEngine::WindowSystem::Initialize();
    MangoEngine::EventSystem::Initialize();
    MangoEngine::InputSystem::Initialize();
    MangoEngine::RenderSystem::Initialize();

    MangoEngine::Result res;
    auto *application = MangoEngine::create_application();
    res = application->initialize();
    if (res != MangoEngine::Result::eSuccess) {
        MG_FATAL("Failed Initialize Application.")
        return -1;
    }

    while (MangoEngine::window->pull_events() != MangoEngine::MG_FALSE) {
        application->on_update();
        if (MangoEngine::render_system->begin_render() == MangoEngine::Result::eSuccess) {
            application->on_draw();
            MangoEngine::render_system->end_render();
        }
        MangoEngine::input->swap_state();
    }

    res = application->quit();
    if (res != MangoEngine::Result::eSuccess) {
        MG_FATAL("Failed Quit Application.")
        return -1;
    }

    MangoEngine::RenderSystem::Quit();
    MangoEngine::InputSystem::Quit();
    MangoEngine::EventSystem::Quit();
    MangoEngine::WindowSystem::Quit();
    MangoEngine::LoggerSystem::Quit();
}
