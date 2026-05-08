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

## 快速开始

### 本地构建

```bash
# 克隆仓库
git clone https://github.com/y-Adrian/datapipe.git
cd datapipe

# 创建构建目录
mkdir build && cd build

# 配置和编译
cmake ..
make -j$(nproc)

# 运行测试
ctest --output-on-failure
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

## 贡献

欢迎提交 Issue 和 Pull Request！

## 许可证

MIT License
