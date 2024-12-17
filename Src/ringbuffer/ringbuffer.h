#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

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

RingBuffer RingBufferCreate(size_t capacity);

bool RingBufferIsFull(const RingBuffer *rbuf);

bool RingBufferIsEmpty(const RingBuffer *rbuf);

size_t RingBufferSize(const RingBuffer *rbuf);

size_t RingBufferWriteData(RingBuffer *rbuf, uint8_t *data, size_t len);

size_t RingBufferReadData(RingBuffer *rbuf, uint8_t *data, size_t len);

void RingBufferClear(RingBuffer *rbuf);

#endif
