/**
 * @file platform.c
 * @brief 平台抽象层实现
 */

#include "platform.h"
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#endif

const char* datapipe_get_platform_name(void) {
#if defined(DATAPIPE_PLATFORM_X86_64)
    return "x86_64";
#elif defined(DATAPIPE_PLATFORM_ARM64)
    return "aarch64";
#elif defined(DATAPIPE_PLATFORM_RISCV64)
    return "riscv64";
#else
    return "unknown";
#endif
}

const char* datapipe_get_arch_name(void) {
#if defined(__x86_64__) || defined(__amd64__)
    return "x86_64";
#elif defined(__aarch64__)
    return "aarch64";
#elif defined(__riscv)
    return "riscv64";
#else
    return "unknown";
#endif
}

void* datapipe_aligned_alloc(size_t size, size_t alignment) {
#if defined(_MSC_VER)
    return _aligned_malloc(size, alignment);
#else
    void* ptr = NULL;
    if (posix_memalign(&ptr, alignment, size) != 0) {
        return NULL;
    }
    return ptr;
#endif
}

void datapipe_aligned_free(void* ptr) {
#if defined(_MSC_VER)
    _aligned_free(ptr);
#else
    free(ptr);
#endif
}

int datapipe_get_cpu_cores(void) {
#ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return si.dwNumberOfProcessors;
#else
    return (int)sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

size_t datapipe_get_page_size(void) {
#ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return si.dwPageSize;
#else
    return (size_t)sysconf(_SC_PAGESIZE);
#endif
}

void datapipe_memory_barrier(void) {
#ifdef _WIN32
    MemoryBarrier();
#elif defined(__x86_64__) || defined(__amd64__)
    __asm__ __volatile__("mfence" ::: "memory");
#elif defined(__aarch64__)
    __asm__ __volatile__("dmb ish" ::: "memory");
#else
    /* 通用实现 */
    __sync_synchronize();
#endif
}

uint64_t datapipe_get_time_ns(void) {
#ifdef _WIN32
    static uint64_t frequency = 0;
    if (frequency == 0) {
        QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
    }
    uint64_t counter;
    QueryPerformanceCounter((LARGE_INTEGER*)&counter);
    return (counter * 1000000000ULL) / frequency;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
#endif
}
