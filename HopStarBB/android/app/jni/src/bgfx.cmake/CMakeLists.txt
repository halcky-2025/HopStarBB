# bgfx.cmake - CMake build configuration for bgfx, bimg, and bx
# This file should be included via add_subdirectory() from the parent CMakeLists.txt

cmake_minimum_required(VERSION 3.6)

# Options
option(BGFX_BUILD_TOOLS "Build bgfx tools" OFF)
option(BGFX_BUILD_EXAMPLES "Build bgfx examples" OFF)
option(BGFX_BUILD_TESTS "Build bgfx tests" OFF)
option(BGFX_CONFIG_RENDERER_VULKAN "Enable Vulkan renderer" ON)
option(BGFX_CONFIG_RENDERER_OPENGLES "Enable OpenGL ES renderer" OFF)

# Include the individual library configurations
include(${CMAKE_CURRENT_SOURCE_DIR}/bx.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/bimg.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/bgfx.cmake)
