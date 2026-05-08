#!/bin/bash
# DataPipe 多平台构建脚本

set -e

PROJECT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="${PROJECT_DIR}/build"

echo "========================================"
echo "DataPipe Build Script"
echo "========================================"

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

log_info() { echo -e "${GREEN}[INFO]${NC} $1"; }
log_warn() { echo -e "${YELLOW}[WARN]${NC} $1"; }
log_error() { echo -e "${RED}[ERROR]${NC} $1"; }

# 检测平台
detect_platform() {
    case "$(uname -m)" in
        x86_64)
            echo "x86_64"
            ;;
        aarch64)
            echo "aarch64"
            ;;
        riscv64)
            echo "riscv64"
            ;;
        *)
            echo "unknown"
            ;;
    esac
}

# 构建函数
build_native() {
    log_info "Building native platform..."
    mkdir -p "${BUILD_DIR}"
    cd "${BUILD_DIR}"
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make -j$(nproc)
    log_info "Build completed: ${BUILD_DIR}/lib/libdatapipe.so"
}

# 运行测试
run_tests() {
    log_info "Running tests..."
    cd "${BUILD_DIR}"
    if ctest --output-on-failure; then
        log_info "All tests passed!"
    else
        log_error "Some tests failed!"
        exit 1
    fi
}

# 分析符号
analyze_symbols() {
    log_info "Analyzing symbols..."
    cd "${BUILD_DIR}"
    
    echo "=== Symbol Analysis ==="
    echo ""
    echo "--- nm -S output ---"
    nm -S lib/libdatapipe.so 2>/dev/null || true
    
    echo ""
    echo "--- size output ---"
    size lib/libdatapipe.so 2>/dev/null || true
    
    echo ""
    echo "--- Section Headers ---"
    objdump -h lib/libdatapipe.so | head -20
}

# 清理构建
clean() {
    log_info "Cleaning build..."
    rm -rf "${BUILD_DIR}"
    log_info "Clean completed"
}

# 显示帮助
show_help() {
    cat << EOF
DataPipe Build Script

用法: $0 [选项]

选项:
    build       构建项目（默认）
    test        运行测试
    analyze     分析符号
    clean       清理构建目录
    all         完整构建+测试+分析
    help        显示此帮助信息

示例:
    $0 all      # 完整构建流程
    $0 build    # 仅构建
    $0 test     # 仅运行测试
EOF
}

# 主逻辑
case "${1:-build}" in
    build)
        build_native
        ;;
    test)
        run_tests
        ;;
    analyze)
        analyze_symbols
        ;;
    clean)
        clean
        ;;
    all)
        build_native
        run_tests
        analyze_symbols
        ;;
    help)
        show_help
        ;;
    *)
        log_error "Unknown option: $1"
        show_help
        exit 1
        ;;
esac

echo ""
log_info "Done!"
