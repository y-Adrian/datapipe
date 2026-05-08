/**
 * @file test_buffer.c
 * @brief 缓冲区管理模块测试
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "datapipe.h"

#define TEST_PASS(msg) printf("[PASS] %s\n", msg)
#define TEST_FAIL(msg) printf("[FAIL] %s\n", msg)

int main() {
    printf("=== Buffer Module Tests ===\n\n");
    int all_passed = 1;
    
    /* 测试 1: 创建和销毁 */
    datapipe_buffer_t* buf = datapipe_buffer_create(16);
    if (buf && buf->data && buf->size == 0 && buf->capacity == 16) {
        TEST_PASS("Buffer creation");
    } else {
        TEST_FAIL("Buffer creation");
        all_passed = 0;
    }
    datapipe_buffer_destroy(buf);
    
    /* 测试 2: 写入数据 */
    buf = datapipe_buffer_create(8);
    uint8_t data1[] = "Hello";
    if (datapipe_buffer_write(buf, data1, 5) == DATAPIPE_OK && buf->size == 5) {
        TEST_PASS("Buffer write");
    } else {
        TEST_FAIL("Buffer write");
        all_passed = 0;
    }
    
    /* 测试 3: 读取数据 */
    uint8_t read_buf[6] = {0};
    if (datapipe_buffer_read(buf, read_buf, 5) == DATAPIPE_OK && 
        memcmp(read_buf, "Hello", 5) == 0 && buf->size == 0) {
        TEST_PASS("Buffer read");
    } else {
        TEST_FAIL("Buffer read");
        all_passed = 0;
    }
    
    /* 测试 4: 自动扩容 */
    uint8_t data2[32] = {0};
    memset(data2, 0xAA, 32);
    if (datapipe_buffer_write(buf, data2, 32) == DATAPIPE_OK && 
        buf->capacity >= 32) {
        TEST_PASS("Buffer auto-resize");
    } else {
        TEST_FAIL("Buffer auto-resize");
        all_passed = 0;
    }
    
    /* 测试 5: 清空缓冲区 */
    datapipe_buffer_clear(buf);
    if (buf->size == 0) {
        TEST_PASS("Buffer clear");
    } else {
        TEST_FAIL("Buffer clear");
        all_passed = 0;
    }
    
    /* 测试 6: 边界检查 */
    if (datapipe_buffer_read(buf, read_buf, 1) == DATAPIPE_ERR_INVALID_PARAM) {
        TEST_PASS("Buffer read bounds check");
    } else {
        TEST_FAIL("Buffer read bounds check");
        all_passed = 0;
    }
    
    datapipe_buffer_destroy(buf);
    
    printf("\n=== Buffer Tests %s ===\n", 
           all_passed ? "PASSED" : "FAILED");
    return all_passed ? 0 : 1;
}
