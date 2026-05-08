# Codec module CMakeLists.txt
add_library(datapipe_codec STATIC codec.c)
target_include_directories(datapipe_codec PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
