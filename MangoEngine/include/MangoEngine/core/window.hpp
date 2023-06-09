#pragma once

#include "../commons.hpp"
#include <GLFW/glfw3.h>

namespace MangoEngine {
    class WindowSystem;

    struct UserPointer {
    };

    class WindowSystem {
    public:
        Bool pull_events();
        GLFWwindow *get_glfw_window() { return glfw_window; }

    private:
        u32 x, y;
        u32 width, height;
        const char *title;
        GLFWwindow *glfw_window;
        std::unique_ptr<UserPointer> user_pointer;

    declare_runtime_system(WindowSystem)
    };
    declare_runtime_system_alias(WindowSystem, window_system)
}
