#include "MangoEngine/core/window.hpp"

namespace MangoEngine {
    WindowSystem::WindowSystem() {
        x = engine_config->window_x;
        y = engine_config->window_y;
        width = engine_config->window_width;
        height = engine_config->window_height;
        title = engine_config->title;
    }

    WindowSystem::~WindowSystem() = default;

    Bool WindowSystem::pull_events() {
        return MG_FALSE;
    }
}
