/**
 * @file codec.h
 * @brief 数据编解码模块头文件
 */

#ifndef DATAPIPE_CODEC_H
#define DATAPIPE_CODEC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 编解码类型
 */
typedef enum {
    DATAPIPE_CODEC_NONE = 0,   /* 无编码 */
    DATAPIPE_CODEC_BASE64,      /* Base64 编码 */
    DATAPIPE_CODEC_HEX,         /* Hex 编码 */
    DATAPIPE_CODEC_XOR          /* XOR 简单加密 */
} datapipe_codec_type_t;

/**
 * @brief 编码数据
 * @param type 编码类型
 * @param input 输入数据
 * @param input_len 输入长度
 * @param output 输出缓冲区（调用者分配）
 * @param output_len 输入输出参数，传入输出缓冲区大小，传出实际输出大小
 * @return 成功返回 DATAPIPE_OK，失败返回错误码
 */
int datapipe_encode(datapipe_codec_type_t type,
                    const uint8_t* input, size_t input_len,
                    uint8_t* output, size_t* output_len);

/**
 * @brief 解码数据
 * @param type 编码类型
 * @param input 输入数据
 * @param input_len 输入长度
 * @param output 输出缓冲区（调用者分配）
 * @param output_len 输入输出参数，传入输出缓冲区大小，传出实际输出大小
 * @return 成功返回 DATAPIPE_OK，失败返回错误码
 */
int datapipe_decode(datapipe_codec_type_t type,
                    const uint8_t* input, size_t input_len,
                    uint8_t* output, size_t* output_len);

/**
 * @brief 计算编码后数据大小
 * @param type 编码类型
 * @param input_len 输入长度
 * @return 编码后需要的缓冲区大小
 */
size_t datapipe_encoded_size(datapipe_codec_type_t type, size_t input_len);

/**
 * @brief 计算解码后数据大小
 * @param type 编码类型
 * @param input_len 输入长度
 * @return 解码后需要的缓冲区大小
 */
size_t datapipe_decoded_size(datapipe_codec_type_t type, size_t input_len);

#ifdef __cplusplus
}
#endif

#endif /* DATAPIPE_CODEC_H */
