#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct RingBuffer_t
{
    uint8_t *buffer;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
} RingBuffer;

typedef struct RingBuffer_fuc_t
{
    void (*init)(RingBuffer *rbuf, size_t capacity);
    bool (*is_full)(const RingBuffer *rbuf);
    bool (*is_empty)(const RingBuffer *rbuf);
    size_t (*size)(const RingBuffer *rbuf);
    size_t (*write)(RingBuffer *rbuf, uint8_t *data, size_t len);
    size_t (*read)(RingBuffer *rbuf, uint8_t *data, size_t len);
    void (*free)(RingBuffer *rbuf);
} RingBuffer_fuc_t;

extern const RingBuffer_fuc_t rbuf_fuc;
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}

#endif

#endif /* __RINGBUF_H_ */
