/**
 * @file RingBuffer相关操作函数的实现
 * @brief 这个文件包含了用于操作RingBuffer（环形缓冲区）的一系列函数，涵盖创建、判断状态、读写数据以及清除等功能。
 */
#include "ringbuffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 假设这里有MIN宏定义用于取最小值，若实际不存在需补充合适的实现
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * @brief 创建一个RingBuffer（环形缓冲区）实例。
 *
 * @param capacity 环形缓冲区的容量大小，以字节为单位。
 * @return RingBuffer 返回创建好的RingBuffer结构体实例，如果内存分配失败则返回的结构体中相关成员可能处于未初始化的无效状态（需根据实际使用情况处理错误）。
 */
RingBuffer RingBufferCreate(size_t capacity)
{
    RingBuffer rbuf;
    rbuf.buffer = (uint8_t *)malloc(capacity);
    if (!rbuf.buffer)
    {
        fprintf(stderr, "Memory allocation failed for RingBuffer.\n");
        return;
    }
    rbuf.capacity = capacity;
    rbuf.head = 0;
    rbuf.tail = 0;
    rbuf.count = 0;

    return rbuf;
}

/**
 * @brief 判断给定的RingBuffer是否已满。
 *
 * @param rbuf 指向要检查的RingBuffer结构体的指针。
 * @return bool 如果RingBuffer中的元素数量等于其容量，则返回true，表示已满；否则返回false。
 */
bool RingBufferIsFull(const RingBuffer *rbuf)
{
    return rbuf->count == rbuf->capacity;
}

/**
 * @brief 判断给定的RingBuffer是否为空。
 *
 * @param rbuf 指向要检查的RingBuffer结构体的指针。
 * @return bool 如果RingBuffer中的元素数量为0，则返回true，表示为空；否则返回false。
 */
bool RingBufferIsEmpty(const RingBuffer *rbuf)
{
    return rbuf->count == 0;
}

/**
 * @brief 获取给定RingBuffer中当前存储的元素数量（已使用的空间大小）。
 *
 * @param rbuf 指向要获取元素数量的RingBuffer结构体的指针。
 * @return size_t 返回RingBuffer中当前存储的元素数量（字节数）。
 */
size_t RingBufferSize(const RingBuffer *rbuf)
{
    return rbuf->count;
}

/**
 * @brief 向RingBuffer（环形缓冲区）中写入数据。
 *
 * @param rbuf 指向要写入数据的RingBuffer结构体的指针。
 * @param data 指向要写入的数据缓冲区的指针，数据从此处获取。
 * @param len 要写入的数据长度（字节数）。
 * @return size_t 实际成功写入到RingBuffer中的字节数，如果缓冲区已满或者要写入的长度超过容量等情况导致无法写入，则返回0。
 */
size_t RingBufferWriteData(RingBuffer *rbuf, uint8_t *data, size_t len)
{
    if (len >= rbuf->capacity)
    {
        return 0;
    }
    if (is_rbuf_full(rbuf))
    {
        return 0; // 缓冲区已满，未写入字节.
    }

    size_t space_left = rbuf->capacity - rbuf->count;
    size_t bytes_to_write = MIN(len, space_left);

    // 检查写操作是否会穿过缓冲区的末端
    if (rbuf->head + bytes_to_write <= rbuf->capacity)
    {
        memcpy(&rbuf->buffer[rbuf->head], data, bytes_to_write);
        rbuf->head = (rbuf->head + bytes_to_write) % rbuf->capacity;
    }
    else
    {
        // 写入缓冲区末尾
        size_t bytes_to_end = rbuf->capacity - rbuf->head;
        memcpy(&rbuf->buffer[rbuf->head], data, bytes_to_end);
        // 绕一圈，从头开始继续写入
        memcpy(rbuf->buffer, data + bytes_to_end, bytes_to_write - bytes_to_end);
        rbuf->head = bytes_to_write - bytes_to_end;
    }

    rbuf->count += bytes_to_write;
    return bytes_to_write;
}

/**
 * @brief 从RingBuffer（环形缓冲区）中读取数据。
 *
 * @param rbuf 指向要读取数据的RingBuffer结构体的指针。
 * @param data 指向用于存放读取出来的数据的缓冲区的指针，数据将被复制到此缓冲区。
 * @param len 期望读取的数据长度（字节数）。
 * @return size_t 实际成功从RingBuffer中读取出来的字节数，如果缓冲区为空或者期望读取长度超过容量等情况导致无法读取，则返回0。
 */
size_t RingBufferReadData(RingBuffer *rbuf, uint8_t *data, size_t len)
{
    if (rbuf->count == 0)
    {
        return 0; // 缓冲区为空，没有读取到字节
    }
    if (len >= rbuf->capacity)
    {
        return 0;
    }
    size_t bytes_to_read = MIN(len, rbuf->count);

    // 检查读操作是否会穿过缓冲区的末尾
    if (rbuf->tail + bytes_to_read <= rbuf->capacity)
    {
        memcpy(data, &rbuf->buffer[rbuf->tail], bytes_to_read);
        rbuf->tail = (rbuf->tail + bytes_to_read) % rbuf->capacity;
    }
    else
    {
        // 读到缓冲区的末尾
        size_t bytes_to_end = rbuf->capacity - rbuf->tail;
        memcpy(data, &rbuf->buffer[rbuf->tail], bytes_to_end);
        // 绕一圈，从头开始继续读取
        memcpy(data + bytes_to_end, rbuf->buffer, bytes_to_read - bytes_to_end);
        rbuf->tail = bytes_to_read - bytes_to_end;
    }

    rbuf->count -= bytes_to_read;
    return bytes_to_read;
}

/**
 * @brief 清除RingBuffer（环形缓冲区），释放其内部已分配的缓冲区内存，并重置相关成员变量。
 *
 * @param rbuf 指向要清除的RingBuffer结构体的指针。
 */
void RingBufferClear(RingBuffer *rbuf)
{
    free(rbuf->buffer);
    rbuf->buffer = NULL;
    rbuf->capacity = 0;
    rbuf->head = 0;
    rbuf->tail = 0;
    rbuf->count = 0;
}