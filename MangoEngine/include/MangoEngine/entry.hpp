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

    MangoEngine::Result res;
    auto *application = MangoEngine::create_application();
    res = application->initialize();
    if (res != MangoEngine::Result::eSuccess) {
        MG_FATAL("Failed Initialize Application.")
        return -1;
    }

    MG_INFO("Running")
    for (MangoEngine::u32 i = 0; i < 100; i ++) {
        application->on_draw();
        application->on_update();
    }

    res = application->quit();
    if (res != MangoEngine::Result::eSuccess) {
        MG_FATAL("Failed Quit Application.")
        return -1;
    }

    MangoEngine::Logger::Quit();
}
