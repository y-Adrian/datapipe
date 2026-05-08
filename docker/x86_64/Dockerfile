# x86_64 平台测试环境
FROM ubuntu:22.04

# 设置环境变量
ENV DEBIAN_FRONTEND=noninteractive
ENV PLATFORM=x86_64

# 安装基础工具和交叉编译工具链
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    gcc \
    g++ \
    binutils \
    nm \
    objdump \
    size \
    qemu-user-static \
    && rm -rf /var/lib/apt/lists/*

# 设置工作目录
WORKDIR /workspace

# 复制项目文件
COPY . /workspace/

# 创建构建脚本
RUN echo '#!/bin/bash\n\
mkdir -p build && cd build\n\
cmake .. -DCMAKE_BUILD_TYPE=Release\n\
make -j$(nproc)\n\
ctest --output-on-failure\n\
' > /workspace/build.sh && chmod +x /workspace/build.sh

CMD ["/bin/bash"]
