/**
 * @file test_core.c
 * @brief 核心处理模块测试
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "datapipe.h"

#define TEST_PASS(msg) printf("[PASS] %s\n", msg)
#define TEST_FAIL(msg) printf("[FAIL] %s\n", msg)

int main() {
    printf("=== Core Module Tests ===\n\n");
    int all_passed = 1;
    
    /* 测试 1: 创建和销毁 */
    datapipe_context_t* ctx = datapipe_create(DATAPIPE_CODEC_BASE64);
    if (ctx && ctx->input && ctx->output && ctx->codec == DATAPIPE_CODEC_BASE64) {
        TEST_PASS("Context creation");
    } else {
        TEST_FAIL("Context creation");
        all_passed = 0;
    }
    
    /* 测试 2: 写入数据 */
    uint8_t input[] = "Hello";
    if (datapipe_write(ctx, input, 5) == DATAPIPE_OK && 
        datapipe_buffer_size(ctx->input) == 5) {
        TEST_PASS("Data write");
    } else {
        TEST_FAIL("Data write");
        all_passed = 0;
    }
    
    /* 测试 3: 处理数据（编码）*/
    if (datapipe_process(ctx) == DATAPIPE_OK && 
        datapipe_output_size(ctx) > 0) {
        TEST_PASS("Data process (encode)");
    } else {
        TEST_FAIL("Data process (encode)");
        all_passed = 0;
    }
    
    /* 测试 4: 读取处理后的数据 */
    size_t output_size = datapipe_output_size(ctx);
    uint8_t* output = (uint8_t*)malloc(output_size + 1);
    if (datapipe_read(ctx, output, output_size) == DATAPIPE_OK) {
        TEST_PASS("Data read from output");
    } else {
        TEST_FAIL("Data read from output");
        all_passed = 0;
    }
    free(output);
    
    /* 测试 5: 重置 */
    datapipe_reset(ctx);
    if (datapipe_buffer_size(ctx->input) == 0 && 
        datapipe_output_size(ctx) == 0) {
        TEST_PASS("Context reset");
    } else {
        TEST_FAIL("Context reset");
        all_passed = 0;
    }
    
    /* 测试 6: 版本信息 */
    const char* version = datapipe_version();
    if (version && strlen(version) > 0) {
        TEST_PASS("Version info");
    } else {
        TEST_FAIL("Version info");
        all_passed = 0;
    }
    
    datapipe_destroy(ctx);
    
    printf("\n=== Core Tests %s ===\n", 
           all_passed ? "PASSED" : "FAILED");
    return all_passed ? 0 : 1;
}