#pragma once

#include "MangoEngine/imgui/imgui_renderer.hpp"
#include "MangoEngine/render/render.hpp"

namespace MangoEngine {
    class VulkanImGuiBackend final : public ImGuiBackend {
    public:
        VulkanImGuiBackend();
        ~VulkanImGuiBackend() override;
        void begin_imgui() override;
        void end_imgui() override;

    private:
        MangoRHI::VulkanContext &context;
        VkDescriptorPool descriptor_pool { VK_NULL_HANDLE };
    };
}
