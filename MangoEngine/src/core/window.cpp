#include "MangoEngine/core/window.hpp"

namespace MangoEngine {
    WindowSystem::WindowSystem() = default;

    WindowSystem::~WindowSystem() = default;

    Bool WindowSystem::pull_events() {
        return MG_FALSE;
    }
}
