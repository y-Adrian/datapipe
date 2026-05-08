# Platform module CMakeLists.txt
add_library(datapipe_platform STATIC platform.c)
target_include_directories(datapipe_platform PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
