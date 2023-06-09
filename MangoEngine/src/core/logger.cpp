#include "MangoEngine/core/logger.hpp"
#include <MangoRHI/logger.hpp>

namespace MangoEngine {
    implement_runtime_system_start(LoggerSystem, logger)
        _instance.reset(new LoggerSystem());
    implement_runtime_system_end(LoggerSystem, logger)

    LoggerSystem::LoggerSystem() {
        spd_logger = spdlog::stdout_color_mt("MangoEngine");
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

    MangoRHI::LogLevel level2level(LogLevel level) {
        switch (level) {
        case LogLevel::eTrace:
            return MangoRHI::LogLevel::eTrace;
        case LogLevel::eDebug:
            return MangoRHI::LogLevel::eDebug;
        case LogLevel::eInfo:
            return MangoRHI::LogLevel::eInfo;
        case LogLevel::eWarn:
            return MangoRHI::LogLevel::eWarn;
        case LogLevel::eError:
            return MangoRHI::LogLevel::eError;
        case LogLevel::eFatal:
            return MangoRHI::LogLevel::eFatal;
        }
    }

    void LoggerSystem::set_level(LogLevel level) {
        spd_logger->set_level(level2spdlog(level));
        MangoRHI::set_logger_level(level2level(level));
    }
}
