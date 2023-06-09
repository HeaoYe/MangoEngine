#include "MangoEngine/render/render.hpp"

#if defined (MANGO_USE_XCB_WINDOW)
#include "../xcb/xcb_window.hpp"
#include "vulkan/vulkan_xcb.h"
VkSurfaceKHR surface_create_callback(VkInstance instance, VkAllocationCallbacks *allocator) {
    VkSurfaceKHR surface;
    VkXcbSurfaceCreateInfoKHR create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    MangoEngine::XcbWindowSystem &xcb_window = (MangoEngine::XcbWindowSystem &)*MangoEngine::window;
    create_info.connection = xcb_window.get_connection();
    create_info.window = xcb_window.get_window();
    if (vkCreateXcbSurfaceKHR(instance, &create_info, allocator, &surface) != VK_SUCCESS) {
        MG_FATAL("Failed create xcb vulkan surface.")
    }
    return surface;
}
std::vector<const char *> &get_extensions() {
    std::vector<const char *> *extensions = new std::vector<const char *>();
    extensions->push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    extensions->push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
    return *extensions;
}
#endif

namespace MangoEngine {
    template <>
    MangoRHI::VulkanContextInfo *get_context_info<MangoRHI::VulkanContextInfo>() {
        MangoRHI::VulkanContextInfo *info = new MangoRHI::VulkanContextInfo();
        auto &extensions = get_extensions();
        info->extensions = extensions.data();
        info->extension_count = extensions.size();
        info->app_name = engine_config->title;
        info->engine_name = "Mango Engine";
        MangoRHI::SurfaceCreateFunc f = surface_create_callback;
        info->surface_create_callback = f;
        return info;
    }
}
