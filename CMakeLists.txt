cmake_minimum_required(VERSION 3.16)
project(DataPipe VERSION 1.0.0 LANGUAGES C)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 平台检测
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|i686|i386")
    set(DATAPIPE_PLATFORM "x86_64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64|arm64")
    set(DATAPIPE_PLATFORM "aarch64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "riscv64")
    set(DATAPIPE_PLATFORM "riscv64")
else()
    set(DATAPIPE_PLATFORM "unknown")
endif()

message(STATUS "Building DataPipe for platform: ${DATAPIPE_PLATFORM}")
message(STATUS "CMake System Processor: ${CMAKE_SYSTEM_PROCESSOR}")

# 编译选项
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_TESTS "Build test programs" ON)
option(ENABLE_SYMBOL_HIDDEN "Hide internal symbols by default" ON)

# 设置编译标志
if(ENABLE_SYMBOL_HIDDEN)
    add_compile_options(-fvisibility=hidden)
endif()

# 添加头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 添加子目录
add_subdirectory(src/core)
add_subdirectory(src/buffer)
add_subdirectory(src/codec)
add_subdirectory(src/platform)

# 创建主库
set(DATAPIPE_SOURCES
    src/core/datapipe.c
    src/core/core.c
    src/buffer/buffer.c
    src/codec/codec.c
    src/platform/platform.c
)

if(BUILD_SHARED_LIBS)
    add_library(datapipe SHARED ${DATAPIPE_SOURCES})
    set_target_properties(datapipe PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
        OUTPUT_NAME "datapipe"
    )
else()
    add_library(datapipe STATIC ${DATAPIPE_SOURCES})
endif()

# 设置库的公共头文件
target_include_directories(datapipe PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 安装规则
install(TARGETS datapipe
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/datapipe
    DESTINATION include
)

# 构建测试
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# 打印构建信息
message(STATUS "DataPipe build configuration:")
message(STATUS "  Platform: ${DATAPIPE_PLATFORM}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Shared library: ${BUILD_SHARED_LIBS}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
