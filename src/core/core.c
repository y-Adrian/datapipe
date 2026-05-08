/**
 * @file core.c
 * @brief 核心处理模块实现
 */

#include "core.h"
#include <stdlib.h>
#include <string.h>

datapipe_context_t* datapipe_create(datapipe_codec_type_t codec) {
    datapipe_context_t* ctx = (datapipe_context_t*)malloc(sizeof(datapipe_context_t));
    if (!ctx) return NULL;
    
    ctx->input = datapipe_buffer_create(1024);
    if (!ctx->input) {
        free(ctx);
        return NULL;
    }
    
    ctx->output = datapipe_buffer_create(1024);
    if (!ctx->output) {
        datapipe_buffer_destroy(ctx->input);
        free(ctx);
        return NULL;
    }
    
    ctx->codec = codec;
    ctx->xor_key = 0xDEADBEEF;
    
    return ctx;
}

void datapipe_destroy(datapipe_context_t* ctx) {
    if (!ctx) return;
    if (ctx->input) datapipe_buffer_destroy(ctx->input);
    if (ctx->output) datapipe_buffer_destroy(ctx->output);
    free(ctx);
}

int datapipe_write(datapipe_context_t* ctx, const uint8_t* data, size_t len) {
    if (!ctx || !data) return DATAPIPE_ERR_INVALID_PARAM;
    return datapipe_buffer_write(ctx->input, data, len);
}

int datapipe_read(datapipe_context_t* ctx, uint8_t* data, size_t len) {
    if (!ctx || !data) return DATAPIPE_ERR_INVALID_PARAM;
    return datapipe_buffer_read(ctx->output, data, len);
}

int datapipe_process(datapipe_context_t* ctx) {
    if (!ctx || !ctx->input || !ctx->output) return DATAPIPE_ERR_INVALID_PARAM;
    if (ctx->input->size == 0) return DATAPIPE_OK;
    
    size_t encoded_len = datapipe_encoded_size(ctx->codec, ctx->input->size);
    uint8_t* encoded = (uint8_t*)malloc(encoded_len);
    if (!encoded) return DATAPIPE_ERR_NO_MEMORY;
    
    int ret = datapipe_encode(ctx->codec, ctx->input->data, ctx->input->size,
                               encoded, &encoded_len);
    if (ret == DATAPIPE_OK) {
        ret = datapipe_buffer_write(ctx->output, encoded, encoded_len);
    }
    
    free(encoded);
    if (ret == DATAPIPE_OK) {
        datapipe_buffer_clear(ctx->input);
    }
    return ret;
}

size_t datapipe_output_size(const datapipe_context_t* ctx) {
    return ctx ? datapipe_buffer_size(ctx->output) : 0;
}

void datapipe_reset(datapipe_context_t* ctx) {
    if (!ctx) return;
    datapipe_buffer_clear(ctx->input);
    datapipe_buffer_clear(ctx->output);
}

void datapipe_set_xor_key(datapipe_context_t* ctx, uint32_t key) {
    if (!ctx) return;
    ctx->xor_key = key;
}
