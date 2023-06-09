#include <MangoEngine/entry.hpp>

class TestApplication : public MangoEngine::Application {
public:
    MangoEngine::Result initialize() {
        MangoEngine::event_system->add_event_callback<MangoEngine::KeyPressedEvent>([&](auto event) {
            MG_INFO("Key Pressed {}", event.key)
            if (event.key == 52)
                MangoEngine::render_system->set_bg_color(0.5, 0.2, 0.9, a);
            else if (event.key == 53)
                MangoEngine::render_system->set_bg_color(0.2, 0.9, 0.5, a);
        });
        MangoEngine::render_system->set_bg_color(1.0, 1.0, 1.0, 1.0);
        return MangoEngine::Result::eSuccess;
    }

    MangoEngine::Result on_draw() {
        return MangoEngine::Result::eSuccess;
    }

    MangoEngine::Result on_update() {
        if (MangoEngine::input->is_key_down(50) == MangoEngine::MG_TRUE) {
            MG_INFO("Key 50 is downed.")
            MG_INFO("Current Mouse Pos is {} {}.", MangoEngine::input->get_mouse_x(), MangoEngine::input->get_mouse_pos().y)
        }
        return MangoEngine::Result::eSuccess;
    }

    MangoEngine::Result quit() {
        return MangoEngine::Result::eSuccess;
    }

private:
    float a = 1.0f;
};

namespace MangoEngine {
    void generate_engine_config(EngineConfig *engine_config) {
        engine_config->window_x = 0;
        engine_config->window_y = 0;
        engine_config->window_width = 640;
        engine_config->window_height = 640;
        engine_config->title = "Sandbox";
        engine_config->api = MangoEngine::RenderAPI::eVulkan;
    }

    Application *create_application() {
        return new TestApplication();
    }
}
