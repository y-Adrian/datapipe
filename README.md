# DataPipe - 数据管道中间件

[![CI/CD](https://github.com/y-Adrian/datapipe/actions/workflows/ci.yml/badge.svg)](https://github.com/y-Adrian/datapipe/actions)

## 简介

DataPipe 是一个轻量级的 C 语言数据管道中间件，以动态库（.so / .dylib / .dll）形式发布。提供以下功能：

- **缓冲区管理** - 自动扩容的环形缓冲区
- **数据编解码** - 支持 Base64、Hex、XOR 等多种编码格式
- **平台抽象层** - 统一不同硬件平台的 API 接口
- **多平台支持** - x86_64、ARM64、RISC-V 64

## 项目结构

```
datapipe/
├── include/           # 公共头文件
├── src/              # 源代码
│   ├── core/         # 核心处理模块
│   ├── buffer/       # 缓冲区管理
│   ├── codec/        # 编解码模块
│   └── platform/     # 平台抽象层
├── tests/            # 测试用例
├── cmake/            # CMake toolchain 文件
├── docker/           # 各平台 Docker 测试环境
├── scripts/          # 构建和分析脚本
└── docs/             # 文档
```

## 环境要求

### 基础构建工具

| 工具 | 最低版本 | 说明 |
|------|---------|------|
| GCC/Clang | C11 | C 编译器 |
| CMake | 3.16+ | 构建系统 |
| Make | 3.81+ | 构建工具 |

### 各平台交叉编译工具链

#### GCC 工具链

##### x86_64 Linux
```bash
sudo apt install build-essential cmake binutils
gcc --version  # >= 9.0
```

##### ARM64 (aarch64-linux-gnu)
```bash
sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu binutils-aarch64-linux-gnu
aarch64-linux-gnu-gcc --version
```

##### RISC-V 64 (riscv64-linux-gnu)
```bash
sudo apt install gcc-riscv64-linux-gnu g++-riscv64-linux-gnu binutils-riscv64-linux-gnu
riscv64-linux-gnu-gcc --version
```

#### Clang/LLVM 工具链

##### 安装 Clang
```bash
# Ubuntu/Debian
sudo apt install clang lld

# macOS (已有 Xcode Command Line Tools)
# Linuxbrew
brew install llvm
```

##### Clang 版本要求
| 平台 | 最低版本 | 说明 |
|------|---------|------|
| Linux | Clang 10+ | 推荐 Clang 16+ |
| macOS | Xcode CLang | 随 Xcode 更新 |
| 交叉编译 | Clang 15+ | 支持 `--print-targets` |

##### 使用 Clang 构建
```bash
# 本地构建 (x86_64)
mkdir build && cd build
cmake .. -DCMAKE_C_COMPILER=clang \
         -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld"
make -j$(nproc)
```

##### Clang 交叉编译工具链文件
项目提供了 Clang 版本的 CMake 工具链文件：

| 工具链文件 | 目标平台 |
|-----------|---------|
| `cmake/aarch64-linux-gnu-clang.cmake` | ARM64 |
| `cmake/riscv64-linux-gnu-clang.cmake` | RISC-V 64 |
| `cmake/x86_64-linux-gnu-clang.cmake` | x86_64 |

```bash
# ARM64 交叉编译
mkdir build-arm && cd build-arm
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/aarch64-linux-gnu-clang.cmake
make

# RISC-V 64 交叉编译
mkdir build-riscv && cd build-riscv
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/riscv64-linux-gnu-clang.cmake
make
```

#### macOS (Apple Silicon / Intel)
```bash
# 使用 Xcode Command Line Tools
xcode-select --install
# 或使用 Homebrew 安装 CMake
brew install cmake
```

#### Windows (MinGW-w64)
```bash
# 使用 MSYS2
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake
```

## 快速开始

### 本地构建

```bash
# 克隆仓库
git clone https://github.com/y-Adrian/datapipe.git
cd datapipe

# 创建构建目录
mkdir build && cd build

# 配置和编译（Debug 版本）
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)

# 或编译 Release 版本（推荐用于生产环境）
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# 运行测试
ctest --output-on-failure
```

### 构建选项

```bash
# 启用/禁用共享库（默认启用）
cmake -DBUILD_SHARED_LIBS=ON ..
cmake -DBUILD_SHARED_LIBS=OFF ..  # 编译为静态库

# 启用/禁用测试（默认启用）
cmake -DBUILD_TESTS=ON ..

# 启用 Doxygen 文档生成（需要安装 doxygen）
cmake -DBUILD_DOCS=ON ..
make docs

# 符号可见性控制（默认启用，隐藏内部符号）
cmake -DENABLE_SYMBOL_HIDDEN=ON ..
```

### 交叉编译

```bash
# x86_64 Linux
mkdir build-x86 && cd build-x86
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/x86_64-linux-gnu.cmake ..
make

# ARM64 Linux  
mkdir build-arm && cd build-arm
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/aarch64-linux-gnu.cmake ..
make

# RISC-V 64
mkdir build-riscv && cd build-riscv
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/riscv64-linux-gnu.cmake ..
make
```

## 安装

### 安装到系统目录

```bash
cd build
sudo make install

# 刷新动态链接库缓存（Linux）
sudo ldconfig

# 刷新动态链接库缓存（macOS）
sudo update_dyld_shared_cache
```

### 安装到自定义目录

```bash
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
make install
```

## 使用示例

```c
#include "datapipe.h"
#include "core.h"

int main() {
    /* 创建数据管道上下文 */
    datapipe_context_t* ctx = datapipe_create(DATAPIPE_CODEC_BASE64);
    
    /* 写入数据 */
    uint8_t data[] = "Hello World";
    datapipe_write(ctx, data, sizeof(data) - 1);
    
    /* 处理数据（编码）*/
    datapipe_process(ctx);
    
    /* 读取处理后的数据 */
    size_t output_size = datapipe_output_size(ctx);
    uint8_t* output = malloc(output_size);
    datapipe_read(ctx, output, output_size);
    
    /* 清理 */
    datapipe_destroy(ctx);
    free(output);
    return 0;
}
```

## 测试

项目包含完整的单元测试：

```bash
# 运行所有测试
ctest --output-on-failure

# 运行单个测试
./build/bin/test_buffer
./build/bin/test_codec
./build/bin/test_core
./build/bin/test_platform
```

## Docker 测试环境

项目为每种平台提供了 Docker 测试环境：

```bash
# x86_64 平台测试
cd docker/x86_64
docker build -t datapipe:x86_64 .
docker run --rm datapipe:x86_64 /workspace/build.sh

# ARM64 平台测试（使用 QEMU 模拟）
cd docker/aarch64
docker build -t datapipe:aarch64 .
docker run --rm datapipe:aarch64 /workspace/run_tests.sh
```

## CI/CD

每次代码提交都会触发 GitHub Actions 工作流，自动执行：

1. 多平台编译（x86_64、ARM64、RISC-V）
2. 运行所有测试用例
3. 分析二进制符号大小变更
4. 生成构建产物和测试报告

## 代码规范

项目使用以下工具保持代码风格一致：

```bash
# 格式化 C 代码（需要 clang-format）
clang-format -i -style=file src/*.c include/*.h

# 生成 API 文档（需要 doxygen）
doxygen docs/Doxyfile
```

## 未来规划

欢迎贡献以下方向的改进：

- [ ] 添加更多编解码格式（URL 编码、JSON 序列化、LZ4/ZSTD 压缩）
- [ ] 添加性能基准测试
- [ ] 添加流式处理 API
- [ ] 添加更多平台支持（Windows ARM64）
- [ ] 添加 Python/Lua 绑定
- [ ] 添加 benchmark 和 profiling 工具

## 贡献

欢迎提交 Issue 和 Pull Request！

## 许可证

MIT License
