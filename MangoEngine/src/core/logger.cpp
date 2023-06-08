#include "MangoEngine/core/logger.hpp"

namespace MangoEngine {
    ::std::unique_ptr<LoggerSystem> LoggerSystem::_instance;

    LoggerSystem &LoggerSystem::GetInstance() {
        return *_instance;
    }

    void LoggerSystem::Quit() {
        MG_INFO("Quit Logger Runtime System.")
        _instance.reset();
    }

    void LoggerSystem::Initialize(LogLevel level) {
        logger = new LoggerSystem(level);
        _instance.reset(logger);
        MG_INFO("Initialize Logger Runtime System.")
    }

    LoggerSystem::LoggerSystem(LogLevel level) {
        spd_logger = spdlog::stdout_color_mt("MangoEngine");
        set_level(level);
    }

    LoggerSystem::~LoggerSystem() {
        spdlog::shutdown();
    }

    spdlog::level::level_enum level2spdlog(LogLevel level) {
        switch (level) {
        case LogLevel::eTrace:
            return spdlog::level::trace;
        case LogLevel::eDebug:
            return spdlog::level::debug;
        case LogLevel::eInfo:
            return spdlog::level::info;
        case LogLevel::eWarn:
            return spdlog::level::warn;
        case LogLevel::eError:
            return spdlog::level::err;
        case LogLevel::eFatal:
            return spdlog::level::critical;
        }
    }

    void LoggerSystem::set_level(LogLevel level) {
        spd_logger->set_level(level2spdlog(level));
    }

    LoggerSystem *logger;
}
