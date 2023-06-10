project(MangoEngine)
add_compile_options(-fPIC)

file(GLOB MangoEngineSource ${MangoEngineSource} src/*.cpp)
file(GLOB MangoEngineSource ${MangoEngineSource} src/core/*.cpp)
file(GLOB MangoEngineSource ${MangoEngineSource} src/render/*.cpp)
file(GLOB MangoEngineSource ${MangoEngineSource} src/imgui/*.cpp)
file(GLOB MangoEngineSource ${MangoEngineSource} platform/xcb/*.cpp)
file(GLOB MangoEngineSource ${MangoEngineSource} platform/vulkan/*.cpp)

set(ImGuiSource ${thirdparty_dir}/imgui/imgui/backends/imgui_impl_vulkan.cpp  ${thirdparty_dir}/imgui/imgui/backends/imgui_impl_glfw.cpp)
file(GLOB ImGuiSource ${ImGuiSource} ${thirdparty_dir}/imgui/imgui/*.cpp)

add_library(MangoEngine SHARED ${MangoEngineSource} ${ImGuiSource})

target_include_directories(MangoEngine PUBLIC include/ ${thirdparty_dir}/imgui ${thirdparty_dir}/glm)
target_include_directories(MangoEngine PRIVATE ${thirdparty_dir}/imgui/imgui)

target_include_directories(MangoEngine PRIVATE ${thirdparty_dir}/glfw/include)
target_link_directories(MangoEngine PRIVATE ${thirdparty_dir}/glfw/build)
target_link_libraries(MangoEngine PRIVATE glfw)

target_link_libraries(MangoEngine PUBLIC MangoRHI)
