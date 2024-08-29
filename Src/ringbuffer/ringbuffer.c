#include "ringbuffer.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static bool is_rbuf_full(const RingBuffer *self);
static bool is_rbuf_empty(const RingBuffer *self);
static size_t get_size(const RingBuffer *self);
static size_t write_rbuf(RingBuffer *self, uint8_t *data, size_t len);
static size_t read_rbuf(RingBuffer *self, uint8_t *data, size_t len);
static void clear_rbuf(RingBuffer *self);

RingBuffer *newRingBuffer(size_t capacity)
{
    RingBuffer *self = malloc(sizeof(RingBuffer));
    self->buffer = (uint8_t *)malloc(capacity);
    if (!self->buffer)
    {
        // printf("Memory allocation failed.\n");
        return;
    }
    self->capacity = capacity;
    self->head = 0;
    self->tail = 0;
    self->count = 0;

    self->is_full = is_rbuf_full;
    self->is_empty = is_rbuf_empty;
    self->size = get_size;
    self->write = write_rbuf;
    self->read = read_rbuf;
    self->clear = clear_rbuf;
    return self;
}

void freeRingBuffer(RingBuffer *self)
{
    free(self->buffer);
    free(self);
}

RingBuffer RingBuffer_Create(size_t capacity)
{
    RingBuffer self;
    self.buffer = (uint8_t *)malloc(capacity);
    if (!self.buffer)
    {
        // printf("Memory allocation failed.\n");
        return;
    }
    self.capacity = capacity;
    self.head = 0;
    self.tail = 0;
    self.count = 0;

    self.is_full = is_rbuf_full;
    self.is_empty = is_rbuf_empty;
    self.size = get_size;
    self.write = write_rbuf;
    self.read = read_rbuf;
    self.clear = clear_rbuf;
    return self;
}

static bool is_rbuf_full(const RingBuffer *self)
{
    return self->count == self->capacity;
}

static bool is_rbuf_empty(const RingBuffer *self)
{
    return self->count == 0;
}

static size_t get_size(const RingBuffer *self)
{
    return self->count;
}

// 写入数据到缓冲区
static size_t write_rbuf(RingBuffer *self, uint8_t *data, size_t len)
{
    if (len >= self->capacity)
    {
        return 0;
    }
    if (is_rbuf_full(self))
    {
        return 0; // Buffer is full, no bytes written.
    }

    size_t space_left = self->capacity - self->count;
    size_t bytes_to_write = MIN(len, space_left);

    // Check if the write operation will cross the end of the buffer
    if (self->head + bytes_to_write <= self->capacity)
    {
        memcpy(&self->buffer[self->head], data, bytes_to_write);
        self->head = (self->head + bytes_to_write) % self->capacity;
    }
    else
    {
        // Write to the end of the buffer
        size_t bytes_to_end = self->capacity - self->head;
        memcpy(&self->buffer[self->head], data, bytes_to_end);
        // Wrap around and continue writing from the beginning
        memcpy(self->buffer, data + bytes_to_end, bytes_to_write - bytes_to_end);
        self->head = bytes_to_write - bytes_to_end;
    }

    self->count += bytes_to_write;
    return bytes_to_write;
}

// 从缓冲区读取数据
static size_t read_rbuf(RingBuffer *self, uint8_t *data, size_t len)
{
    if (self->count == 0)
    {
        return 0; // Buffer is empty, no bytes read.
    }
    if (len >= self->capacity)
    {
        return 0;
    }
    size_t bytes_to_read = MIN(len, self->count);

    // Check if the read operation will cross the end of the buffer
    if (self->tail + bytes_to_read <= self->capacity)
    {
        memcpy(data, &self->buffer[self->tail], bytes_to_read);
        self->tail = (self->tail + bytes_to_read) % self->capacity;
    }
    else
    {
        // Read to the end of the buffer
        size_t bytes_to_end = self->capacity - self->tail;
        memcpy(data, &self->buffer[self->tail], bytes_to_end);
        // Wrap around and continue reading from the beginning
        memcpy(data + bytes_to_end, self->buffer, bytes_to_read - bytes_to_end);
        self->tail = bytes_to_read - bytes_to_end;
    }

    self->count -= bytes_to_read;
    return bytes_to_read;
}

static void clear_rbuf(RingBuffer *self)
{
    free(self->buffer);
    self->buffer = NULL;
    self->capacity = 0;
    self->head = 0;
    self->tail = 0;
    self->count = 0;
}
