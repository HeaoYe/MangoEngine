project(MangoEditor)

file(GLOB MangoEditorSource ${MangoEditorSource} src/*.cpp)

add_executable(MangoEditor ${MangoEditorSource})

target_include_directories(MangoEditor PRIVATE ${thirdparty_dir}/imgui/imgui)

target_link_libraries(MangoEditor PUBLIC MangoEngine)
