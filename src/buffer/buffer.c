/**
 * @file buffer.c
 * @brief 缓冲区管理模块实现
 */

#include "buffer.h"
#include "datapipe.h"
#include <stdlib.h>
#include <string.h>

datapipe_buffer_t* datapipe_buffer_create(size_t initial_capacity) {
    datapipe_buffer_t* buf = (datapipe_buffer_t*)malloc(sizeof(datapipe_buffer_t));
    if (!buf) return NULL;
    
    buf->data = (uint8_t*)malloc(initial_capacity);
    if (!buf->data) {
        free(buf);
        return NULL;
    }
    
    buf->size = 0;
    buf->capacity = initial_capacity;
    return buf;
}

void datapipe_buffer_destroy(datapipe_buffer_t* buf) {
    if (!buf) return;
    if (buf->data) free(buf->data);
    free(buf);
}

int datapipe_buffer_write(datapipe_buffer_t* buf, const uint8_t* data, size_t len) {
    if (!buf || !data) return DATAPIPE_ERR_INVALID_PARAM;
    
    /* 检查是否需要扩容 */
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (new_capacity < buf->size + len) {
            new_capacity *= 2;
        }
        
        uint8_t* new_data = (uint8_t*)realloc(buf->data, new_capacity);
        if (!new_data) return DATAPIPE_ERR_NO_MEMORY;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return DATAPIPE_OK;
}

int datapipe_buffer_read(datapipe_buffer_t* buf, uint8_t* data, size_t len) {
    if (!buf || !data) return DATAPIPE_ERR_INVALID_PARAM;
    if (len > buf->size) return DATAPIPE_ERR_INVALID_PARAM;
    
    memcpy(data, buf->data, len);
    /* 移除已读取的数据 */
    if (len < buf->size) {
        memmove(buf->data, buf->data + len, buf->size - len);
    }
    buf->size -= len;
    return DATAPIPE_OK;
}

void datapipe_buffer_clear(datapipe_buffer_t* buf) {
    if (!buf) return;
    buf->size = 0;
}

size_t datapipe_buffer_size(const datapipe_buffer_t* buf) {
    return buf ? buf->size : 0;
}

size_t datapipe_buffer_capacity(const datapipe_buffer_t* buf) {
    return buf ? buf->capacity : 0;
}
