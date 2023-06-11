#include <MangoEngine/entry.hpp>
#include <list>

struct Quad {
    glm::vec2 root;
    glm::vec2 dir;
    float random;
    float sign;
    float dt;
    Quad(glm::vec2 root, glm::vec2 dir, float random, float sign, float dt) : root(root), dir(dir), random(random), sign(sign), dt(dt) {}
};

bool operator==(const Quad &other1, const Quad &other2) {
    return other1.random == other2.random && other1.dt == other2.dt;
};
constexpr float quad_duration = 0.75f;
constexpr double pi = 3.14159265358979323846;
int quad_per_second = 350;
constexpr float max_quad_size = 200.0f;
constexpr uint32_t window_width = 1920;
constexpr uint32_t window_height = 1080;

class QuadManager {
public:
    QuadManager() {
        texture = MangoEngine::Texture::load_from_file("assets/textures/dhl.png");
    }

    void generate() {
        if (total < 1.0f / static_cast<float>(quad_per_second))
            return;
        total -= 1.0f / static_cast<float>(quad_per_second);
        if (total > 2.0f / static_cast<float>(quad_per_second))
            total = 0.0f;
        double n = static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * 2.0 * pi;
        length += 1;
        quads.emplace_back(
            glm::vec2(MangoEngine::input_system->get_last_mouse_x() - static_cast<int>(window_width / 2), static_cast<int>(window_height / 2) - MangoEngine::input_system->get_last_mouse_y()),
            glm::vec2(cos(n), sin(n)),
            static_cast<float>(static_cast<double>(rand()) / static_cast<double>(RAND_MAX)),
            (static_cast<float>(rand() % 2) - 0.5f) * 2.0f,
            0.0f
        );
    }

    void draw() {
        std::for_each(quads.begin(), quads.end(), [this](auto &quad) {
            auto &command = MangoEngine::render_system->get_render_command();
            command.draw_quad(
                quad.root + max_quad_size * 2.0f * quad.dir * quad.dt * (quad.random + 0.5f),
                (quad_duration - quad.dt) / quad_duration * max_quad_size * (quad.random * 0.5f + 0.75f),
                (quad.random + (quad.random / 2.0f + 0.75f) * 2.0f * quad.dt * quad.sign) * 2.0f * static_cast<float>(pi),
                // { 1.0f, 0.7f, 0.0f, (quad_duration - quad.dt) / quad_duration * 0.5f + 0.5f },
                { 1.0f, 1.0f, 1.0f, (quad_duration - quad.dt) / quad_duration * 0.5f + 0.5f },
                texture
            );
        });
    }

    void update(float dt) {
        quads.remove_if([&](auto &quad) {
            if ((quad.dt += dt) > quad_duration && (length -= 1) > -1) {
                return true;
            }
            return false;
        });
        total += dt;
    }

    int length = 0;
private:
    std::shared_ptr<MangoEngine::Texture> texture;
    std::list<Quad> quads;
    float total = 0;
};

class TestApplication final : public MangoEngine::Application {
public:
    MangoEngine::Result initialize() override {
        MangoEngine::event_system->add_event_callback<MangoEngine::KeyPressedEvent>([&](auto event) {
            MG_INFO("Key Pressed: {}", MangoEngine::to_string(event.key))
        });
        camera = &MangoEngine::camera_system->create_orthographic_camera({ 0, 0, 1 }, { window_width, window_height }, 2);
        texture = MangoEngine::Texture::load_from_file("assets/textures/dance.png");
        return MangoEngine::Result::eSuccess;
    }

    MangoEngine::Result on_draw_frame() override {
        auto &command = MangoEngine::render_system->get_render_command();
        camera->bind();
        command.draw_quad({ 0, 0, -0.1 }, { 240, 240 }, rotate, { 0.05, 0.9 , 0.99, 0.5f });
        command.draw_quad({ 0, 0, -0.1 }, { 320, 320 }, rotate, { 1.0f, 1.0f, 1.0f, 1.0f }, texture);
        command.draw_quad({ 320, 0, -0.1 }, { 320, 320 }, -rotate, { 1.0f, 1.0f, 1.0f, 1.0f }, texture);
        command.draw_quad({ -320, 0, -0.1 }, { 320, 320 }, -rotate - pi, { 1.0f, 1.0f, 1.0f, 1.0f }, texture);
        quad_manager.draw();
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
        ImGui::SliderInt("Quad Generate Rate 10-500", &quad_per_second, 10, 500);
        ImGui::SliderInt("Quad Generate Rate 500-2000", &quad_per_second, 500, 2000);
        ImGui::Text("Current Quad Number: %d", quad_manager.length);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);
        MangoEngine::render_system->set_bg_color(clear_color[0], clear_color[1], clear_color[2], 0.0f);
        if (ImGui::Button("Button"))
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::InputText("中文", buffer, 100);
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
        if (MangoEngine::input_system->is_mouse_button_down(MangoEngine::MouseButton::eRight)) {
            quad_manager.generate();
        }
        rotate += 3.14 * dt;
        quad_manager.update(dt);
        camera->zoom = zoom;
        camera->update();
        return MangoEngine::Result::eSuccess;
    }

    MangoEngine::Result quit() override {
        return MangoEngine::Result::eSuccess;
    }

private:
    float zoom = 1.0f;
    float rotate = 0.0f;
    MangoEngine::Camera *camera;
    QuadManager quad_manager;
    std::shared_ptr<MangoEngine::Texture> texture;
};

namespace MangoEngine {
    void generate_engine_config(EngineConfig *engine_config) {
        engine_config->window_x = 0;
        engine_config->window_y = 0;
        engine_config->window_width = window_width;
        engine_config->window_height = window_height;
        engine_config->title = "Sandbox";
        engine_config->api = MangoEngine::RenderAPI::eVulkan;
    }

    Application *create_application() {
        return new TestApplication();
    }
}
