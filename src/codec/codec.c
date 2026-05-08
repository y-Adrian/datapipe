/**
 * @file codec.c
 * @brief 数据编解码模块实现
 */

#include "codec.h"
#include "datapipe.h"
#include <string.h>
#include <stdlib.h>

/* Base64 编码表 */
static const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* Base64 解码表 */
static const unsigned char base64_decode_table[256] = {
    ['A'] = 0,  ['B'] = 1,  ['C'] = 2,  ['D'] = 3,
    ['E'] = 4,  ['F'] = 5,  ['G'] = 6,  ['H'] = 7,
    ['I'] = 8,  ['J'] = 9,  ['K'] = 10, ['L'] = 11,
    ['M'] = 12, ['N'] = 13, ['O'] = 14, ['P'] = 15,
    ['Q'] = 16, ['R'] = 17, ['S'] = 18, ['T'] = 19,
    ['U'] = 20, ['V'] = 21, ['W'] = 22, ['X'] = 23,
    ['Y'] = 24, ['Z'] = 25, ['a'] = 26, ['b'] = 27,
    ['c'] = 28, ['d'] = 29, ['e'] = 30, ['f'] = 31,
    ['g'] = 32, ['h'] = 33, ['i'] = 34, ['j'] = 35,
    ['k'] = 36, ['l'] = 37, ['m'] = 38, ['n'] = 39,
    ['o'] = 40, ['p'] = 41, ['q'] = 42, ['r'] = 43,
    ['s'] = 44, ['t'] = 45, ['u'] = 46, ['v'] = 47,
    ['w'] = 48, ['x'] = 49, ['y'] = 50, ['z'] = 51,
    ['0'] = 52, ['1'] = 53, ['2'] = 54, ['3'] = 55,
    ['4'] = 56, ['5'] = 57, ['6'] = 58, ['7'] = 59,
    ['8'] = 60, ['9'] = 61, ['+'] = 62, ['/'] = 63
};

/* ========== Base64 编码 ========== */
static int encode_base64(const uint8_t* input, size_t input_len,
                         uint8_t* output, size_t* output_len) {
    size_t required = ((input_len + 2) / 3) * 4;
    if (*output_len < required) {
        *output_len = required;
        return DATAPIPE_ERR_INVALID_PARAM;
    }
    
    size_t i, j = 0;
    for (i = 0; i < input_len; i += 3) {
        uint32_t triple = (input[i] << 16) |
                         ((i + 1 < input_len) ? (input[i + 1] << 8) : 0) |
                         ((i + 2 < input_len) ? input[i + 2] : 0);
        
        output[j++] = base64_chars[(triple >> 18) & 0x3F];
        output[j++] = base64_chars[(triple >> 12) & 0x3F];
        output[j++] = (i + 1 < input_len) ? base64_chars[(triple >> 6) & 0x3F] : '=';
        output[j++] = (i + 2 < input_len) ? base64_chars[triple & 0x3F] : '=';
    }
    
    *output_len = j;
    return DATAPIPE_OK;
}

/* ========== Base64 解码 ========== */
static int decode_base64(const uint8_t* input, size_t input_len,
                         uint8_t* output, size_t* output_len) {
    /* 简化实现：移除填充后解码 */
    if (input_len % 4 != 0) return DATAPIPE_ERR_CODEC;
    
    size_t padding = 0;
    if (input_len >= 1 && input[input_len - 1] == '=') padding++;
    if (input_len >= 2 && input[input_len - 2] == '=') padding++;
    
    size_t required = (input_len / 4) * 3 - padding;
    if (*output_len < required) {
        *output_len = required;
        return DATAPIPE_ERR_INVALID_PARAM;
    }
    
    size_t i, j = 0;
    for (i = 0; i < input_len; i += 4) {
        uint32_t triple = (base64_decode_table[input[i]] << 18) |
                         (base64_decode_table[input[i + 1]] << 12) |
                         ((i + 2 < input_len && input[i + 2] != '=') ? (base64_decode_table[input[i + 2]] << 6) : 0) |
                         ((i + 3 < input_len && input[i + 3] != '=') ? base64_decode_table[input[i + 3]] : 0);
        
        output[j++] = (triple >> 16) & 0xFF;
        if (i + 2 < input_len && input[i + 2] != '=')
            output[j++] = (triple >> 8) & 0xFF;
        if (i + 3 < input_len && input[i + 3] != '=')
            output[j++] = triple & 0xFF;
    }
    
    *output_len = j;
    return DATAPIPE_OK;
}

