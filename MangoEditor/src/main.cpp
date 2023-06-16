#include <MangoEngine/entry.hpp>
#include "imgui/backends/imgui_impl_vulkan.h"

namespace MangoEditor {
    class MangoEditorApplication final : public MangoEngine::Application {
    public:
        void generate_engine_config(MangoEngine::EngineConfig *engine_config) override {
            engine_config->api = MangoEngine::RenderAPI::eVulkan;
            engine_config->title = "Mango Engine Editor";
            engine_config->window_x = -1;
            engine_config->window_y = -1;
            engine_config->window_width = 1920;
            engine_config->window_height = 1080;
        }

        MangoEngine::Result initialize() override {
            MangoEngine::render_system->set_bg_color(1, 0.5, 0.9, 1);
            camera = MangoEngine::camera_system->create_orthographic_camera({ 0, 0, 1 }, { 1920.0f / 4.0f, 1080.0f / 4.0f }, 2);
            return MangoEngine::Result::eSuccess;
        }

        MangoEngine::Result on_update(MangoEngine::f32 dt) override {
            rotate += dt;
            return MangoEngine::Result::eSuccess;
        }

        MangoEngine::Result on_draw_frame() override {
            auto &command = MangoEngine::render_system->get_render_command();
            command.begin_scene(camera);

            command.draw_quad({ 0, 0 }, { 100, 100 }, rotate, { 0, 0.3, 0.99 });

            command.end_scene();
            return MangoEngine::Result::eSuccess;
        }

        MangoEngine::Result on_draw_imgui() override {
            ImGui::DockSpaceOverViewport();
            ImGui::Begin("Scene");
            auto extent = ImGui::GetContentRegionAvail();
            camera->set_extent(extent.x, extent.y);
            camera->update();
            ImGui::Image(MangoEngine::imgui_renderer->get_scene_texture(), extent);
            ImGui::End();
            return MangoEngine::Result::eSuccess;
        }

        MangoEngine::Result quit() override {
            return MangoEngine::Result::eSuccess;
        }

    private:
        std::shared_ptr<MangoEngine::OrthographicCamera> camera;
        float rotate = 0.0f;
    };
}

MangoEngine::Application *MangoEngine::create_application() {
    return new MangoEditor::MangoEditorApplication();
}