#pragma once

#include "commons.hpp"
#include "MangoEngine.hpp"

int main() {
    #if defined (MANGO_DEBUG)
    auto level = MangoEngine::LogLevel::eDebug;
    #else
    auto level = MangoEngine::LogLevel::eInfo;
    #endif

    MangoEngine::Logger::Initialize(level);
    MangoEngine::Window::Initialize();
    MangoEngine::EventSystem::Initialize();
    MangoEngine::Input::Initialize();

    MangoEngine::Result res;
    auto *application = MangoEngine::create_application();
    res = application->initialize();
    if (res != MangoEngine::Result::eSuccess) {
        MG_FATAL("Failed Initialize Application.")
        return -1;
    }

    while (MangoEngine::Window::GetInstance().pull_events() != MangoEngine::MG_FALSE) {
        application->on_draw();
        application->on_update();
    }

    res = application->quit();
    if (res != MangoEngine::Result::eSuccess) {
        MG_FATAL("Failed Quit Application.")
        return -1;
    }

    MangoEngine::Input::Quit();
    MangoEngine::EventSystem::Quit();
    MangoEngine::Window::Quit();
    MangoEngine::Logger::Quit();
}
