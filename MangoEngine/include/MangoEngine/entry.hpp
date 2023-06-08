#pragma once

#include "commons.hpp"
#include "MangoEngine.hpp"

int main() {
    #if defined (MANGO_DEBUG)
    auto level = MangoEngine::LogLevel::eDebug;
    #else
    auto level = MangoEngine::LogLevel::eInfo;
    #endif

    MangoEngine::LoggerSystem::Initialize(level);
    MangoEngine::WindowSystem::Initialize();
    MangoEngine::EventSystem::Initialize();
    MangoEngine::InputSystem::Initialize();

    MangoEngine::Result res;
    auto *application = MangoEngine::create_application();
    res = application->initialize();
    if (res != MangoEngine::Result::eSuccess) {
        MG_FATAL("Failed Initialize Application.")
        return -1;
    }

    while (MangoEngine::window->pull_events() != MangoEngine::MG_FALSE) {
        application->on_draw();
        application->on_update();
        MangoEngine::input->swap_state();
    }

    res = application->quit();
    if (res != MangoEngine::Result::eSuccess) {
        MG_FATAL("Failed Quit Application.")
        return -1;
    }

    MangoEngine::InputSystem::Quit();
    MangoEngine::EventSystem::Quit();
    MangoEngine::WindowSystem::Quit();
    MangoEngine::LoggerSystem::Quit();
}
