/**
 * @file test_codec.c
 * @brief 编解码模块测试
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codec.h"
#include "datapipe.h"

#define TEST_PASS(msg) printf("[PASS] %s\n", msg)
#define TEST_FAIL(msg) printf("[FAIL] %s\n", msg)

int test_base64() {
    int passed = 1;
    printf("--- Base64 Tests ---\n");
    
    /* 测试编码 */
    uint8_t input[] = "Hello World";
    size_t encoded_len = datapipe_encoded_size(DATAPIPE_CODEC_BASE64, 11);
    uint8_t* encoded = (uint8_t*)malloc(encoded_len);
    
    if (datapipe_encode(DATAPIPE_CODEC_BASE64, input, 11, encoded, &encoded_len) == DATAPIPE_OK) {
        /* "Hello World" 的 Base64 编码应该是 "SGVsbG8gV29ybGQ=" */
        uint8_t expected[] = "SGVsbG8gV29ybGQ=";
        if (encoded_len == 16 && memcmp(encoded, expected, 16) == 0) {
            TEST_PASS("Base64 encode");
        } else {
            TEST_FAIL("Base64 encode");
            passed = 0;
        }
    } else {
        TEST_FAIL("Base64 encode");
        passed = 0;
    }
    
    /* 测试解码 */
    uint8_t decoded[12] = {0};
    size_t decoded_len = sizeof(decoded) - 1;
    if (datapipe_decode(DATAPIPE_CODEC_BASE64, encoded, encoded_len, decoded, &decoded_len) == DATAPIPE_OK) {
        if (decoded_len == 11 && memcmp(decoded, "Hello World", 11) == 0) {
            TEST_PASS("Base64 decode");
        } else {
            TEST_FAIL("Base64 decode");
            passed = 0;
        }
    } else {
        TEST_FAIL("Base64 decode");
        passed = 0;
    }
    
    free(encoded);
    return passed;
}

int test_hex() {
    int passed = 1;
    printf("--- Hex Tests ---\n");
    
    uint8_t input[] = {0x01, 0xAF, 0xCD, 0x00, 0xFF};
    size_t encoded_len = datapipe_encoded_size(DATAPIPE_CODEC_HEX, 5);
    uint8_t* encoded = (uint8_t*)malloc(encoded_len);
    
    if (datapipe_encode(DATAPIPE_CODEC_HEX, input, 5, encoded, &encoded_len) == DATAPIPE_OK) {
        /* 预期输出: "01AFCD00FF" */
        uint8_t expected[] = "01AFCD00FF";
        if (encoded_len == 10 && memcmp(encoded, expected, 10) == 0) {
            TEST_PASS("Hex encode");
        } else {
            TEST_FAIL("Hex encode");
            passed = 0;
        }
    } else {
        TEST_FAIL("Hex encode");
        passed = 0;
    }
    
    free(encoded);
    return passed;
}

int main() {
    printf("=== Codec Module Tests ===\n\n");
    int all_passed = 1;
    
    if (!test_base64()) all_passed = 0;
    if (!test_hex()) all_passed = 0;
    
    printf("\n=== Codec Tests %s ===\n", 
           all_passed ? "PASSED" : "FAILED");
    return all_passed ? 0 : 1;
}
