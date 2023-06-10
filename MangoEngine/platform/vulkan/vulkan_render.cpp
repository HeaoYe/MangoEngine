#include "MangoEngine/render/render.hpp"
#include "MangoEngine/core/window.hpp"
#include <GLFW/glfw3.h>

namespace MangoEngine {
    template <>
    void *get_context_info<RenderAPI::eVulkan>() {
        MangoRHI::VulkanContextInfo *info = new MangoRHI::VulkanContextInfo();
        info->extensions = glfwGetRequiredInstanceExtensions(&info->extension_count);
        info->app_name = engine_config->title;
        info->engine_name = "Mango Engine";
        info->surface_create_callback = [&](VkInstance instance, VkAllocationCallbacks *allocator) {
            VkSurfaceKHR surface;
            if (glfwCreateWindowSurface(instance, static_cast<GLFWwindow *>(window_system->get_native_pointer()), allocator, &surface) != VK_SUCCESS) {
                MG_FATAL("Failed create surface.")
            }
            return surface;
        };
        return info;
    }
}
