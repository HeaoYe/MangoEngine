#include "MangoEngine/core/input.hpp"

namespace MangoEngine {
    implement_runtime_system(Input)
        _instance.reset(new Input());
    }

    Input::Input() {}
    Input::~Input() {}
}
