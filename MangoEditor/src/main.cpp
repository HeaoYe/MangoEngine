#include <MangoEngine/entry.hpp>
#include <imgui/imgui_internal.h>

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
            camera = MangoEngine::camera_system->create_orthographic_camera({ 0, 0, 1 }, { 1920.0f / 2.0f, 1080.0f / 2.0f }, 2);
            return MangoEngine::Result::eSuccess;
        }

        MangoEngine::Result on_update(MangoEngine::f32 dt) override {
            rotate += dt;
            MangoEngine::render_system->set_bg_color(
                static_cast<float>(MangoEngine::input_system->is_key_down(MangoEngine::Key::eQ)),
                static_cast<float>(MangoEngine::input_system->get_last_mouse_x()) / static_cast<float>(MangoEngine::window_system->get_width()),
                static_cast<float>(MangoEngine::input_system->get_last_mouse_y()) / static_cast<float>(MangoEngine::window_system->get_height()),
                1
            );
            return MangoEngine::Result::eSuccess;
        }

        MangoEngine::Result on_draw_frame() override {
            auto &command = MangoEngine::render_system->get_render_command();
            command.begin_scene(camera);

            command.draw_quad({ 0, 0 }, { 100, 100 }, rotate, { 0, 0.3, 0.99 });
            command.draw_quad({ 100, 200 }, { 100, 100 }, -rotate, { 0, 0.3, 0.99 });

            command.end_scene();
            return MangoEngine::Result::eSuccess;
        }

        MangoEngine::Result on_draw_imgui() override {
            auto *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGuiWindowFlags window_flags = 0;
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace", nullptr, window_flags);
            ImGui::PopStyleVar(3);
            auto dockspace = ImGui::GetID("DockSpace");
            ImGui::DockSpace(dockspace, { 0.0f, 0.0f }, 0, nullptr);

            auto &io = ImGui::GetIO();
            ImGui::Begin("Settings");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
            ImGui::Begin("Scene");
            auto extent = ImGui::GetContentRegionAvail();
            float expected_width = extent.y * camera->get_extent().x / camera->get_extent().y;
            float pad = (expected_width - extent.x) / expected_width / 2.0f;
            ImGui::Image(MangoEngine::imgui_renderer->get_scene_texture(), extent, { 0 + pad, 0 }, { 1 - pad, 1 });
            ImGui::End();
            ImGui::PopStyleVar();

            static bool startup = true;
            if (startup) {
                startup = false;
                ImGuiID left_id, right_id;
                ImGui::DockBuilderAddNode(dockspace, ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSplitNode(dockspace, ImGuiDir_Left, 1.0f, &left_id, &right_id);
                ImGui::DockBuilderDockWindow("Settings", left_id);
                ImGui::DockBuilderDockWindow("Scene", right_id);
                ImGui::DockBuilderFinish(dockspace);
            }

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
