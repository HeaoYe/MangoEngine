#include "MangoEngine/render/render.hpp"
#include "MangoEngine/core/event.hpp"
#include <MangoRHI/MangoRHI.hpp>

namespace MangoEngine {
    implement_runtime_system(RenderSystem, render_system)

    RenderSystem::RenderSystem() : context([]() -> MangoRHI::Context & {
        MangoRHI::initialize(transform<RenderAPI, MangoRHI::API>(engine_config->api));
        return MangoRHI::get_context();
    }()) {
        switch (engine_config->api) {
        case RenderAPI::eNone:
            MG_FATAL("Not impl for None yet.")
            break;
        case RenderAPI::eOpenGL:
            MG_FATAL("Not impl for OpenGL yet.")
            break;
        case RenderAPI::eVulkan:
            context.set_api_info(get_context_info<RenderAPI::eVulkan>());
            break;
        case RenderAPI::eDirectX:
            MG_FATAL("Not impl for DirectX yet.")
            break;
        case RenderAPI::eMetal:
            MG_FATAL("Not impl for Metal yet.")
            break;
        }
        context.set_vsync_enabled(MangoRHI::MG_FALSE);
        context.set_swapchain_image_count(3);
        context.set_max_in_flight_frame_count(2);
        context.set_multisample_count(MangoRHI::MultisampleCount::e1);

        auto &render_pass = context.get_render_pass_reference();
        render_pass.add_output_render_target(MANGORHI_SURFACE_RENDER_TARGET_NAME, MangoRHI::RenderTargetLayout::eColor, {
            .src_color_factor = MangoRHI::BlendFactor::eSrcAlpha,
            .dst_color_factor = MangoRHI::BlendFactor::eOneMinusSrcAlpha,
            .color_op = MangoRHI::BlendOp::eAdd,
            .src_alpha_factor = MangoRHI::BlendFactor::eZero,
            .dst_alpha_factor = MangoRHI::BlendFactor::eOne,
            .alpha_op = MangoRHI::BlendOp::eAdd
        });
        render_pass.add_subpass("main", MangoRHI::PipelineBindPoint::eGraphicsPipeline);
        render_pass.add_output_render_target(MANGORHI_SURFACE_RENDER_TARGET_NAME, MangoRHI::RenderTargetLayout::eColor);
        render_pass.add_subpass("imgui", MangoRHI::PipelineBindPoint::eGraphicsPipeline);
        render_pass.add_dependency({
            .name = "main",
            .stage = MangoRHI::PipelineStage::eColorOutput,
            .access = MangoRHI::Access::eColorRenderTargetWrite,
        }, {
            .name = "imgui",
            .stage = MangoRHI::PipelineStage::eColorOutput,
            .access = MangoRHI::Access::eColorRenderTargetWrite,
        });

        context.create();
        auto &resource_factory = context.get_resource_factory_reference();

        quad_shader_program.reset(resource_factory.create_shader_program("main").release());
        quad_shader_program->add_vertex_attribute(MangoRHI::VertexInputType::eFloat3, sizeof(glm::vec3));
        quad_shader_program->add_vertex_attribute(MangoRHI::VertexInputType::eFloat2, sizeof(glm::vec2));
        quad_shader_program->add_vertex_attribute(MangoRHI::VertexInputType::eFloat4, sizeof(glm::vec4));
        quad_shader_program->add_vertex_attribute(MangoRHI::VertexInputType::eInt, sizeof(i32));
        quad_shader_program->add_vertex_binding(MangoRHI::VertexInputRate::ePerInstance);
        auto builtin_quad_vert_shader = resource_factory.create_shader("assets/shaders/builtin-quad_shader.vert.spv");
        auto builtin_quad_frag_shader = resource_factory.create_shader("assets/shaders/builtin-quad_shader.frag.spv");
        quad_shader_program->attach_vertex_shader(*builtin_quad_vert_shader, "main");
        quad_shader_program->attach_fragment_shader(*builtin_quad_frag_shader, "main");
        quad_shader_program->set_cull_mode(MangoRHI::CullMode::eNone);
        descriptor_set = quad_shader_program->create_descriptor_set();
        auto &empty_texture = *resource_factory.create_empty_texture().release();
        descriptor_set.lock()->add_textures_descriptor(MangoRHI::DescriptorStage::eFragment, {
            empty_texture, empty_texture, empty_texture, empty_texture, empty_texture, empty_texture,
            empty_texture, empty_texture, empty_texture, empty_texture, empty_texture, empty_texture,
            empty_texture, empty_texture, empty_texture, empty_texture, empty_texture, empty_texture,
            empty_texture, empty_texture, empty_texture, empty_texture, empty_texture, empty_texture,
            empty_texture, empty_texture, empty_texture, empty_texture, empty_texture, empty_texture,
            empty_texture, empty_texture
        });
        empty_texture.destroy_before(quad_shader_program.get());
        quad_shader_program->create();

        render_command.reset(new RenderCommand(*this));
        viewport = { 0, 0, 0, 0, 0, 1 };
        event_system->add_event_callback<WindowResizedEvent>([&](auto &event) {
            this->viewport.width = static_cast<f32>(event.width);
            this->viewport.height = static_cast<f32>(event.height);
        });
    }

    RenderSystem::~RenderSystem() {
        render_command.reset();
        quad_shader_program.reset();
        MangoRHI::quit();
    }

    void RenderSystem::set_bg_color(f32 r, f32 g, f32 b, f32 a) {
        context.set_clear_value(MANGORHI_SURFACE_RENDER_TARGET_NAME, { .color = { r, g, b, a } });
    }

    Result RenderSystem::begin_render() {
        if (context.begin_frame() != MangoRHI::Result::eSuccess) {
            return Result::eFailed;
        }
        auto &command = context.get_current_command_reference();
        command.bind_shader_program(*quad_shader_program);
        command.set_viewport(viewport);
        command.set_scissor({ 0, 0, static_cast<u32>(viewport.width), static_cast<u32>(viewport.height) });
        render_command->render(context.get_current_command_reference());
        return Result::eSuccess;
    }

    Result RenderSystem::end_render() {
        if (context.end_frame() != MangoRHI::Result::eSuccess) {
            return Result::eFailed;
        }
        return Result::eSuccess;
    }

    MangoRHI::Context &RenderSystem::get_context() {
        return context;
    }
}
