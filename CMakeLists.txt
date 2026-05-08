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
option(BUILD_DOCS "Build documentation with Doxygen" OFF)
option(ENABLE_SYMBOL_HIDDEN "Hide internal symbols by default" ON)

# 设置编译标志
if(ENABLE_SYMBOL_HIDDEN)
    add_compile_options(-fvisibility=hidden)
endif()

# 设置头文件目录 - 添加 include/datapipe 以支持 #include "xxx.h" 形式
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/datapipe)

# 创建主库（直接列出源文件，不使用子目录静态库）
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

# 生成 pkg-config 文件
configure_file(
    "${CMAKE_SOURCE_DIR}/datapipe.pc.in"
    "${CMAKE_BINARY_DIR}/datapipe.pc"
    @ONLY
)
install(FILES "${CMAKE_BINARY_DIR}/datapipe.pc"
        DESTINATION lib/pkgconfig)

# Doxygen 文档构建
if(BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    set(DOXYGEN_INPUT_DIR "${CMAKE_SOURCE_DIR}")
    set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs")
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/docs/Doxyfile)

    # 创建输出目录
    file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

    # 配置 Doxyfile
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    install(DIRECTORY ${CMAKE_BINARY_DIR}/docs/html
            DESTINATION share/doc/datapipe
            COMPONENT documentation)
endif()

# 打印构建信息
message(STATUS "DataPipe build configuration:")
message(STATUS "  Platform: ${DATAPIPE_PLATFORM}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Shared library: ${BUILD_SHARED_LIBS}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
