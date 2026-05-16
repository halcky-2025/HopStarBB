# bimg library CMake configuration

set(BIMG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../bimg)

set(BIMG_SOURCES
    ${BIMG_DIR}/src/image.cpp
    ${BIMG_DIR}/src/image_cubemap_filter.cpp
    ${BIMG_DIR}/src/image_decode.cpp
    ${BIMG_DIR}/src/image_encode.cpp
    ${BIMG_DIR}/src/image_gnf.cpp
)

# 3rdparty sources
set(BIMG_3RDPARTY_SOURCES
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_averages_and_directions.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_block_sizes.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_color_quantize.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_color_unquantize.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_compress_symbolic.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_compute_variance.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_decompress_symbolic.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_diagnostic_trace.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_entry.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_find_best_partitioning.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_ideal_endpoints_and_weights.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_image.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_integer_sequence.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_mathlib.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_mathlib_softfloat.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_partition_tables.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_percentile_tables.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_pick_best_endpoint_format.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_quantization.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_symbolic_physical.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_weight_align.cpp
    ${BIMG_DIR}/3rdparty/astc-encoder/source/astcenc_weight_quant_xfer_tables.cpp
)

add_library(bimg STATIC ${BIMG_SOURCES} ${BIMG_3RDPARTY_SOURCES})

target_include_directories(bimg
    PUBLIC
        ${BIMG_DIR}/include
    PRIVATE
        ${BIMG_DIR}/3rdparty
        ${BIMG_DIR}/3rdparty/astc-encoder
        ${BIMG_DIR}/3rdparty/astc-encoder/include
        ${BIMG_DIR}/3rdparty/iqa/include
        ${BIMG_DIR}/3rdparty/nvtt
        ${BIMG_DIR}/3rdparty/etc1
        ${BIMG_DIR}/3rdparty/etc2
        ${BIMG_DIR}/3rdparty/pvrtc
        ${BIMG_DIR}/3rdparty/libsquish
        ${BIMG_DIR}/3rdparty/edtaa3
        ${BIMG_DIR}/3rdparty/tinyexr/deps   # <miniz/miniz.h> 解決のため一段上を入れる
        ${BIMG_DIR}/3rdparty/tinyexr/deps/miniz
        ${BIMG_DIR}/3rdparty/lodepng
        ${BIMG_DIR}/3rdparty/stb
)

target_link_libraries(bimg PUBLIC bx)

target_compile_definitions(bimg
    PRIVATE
        BIMG_DECODE_ASTC=1
)
