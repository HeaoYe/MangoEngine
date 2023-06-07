project(Sandbox)
add_executable(Sandbox main.cpp)
target_link_directories(Sandbox PUBLIC ../build/MangoEngine)
target_link_libraries(Sandbox PUBLIC MangoEngine)
