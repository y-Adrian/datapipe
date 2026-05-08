/**
 * @file platform.h
 * @brief 平台抽象层头文件
 */

#ifndef DATAPIPE_PLATFORM_H
#define DATAPIPE_PLATFORM_H

#include <stddef.h>
#include <stdint.h>
#include "datapipe.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获取当前平台名称
 * @return 平台名称字符串
 */
DATAPIPE_EXPORT const char* datapipe_get_platform_name(void);

/**
 * @brief 获取当前架构名称
 * @return 架构名称字符串
 */
DATAPIPE_EXPORT const char* datapipe_get_arch_name(void);

/**
 * @brief 内存对齐分配
 * @param size 分配大小
 * @param alignment 对齐字节数（必须是 2 的幂）
 * @return 分配的内存指针，失败返回 NULL
 */
DATAPIPE_EXPORT void* datapipe_aligned_alloc(size_t size, size_t alignment);

/**
 * @brief 释放对齐分配的内存
 * @param ptr 内存指针
 */
DATAPIPE_EXPORT void datapipe_aligned_free(void* ptr);

/**
 * @brief 获取 CPU 核心数
 * @return CPU 核心数
 */
DATAPIPE_EXPORT int datapipe_get_cpu_cores(void);

/**
 * @brief 获取页面大小
 * @return 页面大小（字节）
 */
DATAPIPE_EXPORT size_t datapipe_get_page_size(void);

/**
 * @brief 内存屏障（用于多线程同步）
 */
DATAPIPE_EXPORT void datapipe_memory_barrier(void);

/**
 * @brief 获取高精度时间戳（纳秒）
 * @return 纳秒时间戳
 */
DATAPIPE_EXPORT uint64_t datapipe_get_time_ns(void);

#ifdef __cplusplus
}
#endif

#endif /* DATAPIPE_PLATFORM_H */
