# Core module CMakeLists.txt
add_library(datapipe_core STATIC datapipe.c core.c)
target_include_directories(datapipe_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
