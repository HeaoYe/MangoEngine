#pragma once

#include "../commons.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace MangoEngine {
    class Logger {
    public:
        void set_level(LogLevel level);

        template<typename... Args>
        void trace(Args &&... args) {
            spd_logger->trace(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void debug(Args &&... args) {
            spd_logger->debug(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void info(Args &&... args) {
            spd_logger->info(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void warn(Args &&... args) {
            spd_logger->warn(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void error(Args &&... args) {
            spd_logger->error(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void fatal(Args &&... args) {
            spd_logger->critical(std::forward<Args>(args)...);
        }
    private:
        std::shared_ptr<spdlog::logger> spd_logger;
    
    declare_runtime_system(Logger, LogLevel level)
    };
}

#define MG_TRACE(...) ::MangoEngine::Logger::GetInstance().trace(__VA_ARGS__);
#define MG_DEBUG(...) ::MangoEngine::Logger::GetInstance().debug(__VA_ARGS__);
#define MG_INFO(...) ::MangoEngine::Logger::GetInstance().info(__VA_ARGS__);
#define MG_WARN(...) ::MangoEngine::Logger::GetInstance().warn(__VA_ARGS__);
#define MG_ERROR(...) ::MangoEngine::Logger::GetInstance().error(__VA_ARGS__);
#define MG_FATAL(...) ::MangoEngine::Logger::GetInstance().fatal(__VA_ARGS__);
