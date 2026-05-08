/**
 * @file buffer.h
 * @brief 缓冲区管理模块头文件
 */

#ifndef DATAPIPE_BUFFER_H
#define DATAPIPE_BUFFER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 缓冲区结构体
 */
typedef struct {
    uint8_t* data;       /* 数据指针 */
    size_t   size;       /* 当前数据大小 */
    size_t   capacity;   /* 缓冲区容量 */
} datapipe_buffer_t;

/**
 * @brief 创建缓冲区
 * @param initial_capacity 初始容量
 * @return 缓冲区指针，失败返回 NULL
 */
datapipe_buffer_t* datapipe_buffer_create(size_t initial_capacity);

/**
 * @brief 销毁缓冲区
 * @param buf 缓冲区指针
 */
void datapipe_buffer_destroy(datapipe_buffer_t* buf);

/**
 * @brief 向缓冲区写入数据
 * @param buf 缓冲区指针
 * @param data 数据指针
 * @param len 数据长度
 * @return 成功返回 DATAPIPE_OK，失败返回错误码
 */
int datapipe_buffer_write(datapipe_buffer_t* buf, const uint8_t* data, size_t len);

/**
 * @brief 从缓冲区读取数据
 * @param buf 缓冲区指针
 * @param data 输出数据指针
 * @param len 要读取的长度
 * @return 成功返回 DATAPIPE_OK，失败返回错误码
 */
int datapipe_buffer_read(datapipe_buffer_t* buf, uint8_t* data, size_t len);

/**
 * @brief 清空缓冲区
 * @param buf 缓冲区指针
 */
void datapipe_buffer_clear(datapipe_buffer_t* buf);

/**
 * @brief 获取缓冲区当前数据大小
 * @param buf 缓冲区指针
 * @return 数据大小
 */
size_t datapipe_buffer_size(const datapipe_buffer_t* buf);

/**
 * @brief 获取缓冲区容量
 * @param buf 缓冲区指针
 * @return 容量
 */
size_t datapipe_buffer_capacity(const datapipe_buffer_t* buf);

#ifdef __cplusplus
}
#endif

#endif /* DATAPIPE_BUFFER_H */
