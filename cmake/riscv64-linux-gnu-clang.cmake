# riscv64 Linux Clang Toolchain
# 用于交叉编译 RISC-V 64 位平台 (使用 Clang/LLVM)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

# 设置 Clang 交叉编译工具链
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Clang 目标三元组
set(CMAKE_C_COMPILER_TARGET riscv64-unknown-linux-gnu)
set(CMAKE_CXX_COMPILER_TARGET riscv64-unknown-linux-gnu)

# 链接器配置 - 使用 clang 作为链接器（Clang 内置 RISC-V 支持）
set(CMAKE_LINKER clang)
set(CMAKE_LINKER_TARGET riscv64-unknown-linux-gnu)
set(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=ld")

# 目标环境路径
set(CMAKE_FIND_ROOT_PATH /usr/riscv64-linux-gnu)

# 搜索程序时只在宿主系统查找
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# 搜索库和头文件时在目标环境中查找
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Clang 编译选项 - RISC-V 64gc (G继承 + C扩展)
set(CMAKE_C_FLAGS_INIT "-target riscv64-unknown-linux-gnu -march=rv64gc")
set(CMAKE_CXX_FLAGS_INIT "-target riscv64-unknown-linux-gnu -march=rv64gc")

message(STATUS "Configuring for riscv64 Linux with Clang")
message(STATUS "  C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "  Target: ${CMAKE_C_COMPILER_TARGET}")
