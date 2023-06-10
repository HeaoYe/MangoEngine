#include <MangoEngine/entry.hpp>

class TestApplication final : public MangoEngine::Application {
public:
    MangoEngine::Result initialize() override {
        MangoEngine::event_system->add_event_callback<MangoEngine::KeyPressedEvent>([&](auto event) {
            MG_INFO("Key Pressed: {}", MangoEngine::to_string(event.key))
        });
        camera = &MangoEngine::camera_system->create_orthographic_camera({ 0, 0, 1 }, { 640, 640 }, 2);
        return MangoEngine::Result::eSuccess;
    }

    MangoEngine::Result on_draw_frame() override {
        auto &command = MangoEngine::render_system->get_render_command();
        camera->bind();
        command.draw_quad({ MangoEngine::input_system->get_last_mouse_x() - 320, 320 - MangoEngine::input_system->get_last_mouse_y(), -0.000001 }, 320, { 1.0, 0.8, 0.4, 0.5f });

        command.draw_quad({ 0, 0 }, { 320, 320 }, { 0.05, 0.9 , 0.99, 0.5f });
        return MangoEngine::Result::eSuccess;
    }

    MangoEngine::Result on_draw_imgui() override {
        ImGuiIO &io = ImGui::GetIO();
        static int counter = 0;
        static float clear_color[3];
        static char *buffer = new char[100]();
        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::SliderFloat("float", &zoom, 0.1f, 2.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);
        MangoEngine::render_system->set_bg_color(clear_color[0], clear_color[1], clear_color[2], 0.0f);
        if (ImGui::Button("Button"))
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::InputText(u8"中文", buffer, 100);
        ImGui::End();

        return MangoEngine::Result::eSuccess;
    }

    MangoEngine::Result on_update(MangoEngine::f32 dt) override {
        if (MangoEngine::input_system->is_key_down(MangoEngine::Key::eQ) == MangoEngine::MG_TRUE) {
            MG_INFO("Key Q is downed.")
            MG_INFO("Current Delta Time is {}.", dt)
            MG_INFO("Current Mouse Pos is {} {}.", MangoEngine::input_system->get_mouse_x(), MangoEngine::input_system->get_mouse_pos().y)
        }
        if (MangoEngine::input_system->is_key_down(MangoEngine::Key::eW) == MangoEngine::MG_TRUE) {
            camera->pos.y += 150.0f * dt;
        }
        if (MangoEngine::input_system->is_key_down(MangoEngine::Key::eS) == MangoEngine::MG_TRUE) {
            camera->pos.y -= 150.0f * dt;
        }
        if (MangoEngine::input_system->is_key_down(MangoEngine::Key::eA) == MangoEngine::MG_TRUE) {
            camera->pos.x -= 150.0f * dt;
        }
        if (MangoEngine::input_system->is_key_down(MangoEngine::Key::eD) == MangoEngine::MG_TRUE) {
            camera->pos.x += 150.0f * dt;
        }
        camera->zoom = zoom;
        camera->update();
        return MangoEngine::Result::eSuccess;
    }

    MangoEngine::Result quit() override {
        return MangoEngine::Result::eSuccess;
    }

private:
    float zoom = 1.0f;
    MangoEngine::Camera *camera;
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
