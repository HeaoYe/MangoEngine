project(MangoEngine)
add_compile_options(-fPIC)

file(GLOB MangoEngineInclude include/*.hpp)
file(GLOB MangoEngineSource ${MangoEngineSource} src/*.cpp)
file(GLOB MangoEngineSource ${MangoEngineSource} src/core/*.cpp)
file(GLOB MangoEngineSource ${MangoEngineSource} platform/xcb/src/core/*.cpp)
add_library(MangoEngine SHARED ${MangoEngineSource} ${MangoEngineInclude})

target_include_directories(MangoEngine PUBLIC include/)
target_link_directories(MangoEngine PUBLIC ../build/MangoRHI/MangoRHI)
target_link_libraries(MangoEngine PUBLIC MangoRHI)
