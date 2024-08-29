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
    bool (*is_full)(RingBuffer *self);
    bool (*is_empty)(RingBuffer *self);
    size_t (*size)(RingBuffer *self);
    size_t (*write)(RingBuffer *self, uint8_t *data, size_t len);
    size_t (*read)(RingBuffer *self, uint8_t *data, size_t len);
    void (*clear)(RingBuffer *self);
} RingBuffer;

RingBuffer RingBuffer_Create(size_t capacity);
RingBuffer* newRingBuffer(size_t capacity);
void freeRingBuffer(RingBuffer* self);

#endif 
