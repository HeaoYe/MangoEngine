#include "MangoEngine/render/render.hpp"
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
        context.set_max_in_flight_frame_count(2);
        context.set_swapchain_image_count(3);
        context.set_multisample_count(MangoRHI::MultisampleCount::e1);

        auto &render_pass = context.get_render_pass_reference();
        render_pass.add_output_render_target(MANGORHI_SURFACE_RENDER_TARGET_NAME, MangoRHI::RenderTargetLayout::eColor);
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
    }

    RenderSystem::~RenderSystem() {
        MangoRHI::quit();
    }

    void RenderSystem::set_bg_color(f32 r, f32 g, f32 b, f32 a) {
        context.get_surface_render_target_reference().set_clear_color({ .color = { r, g, b, a } });
    }

    Result RenderSystem::begin_render() {
        if (context.begin_frame() != MangoRHI::Result::eSuccess) {
            return Result::eFailed;
        }
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
