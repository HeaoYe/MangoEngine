#include "MangoEngine/core/logger.hpp"

namespace MangoEngine {
    ::std::unique_ptr<Logger> Logger::_instance;
    
    Logger &Logger::GetInstance() {
        return *_instance;
    }

    void Logger::Quit() {
        MG_INFO("Quit Logger Runtime System.")
        _instance.reset();
    }

    void Logger::Initialize(LogLevel level) {
        _instance.reset(new Logger(level));
        MG_INFO("Initialize Logger Runtime System.")
    }

    Logger::Logger(LogLevel level) {
        spd_logger = spdlog::stdout_color_mt("MangoEngine");
        set_level(level);
    }

    Logger::~Logger() {
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

    void Logger::set_level(LogLevel level) {
        spd_logger->set_level(level2spdlog(level));
    }
}
