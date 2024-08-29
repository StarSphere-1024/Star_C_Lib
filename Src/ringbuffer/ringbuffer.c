#include "ringbuffer.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static void init_rbuf(RingBuffer *rbuf, size_t capacity);
static bool is_rbuf_full(const RingBuffer *rbuf);
static bool is_rbuf_empty(const RingBuffer *rbuf);
static size_t get_size(const RingBuffer *rbuf);
static size_t write_rbuf(RingBuffer *rbuf, uint8_t *data, size_t len);
static size_t read_rbuf(RingBuffer *rbuf, uint8_t *data, size_t len);
static void free_rbuf(RingBuffer *rbuf);

const RingBuffer_fuc_t rbuf_fuc = {
    .init = init_rbuf,
    .is_full = is_rbuf_full,
    .is_empty = is_rbuf_empty,
    .size = get_size,
    .write = write_rbuf,
    .read = read_rbuf,
    .free = free_rbuf};

static void init_rbuf(RingBuffer *rbuf, size_t capacity)
{
    rbuf->buffer = (uint8_t *)malloc(capacity);
    if (!rbuf->buffer)
    {
        // printf("Memory allocation failed.\n");
        return;
    }
    rbuf->capacity = capacity;
    rbuf->head = 0;
    rbuf->tail = 0;
    rbuf->count = 0;
}

static bool is_rbuf_full(const RingBuffer *rbuf)
{
    return rbuf->count == rbuf->capacity;
}

static bool is_rbuf_empty(const RingBuffer *rbuf)
{
    return rbuf->count == 0;
}

static size_t get_size(const RingBuffer *rbuf)
{
    return rbuf->count;
}

// 写入数据到缓冲区
static size_t write_rbuf(RingBuffer *rbuf, uint8_t *data, size_t len)
{
    if(len >=rbuf->capacity){
        return 0;
    }
    if (is_rbuf_full(rbuf)) {
        return 0; // Buffer is full, no bytes written.
    }

    size_t space_left = rbuf->capacity - rbuf->count;
    size_t bytes_to_write = MIN(len, space_left);

    // Check if the write operation will cross the end of the buffer
    if (rbuf->head + bytes_to_write <= rbuf->capacity) {
        memcpy(&rbuf->buffer[rbuf->head], data, bytes_to_write);
        rbuf->head = (rbuf->head + bytes_to_write) % rbuf->capacity;
    } else {
        // Write to the end of the buffer
        size_t bytes_to_end = rbuf->capacity - rbuf->head;
        memcpy(&rbuf->buffer[rbuf->head], data, bytes_to_end);
        // Wrap around and continue writing from the beginning
        memcpy(rbuf->buffer, data + bytes_to_end, bytes_to_write - bytes_to_end);
        rbuf->head = bytes_to_write - bytes_to_end;
    }

    rbuf->count += bytes_to_write;
    return bytes_to_write;
}

// 从缓冲区读取数据
static size_t read_rbuf(RingBuffer *rbuf, uint8_t *data, size_t len)
{
   if (rbuf->count == 0) {
        return 0; // Buffer is empty, no bytes read.
    }
    if(len >=rbuf->capacity){
        return 0;
    }
    size_t bytes_to_read = MIN(len, rbuf->count);

    // Check if the read operation will cross the end of the buffer
    if (rbuf->tail + bytes_to_read <= rbuf->capacity) {
        memcpy(data, &rbuf->buffer[rbuf->tail], bytes_to_read);
        rbuf->tail = (rbuf->tail + bytes_to_read) % rbuf->capacity;
    } else {
        // Read to the end of the buffer
        size_t bytes_to_end = rbuf->capacity - rbuf->tail;
        memcpy(data, &rbuf->buffer[rbuf->tail], bytes_to_end);
        // Wrap around and continue reading from the beginning
        memcpy(data + bytes_to_end, rbuf->buffer, bytes_to_read - bytes_to_end);
        rbuf->tail = bytes_to_read - bytes_to_end;
    }

    rbuf->count -= bytes_to_read;
    return bytes_to_read;
}

static void free_rbuf(RingBuffer *rbuf)
{
    free(rbuf->buffer);
    rbuf->buffer = NULL;
    rbuf->capacity = 0;
    rbuf->head = 0;
    rbuf->tail = 0;
    rbuf->count = 0;
}
