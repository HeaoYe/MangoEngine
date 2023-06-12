#pragma once

#include "MangoEngine/render/imgui/imgui_renderer.hpp"
#include "MangoEngine/render/render.hpp"

namespace MangoEngine {
    class VulkanImGuiBackend final : public ImGuiBackend {
    public:
        VulkanImGuiBackend();
        ~VulkanImGuiBackend() override;
        void begin_imgui() override;
        void end_imgui() override;
        ImTextureID get_scene_texture() override;

    private:
        MangoRHI::VulkanContext &context;
        VkDescriptorPool descriptor_pool { VK_NULL_HANDLE };
        std::unique_ptr<MangoRHI::VulkanSampler> sampler;
        VkDescriptorSet scene_texture { VK_NULL_HANDLE };
    };
}
