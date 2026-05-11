# bgfx library CMake configuration

set(BGFX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../bgfx)

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
    ${BGFX_DIR}/src/shader.cpp
    ${BGFX_DIR}/src/shader_dxbc.cpp
    ${BGFX_DIR}/src/shader_spirv.cpp
    ${BGFX_DIR}/src/topology.cpp
    ${BGFX_DIR}/src/vertexlayout.cpp
)

add_library(bgfx STATIC ${BGFX_SOURCES})

target_include_directories(bgfx
    PUBLIC
        ${BGFX_DIR}/include
    PRIVATE
        ${BGFX_DIR}/src
        ${BGFX_DIR}/3rdparty
        ${BGFX_DIR}/3rdparty/khronos
        ${BGFX_DIR}/3rdparty/directx-headers/include/directx
)

target_link_libraries(bgfx PUBLIC bimg bx)

# Configuration defines
if(BGFX_CONFIG_RENDERER_VULKAN)
    target_compile_definitions(bgfx
        PUBLIC
            BGFX_CONFIG_RENDERER_VULKAN=1
    )
endif()

if(BGFX_CONFIG_RENDERER_OPENGLES)
    target_compile_definitions(bgfx
        PUBLIC
            BGFX_CONFIG_RENDERER_OPENGLES=1
    )
endif()

if(ANDROID)
    target_link_libraries(bgfx PUBLIC android log EGL GLESv3)
    target_compile_definitions(bgfx
        PRIVATE
            BGFX_CONFIG_RENDERER_OPENGLES_MIN_VERSION=30
    )
endif()
