#pragma once

#if defined (__WIN32)
    #define MANGO_PLATFORM_WINDOWS
    #error Not impl for windows.
#elif defined (__unix__)
    #define MANGO_PLATFORM_UNIX
    #define MANGO_API
    #define MANGO_USE_XCB_WINDOW
    // #define MANGO_USE_WAYLAND_WINDOW
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

    template<typename T>
    struct Reference {
    private:
        T *data;
    public:
        Reference(const Reference &other) : data(other.data) {};
        Reference &operator=(const Reference &) = default;
        Reference(Reference &&other) : data(other.data) { other.data = nullptr; };
        Reference &operator=(Reference &&) = default;
        Reference() : data(nullptr) {}
        Reference(T &data_) : data(&data_) {}
        void clear() { data = nullptr; }
        void set(T &data_) { data = &data_; }
        operator const T&() const { return *data; }
        const T &get() const { return *data; }
        const T *operator->() const { return data; }
        operator T&() { return *data; }
        T &get() { return *data; }
        T *operator->() { return data; }
    };

    #define no_copy_and_move_construction(cls_name) \
    private: \
        cls_name(const cls_name &) = delete; \
        cls_name(cls_name &&) = delete; \
        cls_name &operator=(const cls_name &) = delete; \
        cls_name &operator=(cls_name &&) = delete;

    #define declare_runtime_system(cls_name, ...) \
    public: \
        cls_name(__VA_ARGS__); \
        virtual ~cls_name(); \
        static void Initialize(__VA_ARGS__); \
        static void Quit(); \
        static cls_name &GetInstance(); \
    private: \
        static ::std::unique_ptr<cls_name> _instance; \
    no_copy_and_move_construction(cls_name)

    #define declare_runtime_system_alias(cls_name, alias) \
    extern Reference<cls_name> alias;

    #define implement_runtime_system_start(cls_name, alias, ...) \
    ::std::unique_ptr<cls_name> cls_name::_instance; \
    cls_name &cls_name::GetInstance() { \
        return *_instance; \
    } \
    void cls_name::Quit() { \
        _instance.reset(); \
        alias.clear(); \
        MG_INFO("Quit {} Runtime System.", #cls_name) \
    } \
    void cls_name::Initialize(__VA_ARGS__) {

    #define implement_runtime_system_end(cls_name, alias) \
        alias.set(*_instance); \
        MG_INFO("Initialize {} Runtime System.", #cls_name) \
    } \
    Reference<cls_name> alias {};

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
    extern void generate_engine_config(EngineConfig *engine_config);
    extern EngineConfig *engine_config;

    struct Pos {
        u32 x;
        u32 y;
    };
}

#include "core/logger.hpp"
