/**
 * @file core.h
 * @brief 核心处理模块头文件
 */

#ifndef DATAPIPE_CORE_H
#define DATAPIPE_CORE_H

#include <stddef.h>
#include <stdint.h>
#include "buffer.h"
#include "codec.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 数据管道上下文结构体
 */
typedef struct {
    datapipe_buffer_t* input;   /* 输入缓冲区 */
    datapipe_buffer_t* output;  /* 输出缓冲区 */
    datapipe_codec_type_t codec; /* 编解码类型 */
    uint32_t xor_key;           /* XOR 密钥（当 codec 为 XOR 时使用）*/
} datapipe_context_t;

/**
 * @brief 创建数据管道上下文
 * @param codec 编解码类型
 * @return 上下文指针，失败返回 NULL
 */
datapipe_context_t* datapipe_create(datapipe_codec_type_t codec);

/**
 * @brief 销毁数据管道上下文
 * @param ctx 上下文指针
 */
void datapipe_destroy(datapipe_context_t* ctx);

/**
 * @brief 向管道写入数据（会经过编码处理）
 * @param ctx 上下文指针
 * @param data 输入数据
 * @param len 数据长度
 * @return 成功返回 DATAPIPE_OK，失败返回错误码
 */
int datapipe_write(datapipe_context_t* ctx, const uint8_t* data, size_t len);

/**
 * @brief 从管道读取数据（会经过解码处理）
 * @param ctx 上下文指针
 * @param data 输出缓冲区
 * @param len 要读取的长度
 * @return 成功返回 DATAPIPE_OK，失败返回错误码
 */
int datapipe_read(datapipe_context_t* ctx, uint8_t* data, size_t len);

/**
 * @brief 处理管道中的数据（输入 -> 编码 -> 输出）
 * @param ctx 上下文指针
 * @return 成功返回 DATAPIPE_OK，失败返回错误码
 */
int datapipe_process(datapipe_context_t* ctx);

/**
 * @brief 获取输出数据大小
 * @param ctx 上下文指针
 * @return 输出数据大小
 */
size_t datapipe_output_size(const datapipe_context_t* ctx);

/**
 * @brief 清空管道中的所有数据
 * @param ctx 上下文指针
 */
void datapipe_reset(datapipe_context_t* ctx);

/**
 * @brief 设置 XOR 密钥
 * @param ctx 上下文指针
 * @param key 密钥
 */
void datapipe_set_xor_key(datapipe_context_t* ctx, uint32_t key);

#ifdef __cplusplus
}
#endif

#endif /* DATAPIPE_CORE_H */
