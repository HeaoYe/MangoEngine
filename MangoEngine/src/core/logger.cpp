#include "MangoEngine/core/logger.hpp"
#include <MangoRHI/logger.hpp>

namespace MangoEngine {
    implement_runtime_system(LoggerSystem, logger_system)

    LoggerSystem::LoggerSystem() {
        spd_logger = spdlog::stdout_color_mt("MangoEngine");
    }

    LoggerSystem::~LoggerSystem() {
        spdlog::shutdown();
    }

    void LoggerSystem::set_level(LogLevel level) {
        spd_logger->set_level(transform<LogLevel, spdlog::level::level_enum>(level));
        MangoRHI::set_logger_level(transform<LogLevel, MangoRHI::LogLevel>(level));
    }
}
