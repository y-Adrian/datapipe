/**
 * @file datapipe.h
 * @brief DataPipe - 数据管道中间件主头文件
 * 
 * DataPipe 是一个轻量级的数据处理中间件，提供：
 * - 缓冲区管理
 * - 数据编解码
 * - 平台抽象层
 */

#ifndef DATAPIPE_H
#define DATAPIPE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 版本信息 */
#define DATAPIPE_VERSION_MAJOR 1
#define DATAPIPE_VERSION_MINOR 0
#define DATAPIPE_VERSION_PATCH 0

/* 错误码定义 */
typedef enum {
    DATAPIPE_OK = 0,
    DATAPIPE_ERR_INVALID_PARAM = -1,
    DATAPIPE_ERR_NO_MEMORY = -2,
    DATAPIPE_ERR_IO = -3,
    DATAPIPE_ERR_CODEC = -4,
    DATAPIPE_ERR_TIMEOUT = -5
} datapipe_error_t;

/* 平台检测 */
#if defined(__x86_64__) || defined(__amd64__) || defined(_M_X64)
    #define DATAPIPE_PLATFORM_X86_64
#elif defined(__aarch64__) || defined(_M_ARM64)
    #define DATAPIPE_PLATFORM_ARM64
#elif defined(__riscv) && defined(__riscv_xlen) && (__riscv_xlen == 64)
    #define DATAPIPE_PLATFORM_RISCV64
#endif

/* 导出宏 */
#if defined(_WIN32) || defined(_WIN64)
    #define DATAPIPE_EXPORT __declspec(dllexport)
    #define DATAPIPE_IMPORT __declspec(dllimport)
#else
    #define DATAPIPE_EXPORT __attribute__((visibility("default")))
    #define DATAPIPE_IMPORT
#endif

/* 获取版本字符串 */
DATAPIPE_EXPORT const char* datapipe_version(void);

/* 获取平台名称 */
DATAPIPE_EXPORT const char* datapipe_platform(void);

/* 获取构建信息 */
DATAPIPE_EXPORT const char* datapipe_build_info(void);

#ifdef __cplusplus
}
#endif

#endif /* DATAPIPE_H */
