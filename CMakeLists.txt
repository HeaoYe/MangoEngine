cmake_minimum_required(VERSION 3.2)

if (PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
    message("WARN: PROJECT_BINARY_DIR == PROJECT_SOURCE_DIR")
endif()

if (NOT CMAKE_BUILD_TYPE)
    message("default set CMAKE_BUILD_TYPE to Release")
    set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("DEBUG MODE")
    add_definitions(-DMANGO_DEBUG)
endif()

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_language(CXX)

if (WIN32)
    add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
endif()

if (NOT MSVC)
    find_program(CCACHE_PROGRAM ccache)
    if (CCACHE_PROGRAM)
        message("Found CCache: ${CCACHE_PROGRAM}")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROGRAM})
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PROGRAM})
    endif()
endif()

project(MangoEngine-Demo)
set(thirdparty_dir ../../thirdparty)
add_subdirectory(MangoRHI/MangoRHI)
set(thirdparty_dir ../thirdparty)
add_subdirectory(MangoEngine)
add_subdirectory(Sandbox)
