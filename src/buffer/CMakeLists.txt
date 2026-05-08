# Buffer module CMakeLists.txt
add_library(datapipe_buffer STATIC buffer.c)
target_include_directories(datapipe_buffer PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
