cmake_minimum_required(VERSION 3.21)
project(bgfx_deps LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
add_compile_options(-Wno-error -Wno-volatile -Wno-deprecated-enum-enum-conversion -msse4.1 -msse4.2)

set(JNI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../android/app/jni)
set(SRC_DIR ${JNI_DIR}/src)
set(BGFX_DIR ${JNI_DIR}/bgfx)
set(BX_DIR ${SRC_DIR}/bx_new)
set(BIMG_DIR ${SRC_DIR}/bimg_new)

# --- bx ---
file(GLOB BX_SOURCES ${BX_DIR}/src/*.cpp)
# Remove amalgamated to avoid duplicates
list(FILTER BX_SOURCES EXCLUDE REGEX "amalgamated")
add_library(bx STATIC ${BX_SOURCES})
target_include_directories(bx
    PUBLIC ${BX_DIR}/include
    PRIVATE ${BX_DIR}/3rdparty
)
if(UNIX AND NOT APPLE)
    target_include_directories(bx PUBLIC ${BX_DIR}/include/compat/linux)
endif()
target_compile_definitions(bx PUBLIC BX_CONFIG_DEBUG=0)

# --- bimg ---
file(GLOB BIMG_SOURCES ${BIMG_DIR}/src/*.cpp)
file(GLOB ASTCENC_SOURCES ${BIMG_DIR}/3rdparty/astc-encoder/source/*.cpp)
list(FILTER BIMG_SOURCES EXCLUDE REGEX "amalgamated")
list(APPEND BIMG_SOURCES ${ASTCENC_SOURCES})
add_library(bimg STATIC ${BIMG_SOURCES})
target_include_directories(bimg
    PUBLIC ${BIMG_DIR}/include
    PRIVATE ${BIMG_DIR}/src
    PRIVATE ${BIMG_DIR}/3rdparty
    PRIVATE ${BIMG_DIR}/3rdparty/astc-encoder/include
    PRIVATE ${BIMG_DIR}/3rdparty/astc-encoder/source
    PRIVATE ${BIMG_DIR}/3rdparty/tinyexr/deps
    PRIVATE ${BIMG_DIR}/3rdparty/iqa/include
    PRIVATE ${BIMG_DIR}/3rdparty/nvtt
    PRIVATE ${BIMG_DIR}/3rdparty/etc1
    PRIVATE ${BIMG_DIR}/3rdparty/etc2
    PRIVATE ${BIMG_DIR}/3rdparty/pvrtc
    PRIVATE ${BIMG_DIR}/3rdparty/libsquish
    PRIVATE ${BIMG_DIR}/3rdparty/edtaa3
    PRIVATE ${BIMG_DIR}/3rdparty/stb
)
target_link_libraries(bimg PUBLIC bx)

# --- bgfx ---
set(BGFX_SOURCES
    ${BGFX_DIR}/src/bgfx.cpp
    ${BGFX_DIR}/src/debug_renderdoc.cpp
    ${BGFX_DIR}/src/dxgi.cpp
    ${BGFX_DIR}/src/glcontext_egl.cpp
    ${BGFX_DIR}/src/glcontext_wgl.cpp
    ${BGFX_DIR}/src/nvapi.cpp
    ${BGFX_DIR}/src/renderer_agc.cpp
    ${BGFX_DIR}/src/renderer_d3d11.cpp
    ${BGFX_DIR}/src/renderer_d3d12.cpp
    ${BGFX_DIR}/src/renderer_gl.cpp
    ${BGFX_DIR}/src/renderer_gnm.cpp
    ${BGFX_DIR}/src/renderer_noop.cpp
    ${BGFX_DIR}/src/renderer_nvn.cpp
    ${BGFX_DIR}/src/renderer_vk.cpp
    ${BGFX_DIR}/src/renderer_webgpu.cpp
    ${BGFX_DIR}/src/shader.cpp
    ${BGFX_DIR}/src/topology.cpp
    ${BGFX_DIR}/src/vertexlayout.cpp
)

add_library(bgfx STATIC ${BGFX_SOURCES})
target_include_directories(bgfx
    PUBLIC ${BGFX_DIR}/include
    PRIVATE ${BGFX_DIR}/src
    PRIVATE ${BGFX_DIR}/3rdparty
    PRIVATE ${BGFX_DIR}/3rdparty/khronos
)
target_link_libraries(bgfx PUBLIC bimg bx)
target_compile_definitions(bgfx PUBLIC BGFX_CONFIG_RENDERER_VULKAN=1)

# Platform libs
find_package(Vulkan QUIET)
if(Vulkan_FOUND)
    target_link_libraries(bgfx PUBLIC Vulkan::Vulkan)
    target_include_directories(bgfx PRIVATE ${Vulkan_INCLUDE_DIRS})
endif()
find_package(OpenGL QUIET)
if(OpenGL_FOUND)
    target_link_libraries(bgfx PUBLIC OpenGL::GL)
endif()
find_package(X11 QUIET)
if(X11_FOUND)
    target_link_libraries(bgfx PUBLIC ${X11_LIBRARIES})
endif()
target_link_libraries(bgfx PUBLIC dl pthread)

# Install
install(TARGETS bx bimg bgfx
    ARCHIVE DESTINATION lib
)
install(DIRECTORY ${BX_DIR}/include/bx DESTINATION include)
install(DIRECTORY ${BIMG_DIR}/include/bimg DESTINATION include)
install(DIRECTORY ${BGFX_DIR}/include/bgfx DESTINATION include)
