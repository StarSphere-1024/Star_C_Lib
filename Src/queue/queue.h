#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>
#include <stdint.h>

#define QUEUE_TYPE_LIST;

#ifdef QUEUE_TYPE_LIST

typedef struct Node
{
    void *data;
    struct Node *next;
} Node;


typedef struct Queue
{
    Node *front;
    Node *rear;
} Queue;


Node *QueueNewNode(void *data);

bool QueueCreate(Queue *this);

void QueueAppend(Queue *this, void *data, size_t data_size);

void *QueuePop(Queue *this);

bool QueueIsEmpty(Queue *this);

size_t QueueSize(Queue *this);

void *QueuePeek(Queue *this);

#else

#define MAX_QUEUE_SIZE 128 // 定义队列最大容量

typedef struct Queue
{
    void *data[MAX_QUEUE_SIZE]; // 存储数据的数组
    size_t front;                  // 队头指针
    size_t rear;                   // 队尾指针
    size_t size;                   // 队列当前大小
} Queue;


bool QueueCreate(Queue *this);

void QueueAppend(Queue *this, void* data, size_t data_size);

void* QueuePop(Queue *this);

bool QueueIsEmpty(Queue *this);

bool QueueIsFull(Queue *this);

size_t QueueSize(Queue *this);

#endif

#endif
