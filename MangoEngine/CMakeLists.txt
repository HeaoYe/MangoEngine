project(MangoEngine)
add_compile_options(-fPIC)

file(GLOB MangoEngineSource ${MangoEngineSource} src/*.cpp)
file(GLOB MangoEngineSource ${MangoEngineSource} src/core/*.cpp)
file(GLOB MangoEngineSource ${MangoEngineSource} src/render/*.cpp)
file(GLOB MangoEngineSource ${MangoEngineSource} src/imgui/*.cpp)
file(GLOB MangoEngineSource ${MangoEngineSource} platform/xcb/*.cpp)
file(GLOB MangoEngineSource ${MangoEngineSource} platform/vulkan/*.cpp)

set(ImGuiSource ${thirdparty_dir}/imgui/imgui/backends/imgui_impl_vulkan.cpp)
file(GLOB ImGuiSource ${ImGuiSource} ${thirdparty_dir}/imgui/imgui/*.cpp)

add_library(MangoEngine SHARED ${MangoEngineSource} ${ImGuiSource})

target_include_directories(MangoEngine PUBLIC include/ ${thirdparty_dir}/imgui)
target_link_directories(MangoEngine PUBLIC ../build/MangoRHI/MangoRHI)
target_link_libraries(MangoEngine PUBLIC MangoRHI)
