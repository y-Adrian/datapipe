# riscv64 Linux GNU Toolchain
# 用于交叉编译 RISC-V 64 位平台

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

# 设置交叉编译工具链
set(CMAKE_C_COMPILER riscv64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER riscv64-linux-gnu-g++)

# 目标环境路径
set(CMAKE_FIND_ROOT_PATH /usr/riscv64-linux-gnu)

# 搜索程序时只在宿主系统查找
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# 搜索库和头文件时在目标环境中查找
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# 平台特定编译选项 - 使用通用 RISC-V 指令集
set(CMAKE_C_FLAGS_INIT "-march=rv64gc -mabi=lp64d")
set(CMAKE_CXX_FLAGS_INIT "-march=rv64gc -mabi=lp64d")

message(STATUS "Configuring for riscv64 Linux GNU target")
