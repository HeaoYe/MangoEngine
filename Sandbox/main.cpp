#include <MangoEngine/entry.hpp>

class TestApplication : public MangoEngine::Application {
    MangoEngine::Result initialize() {
        MangoEngine::EventSystem::GetInstance().add_event_callback<MangoEngine::KeyPressedEvent>([](MangoEngine::KeyPressedEvent event) {
            MG_INFO("Key Pressed {}", event.keycode)
        });
        return MangoEngine::Result::eSuccess;
    }

    MangoEngine::Result on_draw() {
        MG_DEBUG("On Draw")
        return MangoEngine::Result::eSuccess;
    }

    MangoEngine::Result on_update() {
        MG_DEBUG("On Update")
        return MangoEngine::Result::eSuccess;
    }

    MangoEngine::Result quit() {
        return MangoEngine::Result::eSuccess;
    }
};

namespace MangoEngine {
    Application *create_application() {
        return new TestApplication();
    }
}
