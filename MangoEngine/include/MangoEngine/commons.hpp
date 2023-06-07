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

#if defined (NDEBUG)
    #define MANGO_DEBUG

    #define MANGO_ASSERT(expr) \
    if(!(expr)) { \
        MG_FATAL("Mango Assert Failed {} -> {}", __FILE__, __LINE__) \
        throw ::std::runtime_error("Mango Assert Failed"); \
    }
#else
    #define MANGO_RELEASE

    #define MANGO_ASSERT(expr)
#endif

#include <MangoRHI/commons.hpp>

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
    constexpr Bool MG_FALSE = 0;
    constexpr Bool MG_TRUE = 1;
    #define BIT(x) (1 << x)
    
    #define declare_runtime_system(cls_name, ...) \
    public: \
        cls_name(__VA_ARGS__); \
        ~cls_name(); \
        static void Initialize(__VA_ARGS__); \
        static void Quit(); \
        static cls_name &GetInstance(); \
    private: \
        static ::std::unique_ptr<cls_name> _instance; \
        cls_name(const cls_name &) = delete; \
        cls_name(cls_name &&) = delete; \
        cls_name &operator=(const cls_name &) = delete; \
        cls_name &operator=(cls_name &&) = delete;
    
    #define implement_runtime_system(cls_name, ...) \
    ::std::unique_ptr<cls_name> cls_name::_instance; \
    cls_name &cls_name::GetInstance() { \
        return *_instance; \
    } \
    void cls_name::Quit() { \
        MG_INFO("Quit {} Runtime System.", #cls_name) \
        _instance.reset(); \
    } \
    void cls_name::Initialize(__VA_ARGS__) { \
        MG_INFO("Initialize {} Runtime System.", #cls_name)

    enum class Result {
        eSuccess,
        eFailed,
    };

    enum class LogLevel : u32 {
        eTrace,
        eDebug,
        eInfo,
        eWarn,
        eError,
        eFatal,
    };
}