/* ========== Hex 编码 ========== */
static int encode_hex(const uint8_t* input, size_t input_len,
                     uint8_t* output, size_t* output_len) {
    size_t required = input_len * 2;
    if (*output_len < required) {
        *output_len = required;
        return DATAPIPE_ERR_INVALID_PARAM;
    }
    
    const char hex_table[] = "0123456789ABCDEF";
    for (size_t i = 0; i < input_len; i++) {
        output[i * 2] = hex_table[(input[i] >> 4) & 0x0F];
        output[i * 2 + 1] = hex_table[input[i] & 0x0F];
    }
    
    *output_len = required;
    return DATAPIPE_OK;
}

/* ========== Hex 解码 ========== */
static int decode_hex(const uint8_t* input, size_t input_len,
                     uint8_t* output, size_t* output_len) {
    if (input_len % 2 != 0) return DATAPIPE_ERR_CODEC;
    
    size_t required = input_len / 2;
    if (*output_len < required) {
        *output_len = required;
        return DATAPIPE_ERR_INVALID_PARAM;
    }
    
    for (size_t i = 0; i < required; i++) {
        char hi = input[i * 2];
        char lo = input[i * 2 + 1];
        
        uint8_t h = (hi >= '0' && hi <= '9') ? (hi - '0') :
                    (hi >= 'A' && hi <= 'F') ? (hi - 'A' + 10) :
                    (hi >= 'a' && hi <= 'f') ? (hi - 'a' + 10) : 0;
        uint8_t l = (lo >= '0' && lo <= '9') ? (lo - '0') :
                    (lo >= 'A' && lo <= 'F') ? (lo - 'A' + 10) :
                    (lo >= 'a' && lo <= 'f') ? (lo - 'a' + 10) : 0;
        
        output[i] = (h << 4) | l;
    }
    
    *output_len = required;
    return DATAPIPE_OK;
}

/* ========== XOR 编解码 ========== */
static int encode_xor(const uint8_t* input, size_t input_len,
                      uint8_t* output, size_t* output_len, uint32_t key) {
    if (*output_len < input_len) {
        *output_len = input_len;
        return DATAPIPE_ERR_INVALID_PARAM;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        output[i] = input[i] ^ ((key >> ((i % 4) * 8)) & 0xFF);
    }
    
    *output_len = input_len;
    return DATAPIPE_OK;
}

/* ========== 公共接口 ========== */
int datapipe_encode(datapipe_codec_type_t type,
                    const uint8_t* input, size_t input_len,
                    uint8_t* output, size_t* output_len) {
    if (!input || !output || !output_len) return DATAPIPE_ERR_INVALID_PARAM;
    
    switch (type) {
        case DATAPIPE_CODEC_NONE:
            if (*output_len < input_len) return DATAPIPE_ERR_INVALID_PARAM;
            memcpy(output, input, input_len);
            *output_len = input_len;
            return DATAPIPE_OK;
        case DATAPIPE_CODEC_BASE64:
            return encode_base64(input, input_len, output, output_len);
        case DATAPIPE_CODEC_HEX:
            return encode_hex(input, input_len, output, output_len);
        case DATAPIPE_CODEC_XOR:
            return encode_xor(input, input_len, output, output_len, 0xDEADBEEF);
        default:
            return DATAPIPE_ERR_CODEC;
    }
}

int datapipe_decode(datapipe_codec_type_t type,
                    const uint8_t* input, size_t input_len,
                    uint8_t* output, size_t* output_len) {
    if (!input || !output || !output_len) return DATAPIPE_ERR_INVALID_PARAM;
    
    switch (type) {
        case DATAPIPE_CODEC_NONE:
            if (*output_len < input_len) return DATAPIPE_ERR_INVALID_PARAM;
            memcpy(output, input, input_len);
            *output_len = input_len;
            return DATAPIPE_OK;
        case DATAPIPE_CODEC_BASE64:
            return decode_base64(input, input_len, output, output_len);
        case DATAPIPE_CODEC_HEX:
            return decode_hex(input, input_len, output, output_len);
        case DATAPIPE_CODEC_XOR:
            return encode_xor(input, input_len, output, output_len, 0xDEADBEEF);
        default:
            return DATAPIPE_ERR_CODEC;
    }
}

size_t datapipe_encoded_size(datapipe_codec_type_t type, size_t input_len) {
    switch (type) {
        case DATAPIPE_CODEC_NONE:
            return input_len;
        case DATAPIPE_CODEC_BASE64:
            return ((input_len + 2) / 3) * 4;
        case DATAPIPE_CODEC_HEX:
            return input_len * 2;
        case DATAPIPE_CODEC_XOR:
            return input_len;
        default:
            return 0;
    }
}

size_t datapipe_decoded_size(datapipe_codec_type_t type, size_t input_len) {
    switch (type) {
        case DATAPIPE_CODEC_NONE:
            return input_len;
        case DATAPIPE_CODEC_BASE64:
            return (input_len / 4) * 3;
        case DATAPIPE_CODEC_HEX:
            return input_len / 2;
        case DATAPIPE_CODEC_XOR:
            return input_len;
        default:
            return 0;
    }
}