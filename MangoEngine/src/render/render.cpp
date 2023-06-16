#include "MangoEngine/render/render.hpp"
#include "MangoEngine/core/event.hpp"
#include <MangoRHI/MangoRHI.hpp>

namespace MangoEngine {
    implement_runtime_system(RenderSystem, render_system)

    RenderSystem::RenderSystem() {
        MangoRHI::initialize(transform<RenderAPI, MangoRHI::API>(engine_config->api));
        auto &context = MangoRHI::get_context();

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
        context.set_multisample_count(MangoRHI::MultisampleCount::e2);

        auto &render_pass = context.get_render_pass_reference();
        render_pass.create_render_target("depth", MangoRHI::RenderTargetUsage::eDepth);
        render_pass.create_render_target("output", MangoRHI::RenderTargetUsage::eColor);
        render_pass.create_render_target("scene_texture", MangoRHI::RenderTargetUsage::eTexture);
        context.set_clear_value("depth", { .depth_stencil = { .depth = 1.0f, .stencil = 1 } });
        render_pass.add_output_render_target("output", MangoRHI::RenderTargetLayout::eColor, {
            .src_color_factor = MangoRHI::BlendFactor::eSrcAlpha,
            .dst_color_factor = MangoRHI::BlendFactor::eOneMinusSrcAlpha,
            .color_op = MangoRHI::BlendOp::eAdd,
            .src_alpha_factor = MangoRHI::BlendFactor::eOne,
            .dst_alpha_factor = MangoRHI::BlendFactor::eZero,
            .alpha_op = MangoRHI::BlendOp::eAdd
        });
        render_pass.add_resolve_render_target("scene_texture", MangoRHI::RenderTargetLayout::eColor);
        render_pass.set_depth_render_target("depth", MangoRHI::RenderTargetLayout::eDepth);
        render_pass.add_subpass("main", MangoRHI::PipelineBindPoint::eGraphicsPipeline);
        render_pass.add_input_render_target("scene_texture", MangoRHI::RenderTargetLayout::eShaderRead);
        render_pass.add_output_render_target("output", MangoRHI::RenderTargetLayout::eColor);
        render_pass.add_resolve_render_target(MANGORHI_SURFACE_RENDER_TARGET_NAME, MangoRHI::RenderTargetLayout::eColor);
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

        render_command.reset(new RenderCommand(*this));
        viewport = { 0, 0, 0, 0, 0, 1 };
        event_system->add_event_callback<WindowResizedEvent>([&](auto &event) {
            this->viewport.width = static_cast<f32>(event.width);
            this->viewport.height = static_cast<f32>(event.height);
        });
    }

    RenderSystem::~RenderSystem() {
        render_command.reset();
        MangoRHI::quit();
    }

    void RenderSystem::set_bg_color(f32 r, f32 g, f32 b, f32 a) {
        auto &context = MangoRHI::get_context();
        context.set_clear_value("output", { .color = { r, g, b, a } });
    }

    Result RenderSystem::acquire() {
        auto &context = MangoRHI::get_context();
        if (context.begin_frame() != MangoRHI::Result::eSuccess) {
            return Result::eFailed;
        }
        return Result::eSuccess;
    }

    void RenderSystem::begin_render() {
        auto &context = MangoRHI::get_context();
        auto &command = context.get_current_command_reference();
        command.set_viewport(viewport);
        command.set_scissor({ 0, 0, static_cast<u32>(viewport.width), static_cast<u32>(viewport.height) });
    }

    void RenderSystem::end_render() {
    }

    Result RenderSystem::present() {
        auto &context = MangoRHI::get_context();
        if (context.end_frame() != MangoRHI::Result::eSuccess) {
            return Result::eFailed;
        }
        return Result::eSuccess;
    }
}
