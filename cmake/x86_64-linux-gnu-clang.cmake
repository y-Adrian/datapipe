# x86_64 Linux Clang Toolchain
# 用于交叉编译 x86_64 平台 (使用 Clang/LLVM)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# 设置 Clang 交叉编译工具链
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Clang 链接器配置
set(CMAKE_LINKER clang)
set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=lld")

# 目标环境路径
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-linux-gnu)

# 搜索程序时只在宿主系统查找
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# 搜索库和头文件时在目标环境中查找
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Clang 编译选项
set(CMAKE_C_FLAGS_INIT "-m64")
set(CMAKE_CXX_FLAGS_INIT "-m64")

message(STATUS "Configuring for x86_64 Linux with Clang")
message(STATUS "  C Compiler: ${CMAKE_C_COMPILER}")
