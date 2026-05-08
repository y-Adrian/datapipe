# aarch64 (ARM64) 平台测试环境
# 使用多阶段构建，首先在 x86_64 上交叉编译，然后在 ARM64 模拟器中测试

FROM --platform=linux/amd64 ubuntu:22.04 AS builder

# 设置环境变量
ENV DEBIAN_FRONTEND=noninteractive
ENV PLATFORM=aarch64

# 安装交叉编译工具链
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    binutils-aarch64-linux-gnu \
    qemu-user-static \
    && rm -rf /var/lib/apt/lists/*

# 设置工作目录
WORKDIR /workspace

# 复制项目文件
COPY . /workspace/

# 使用交叉编译工具链构建
RUN mkdir -p build && cd build && \
    cmake .. \
        -DCMAKE_TOOLCHAIN_FILE=../cmake/aarch64-linux-gnu.cmake \
        -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)

# 创建最终镜像，包含 qemu 用于运行 ARM 二进制
FROM ubuntu:22.04

# 安装 qemu-user-static 用于运行交叉编译的二进制
RUN apt-get update && apt-get install -y \
    qemu-user-static \
    && rm -rf /var/lib/apt/lists/*

# 从构建阶段复制构建产物
COPY --from=builder /workspace/build /workspace/build
COPY --from=builder /workspace/tests /workspace/tests

# 设置工作目录
WORKDIR /workspace

# 运行测试的脚本
RUN echo '#!/bin/bash\n\
cd /workspace/build\n\
qemu-aarch64-static ./bin/test_buffer\n\
qemu-aarch64-static ./bin/test_codec\n\
qemu-aarch64-static ./bin/test_core\n\
qemu-aarch64-static ./bin/test_platform\n\
' > /workspace/run_tests.sh && chmod +x /workspace/run_tests.sh

CMD ["/bin/bash"]
