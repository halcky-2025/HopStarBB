set(BGFX_BUILD_TOOLS OFF)
set(BGFX_BUILD_EXAMPLES OFF)
set(BGFX_BUILD_TESTS OFF)
set(BGFX_INSTALL OFF)
set(BGFX_CUSTOM_TARGETS OFF)

set(BGFX_CONFIG_RENDERER_VULKAN ON)
set(BGFX_CONFIG_RENDERER_OPENGLES OFF)

set(SDL_DISABLE_PCH ON CACHE BOOL "" FORCE)
set(CMAKE_DISABLE_PRECOMPILE_HEADERS ON CACHE BOOL "" FORCE)
cmake_minimum_required(VERSION 3.16)

project(my_app)

if(NOT TARGET SDL3::SDL3)
    find_package(SDL3 CONFIG REQUIRED)
endif()
add_subdirectory(bgfx.cmake)

# SDL_ttf
set(SDLTTF_SAMPLES OFF CACHE BOOL "" FORCE)
set(SDLTTF_INSTALL OFF CACHE BOOL "" FORCE)
set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)
add_subdirectory(../SDL_ttf SDL_ttf)

# PortAudio with OpenSL ES for Android
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(portaudio_opensles)

# FFmpeg library path (jniLibs directory)
set(FFMPEG_DIR "${CMAKE_SOURCE_DIR}/../src/main/jniLibs/${ANDROID_ABI}")
set(FFMPEG_INCLUDE_DIR "${FFMPEG_DIR}/include")
set(FFMPEG_LIB_DIR "${FFMPEG_DIR}")

# Import FFmpeg libraries
add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/libavutil.so")

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/libswresample.so")

add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/libswscale.so")

add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/libavcodec.so")

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/libavformat.so")

add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/libavfilter.so")

add_library(avdevice SHARED IMPORTED)
set_target_properties(avdevice PROPERTIES IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/libavdevice.so")

# Path to main Project2 source folder (relative to android/app/jni/src/)
set(PROJECT2_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Project2")

add_library(main SHARED
    ${PROJECT2_SOURCE_DIR}/main2.cpp
    ${PROJECT2_SOURCE_DIR}/sqlite3.c
    ${PROJECT2_SOURCE_DIR}/platform/android/SpatialLocation.cpp
    ${PROJECT2_SOURCE_DIR}/platform/android/CommPSTN.cpp
    ${PROJECT2_SOURCE_DIR}/platform/android/SystemNotification.cpp
    ${PROJECT2_SOURCE_DIR}/platform/android/SystemBackground.cpp
    ${PROJECT2_SOURCE_DIR}/platform/android/SystemMedia.cpp
    ${PROJECT2_SOURCE_DIR}/platform/android/SystemNearby.cpp
)
set_target_properties(main PROPERTIES CXX_STANDARD 20)
set_target_properties(main PROPERTIES CXX_STANDARD_REQUIRED ON)
target_include_directories(main PRIVATE
    ${PROJECT2_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/bx/include
    ${CMAKE_CURRENT_SOURCE_DIR}/bimg/include
    ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/include
    ${CMAKE_CURRENT_SOURCE_DIR}/portaudio_opensles/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../SDL_ttf/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../libcoro/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../nlohmann_json/include
    ${FFMPEG_INCLUDE_DIR}
)
target_link_libraries(main
    PRIVATE SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    bgfx
    bimg
    bx
    PortAudio
    android
    log
    avutil
    swresample
    swscale
    avcodec
    avformat)
