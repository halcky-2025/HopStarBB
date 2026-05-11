# bx library CMake configuration

set(BX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../bx)

set(BX_SOURCES
    ${BX_DIR}/src/allocator.cpp
    ${BX_DIR}/src/bx.cpp
    ${BX_DIR}/src/commandline.cpp
    ${BX_DIR}/src/debug.cpp
    ${BX_DIR}/src/dtoa.cpp
    ${BX_DIR}/src/easing.cpp
    ${BX_DIR}/src/file.cpp
    ${BX_DIR}/src/filepath.cpp
    ${BX_DIR}/src/hash.cpp
    ${BX_DIR}/src/math.cpp
    ${BX_DIR}/src/mutex.cpp
    ${BX_DIR}/src/os.cpp
    ${BX_DIR}/src/process.cpp
    ${BX_DIR}/src/semaphore.cpp
    ${BX_DIR}/src/settings.cpp
    ${BX_DIR}/src/sort.cpp
    ${BX_DIR}/src/string.cpp
    ${BX_DIR}/src/thread.cpp
    ${BX_DIR}/src/timer.cpp
    ${BX_DIR}/src/url.cpp
)

add_library(bx STATIC ${BX_SOURCES})

target_include_directories(bx
    PUBLIC
        ${BX_DIR}/include
    PRIVATE
        ${BX_DIR}/3rdparty
)

if(ANDROID)
    target_include_directories(bx
        PUBLIC
            ${BX_DIR}/include/compat/linux
    )
elseif(WIN32)
    target_include_directories(bx
        PUBLIC
            ${BX_DIR}/include/compat/msvc
    )
elseif(APPLE)
    target_include_directories(bx
        PUBLIC
            ${BX_DIR}/include/compat/osx
    )
endif()

# Required defines for bx
target_compile_definitions(bx
    PUBLIC
        __STDC_FORMAT_MACROS
        $<$<CONFIG:Debug>:BX_CONFIG_DEBUG=1>
        $<$<NOT:$<CONFIG:Debug>>:BX_CONFIG_DEBUG=0>
)

if(ANDROID)
    target_link_libraries(bx PUBLIC log)
endif()
