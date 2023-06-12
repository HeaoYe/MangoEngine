#include "vulkan_imgui_backend.hpp"
#include "imgui/backends/imgui_impl_vulkan.h"
#include "MangoEngine/core/event.hpp"

namespace MangoEngine {
    template <>
    ImGuiBackend *get_imgui_backend<RenderAPI::eVulkan>() {
        return reinterpret_cast<ImGuiBackend *>(new VulkanImGuiBackend());
    }

    VulkanImGuiBackend::VulkanImGuiBackend() : context(dynamic_cast<MangoRHI::VulkanContext &>(MangoRHI::get_context())) {
        VkDescriptorPoolSize pool_sizes[] = {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
        VkDescriptorPoolCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        create_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        create_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        create_info.poolSizeCount = static_cast<u32>(IM_ARRAYSIZE(pool_sizes));
        create_info.pPoolSizes = pool_sizes;
        vkCreateDescriptorPool(context.get_device()->get_logical_device(), &create_info, context.get_allocator(), &descriptor_pool);

        ImGui_ImplVulkan_InitInfo init {};
        init.Instance = context.get_instance();
        init.PhysicalDevice = context.get_device()->get_physical_device();
        init.Device = context.get_device()->get_logical_device();
        init.QueueFamily = context.get_device()->get_graphics_family_index();
        init.Queue = context.get_device()->get_graphics_queue();
        init.PipelineCache = VK_NULL_HANDLE;
        init.DescriptorPool = descriptor_pool;
        init.Subpass = 1;
        init.MinImageCount = context.get_swapchain()->get_image_count();
        init.ImageCount = context.get_swapchain()->get_image_count();
        init.MSAASamples = context.get_multisample_count();
        init.Allocator = context.get_allocator();
        init.CheckVkResultFn = [](VkResult res) -> void {
            if (res != VK_SUCCESS) {
                MG_ERROR("ImGui Vulkan Error {}", res)
            }
        };
        ImGui_ImplVulkan_Init(&init, context.get_render_pass()->get_render_pass());

        MangoRHI::VulkanCommand command;
        context.get_command_pool()->allocate_single_use(command);
        ImGui_ImplVulkan_CreateFontsTexture(command.get_command_buffer());
        context.get_command_pool()->free(command);
        ImGui_ImplVulkan_DestroyFontUploadObjects();

        auto *scene = context.get_render_pass()->get_render_targets()[context.get_render_pass()->get_render_target_index_by_name("scene_texture")].get();
        sampler.reset(dynamic_cast<MangoRHI::VulkanSampler *>(context.get_resource_factory_reference().create_sampler().release()));
        scene_texture = ImGui_ImplVulkan_AddTexture(sampler->get_sampler(), scene->get_image_views()[0], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        context.add_resource_recreate_callback([this, scene]() {
            auto *scene = context.get_render_pass()->get_render_targets()[context.get_render_pass()->get_render_target_index_by_name("scene_texture")].get();
            vkFreeDescriptorSets(context.get_device()->get_logical_device(), descriptor_pool, 1, &scene_texture);
            scene_texture = ImGui_ImplVulkan_AddTexture(sampler->get_sampler(), scene->get_image_views()[0], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        });
    }

    VulkanImGuiBackend::~VulkanImGuiBackend() {
        vkDeviceWaitIdle(context.get_device()->get_logical_device());
        ImGui_ImplVulkan_Shutdown();
        vkDestroyDescriptorPool(context.get_device()->get_logical_device(), descriptor_pool, context.get_allocator());
    }

    void VulkanImGuiBackend::begin_imgui() {
        context.get_current_command_reference().next_subpass();
        ImGui_ImplVulkan_NewFrame();
    }

    void VulkanImGuiBackend::end_imgui() {
        auto &command = reinterpret_cast<MangoRHI::VulkanCommand &>(context.get_current_command_reference());
        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(draw_data, command.get_command_buffer());
    }

    ImTextureID VulkanImGuiBackend::get_scene_texture() {
        return scene_texture;
    }
}
