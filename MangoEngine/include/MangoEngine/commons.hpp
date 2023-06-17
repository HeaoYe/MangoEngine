#pragma once

#if defined (__WIN32)
    #define MANGO_PLATFORM_WINDOWS
    #error Not impl for windows.
#elif defined (__unix__)
    #define MANGO_PLATFORM_UNIX
    #define MANGO_API
#elif defined (__APPLE__)
    #define MANGO_PLATFORM_APPLE
    #error Not impl for mac and ios.
#elif defined (__ANDROID__)
    #define MANGO_PLATFORM_ANDROID
    #error Not impl for android
#else
    #error Unknown Platform.
#endif

#if defined (MANGO_DEBUG)
    #define MANGO_ASSERT(expr) \
    if(!(expr)) { \
        MG_FATAL("Mango Assert Failed {} -> {}", __FILE__, __LINE__) \
        throw ::std::runtime_error("Mango Assert Failed"); \
    }
#else
    #define MANGO_RELEASE
    #define MANGO_ASSERT(expr)
#endif

#include <cstdint>
#include <sstream>

#include <MangoRHI/MangoRHI.hpp>
#include <glm/glm.hpp>

namespace MangoEngine {
    typedef int8_t i8;
    typedef int16_t i16;
    typedef int32_t i32;
    typedef int64_t i64;
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;
    typedef float f32;
    typedef double d32;
    typedef u8 Bool;
    typedef u8 ButtonState;
    constexpr Bool MG_FALSE = 0;
    constexpr Bool MG_TRUE = 1;
    constexpr ButtonState MG_UP = 0;
    constexpr ButtonState MG_DOWN = 1;
    #define BIT(x) (1 << x)

    #define no_copy_and_move_construction(cls_name) \
    private: \
        cls_name(const cls_name &) = delete; \
        cls_name(cls_name &&) = delete; \
        cls_name &operator=(const cls_name &) = delete; \
        cls_name &operator=(cls_name &&) = delete;

    #define declare_runtime_system(cls_name) \
    public: \
        cls_name(); \
        virtual ~cls_name(); \
        static void Initialize(); \
        static void Quit(); \
        static cls_name &GetInstance(); \
    private: \
        static ::std::unique_ptr<cls_name> _instance; \
    no_copy_and_move_construction(cls_name)

    #define declare_runtime_system_alias(cls_name, alias) \
    extern cls_name *alias;

    #define implement_runtime_system(cls_name, alias) \
    ::std::unique_ptr<cls_name> cls_name::_instance; \
    cls_name *alias = nullptr; \
    cls_name &cls_name::GetInstance() { \
        return *_instance; \
    } \
    void cls_name::Quit() { \
        MG_INFO("Quit {} Runtime System.", #cls_name) \
        _instance.reset(); \
        alias = nullptr; \
    } \
    void cls_name::Initialize() { \
        _instance = std::make_unique<cls_name>(); \
        alias = _instance.get(); \
        MG_INFO("Initialize {} Runtime System.", #cls_name) \
    }

    enum class Result {
        eSuccess,
        eFailed,
    };

    enum class RenderAPI {
        eNone,
        eOpenGL,
        eVulkan,
        eDirectX,
        eMetal,
    };

    enum class LogLevel : u32 {
        eTrace,
        eDebug,
        eInfo,
        eWarn,
        eError,
        eFatal,
    };

    struct EngineConfig {
        u32 window_x = 0, window_y = 0;
        u32 window_width = 320, window_height = 320;
        const char *title = "";
        RenderAPI api = RenderAPI::eNone;
    };
    extern EngineConfig *engine_config;

    template<typename src, typename dst>
    dst transform(src data);
}

#include "core/logger.hpp"
