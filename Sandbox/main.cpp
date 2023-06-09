#include <MangoEngine/entry.hpp>

class TestApplication final : public MangoEngine::Application {
public:
    MangoEngine::Result initialize() override {
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

    MangoEngine::Result on_draw_frame() override {
        return MangoEngine::Result::eSuccess;
    }

    MangoEngine::Result on_draw_imgui() override {
        ImGuiIO &io = ImGui::GetIO();
        static float alpha = 0.5f;
        static int counter = 0;
        static float clear_color[3];
        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::SliderFloat("float", &alpha, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);
        MangoEngine::render_system->set_bg_color(clear_color[0], clear_color[1], clear_color[2], alpha);
        if (ImGui::Button("Button"))
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        return MangoEngine::Result::eSuccess;
    }

    MangoEngine::Result on_update() override {
        if (MangoEngine::input->is_key_down(50) == MangoEngine::MG_TRUE) {
            MG_INFO("Key 50 is downed.")
            MG_INFO("Current Mouse Pos is {} {}.", MangoEngine::input->get_mouse_x(), MangoEngine::input->get_mouse_pos().y)
        }
        return MangoEngine::Result::eSuccess;
    }

    MangoEngine::Result quit() override {
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
