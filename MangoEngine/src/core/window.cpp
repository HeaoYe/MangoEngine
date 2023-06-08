#include "MangoEngine/core/window.hpp"

namespace MangoEngine {
    Window::Window() = default;
    Window::~Window() = default;
    Bool Window::pull_events() {
        return MG_TRUE;
    }
}
