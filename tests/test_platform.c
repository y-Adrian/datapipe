/**
 * @file test_platform.c
 * @brief 平台抽象层测试
 */

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"

#define TEST_PASS(msg) printf("[PASS] %s\n", msg)
#define TEST_FAIL(msg) printf("[FAIL] %s\n", msg)

int main() {
    printf("=== Platform Module Tests ===\n\n");
    int all_passed = 1;
    
    /* 测试 1: 平台名称 */
    const char* platform = datapipe_get_platform_name();
    printf("  Platform: %s\n", platform);
    if (platform && strlen(platform) > 0) {
        TEST_PASS("Get platform name");
    } else {
        TEST_FAIL("Get platform name");
        all_passed = 0;
    }
    
    /* 测试 2: 架构名称 */
    const char* arch = datapipe_get_arch_name();
    printf("  Architecture: %s\n", arch);
    if (arch && strlen(arch) > 0) {
        TEST_PASS("Get architecture name");
    } else {
        TEST_FAIL("Get architecture name");
        all_passed = 0;
    }
    
    /* 测试 3: CPU 核心数 */
    int cores = datapipe_get_cpu_cores();
    printf("  CPU cores: %d\n", cores);
    if (cores > 0) {
        TEST_PASS("Get CPU cores");
    } else {
        TEST_FAIL("Get CPU cores");
        all_passed = 0;
    }
    
    /* 测试 4: 页面大小 */
    size_t page_size = datapipe_get_page_size();
    printf("  Page size: %zu bytes\n", page_size);
    if (page_size > 0) {
        TEST_PASS("Get page size");
    } else {
        TEST_FAIL("Get page size");
        all_passed = 0;
    }
    
    /* 测试 5: 对齐分配 */
    void* aligned_ptr = datapipe_aligned_alloc(1024, 64);
    if (aligned_ptr && ((uintptr_t)aligned_ptr % 64 == 0)) {
        TEST_PASS("Aligned allocation");
        datapipe_aligned_free(aligned_ptr);
    } else {
        TEST_FAIL("Aligned allocation");
        all_passed = 0;
    }
    
    /* 测试 6: 时间戳 */
    uint64_t time1 = datapipe_get_time_ns();
    uint64_t time2 = datapipe_get_time_ns();
    if (time2 >= time1) {
        TEST_PASS("Get time nanoseconds");
    } else {
        TEST_FAIL("Get time nanoseconds");
        all_passed = 0;
    }
    
    printf("\n=== Platform Tests %s ===\n", 
           all_passed ? "PASSED" : "FAILED");
    return all_passed ? 0 : 1;
}
