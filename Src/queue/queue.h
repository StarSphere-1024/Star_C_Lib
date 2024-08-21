#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>
#include <stdint.h>

#define QUEUE_TYPE_LIST;

#ifdef QUEUE_TYPE_LIST
// 定义队列节点结构体
typedef struct Node
{
    void *data;
    struct Node *next;
} Node;

// 定义队列结构体
typedef struct Queue
{
    Node *front;
    Node *rear;
} Queue;

// 创建一个新的队列节点
Node *QueueNewNode(void *data);
// 创建一个新的队列
bool QueueCreate(Queue *this);
// 将一个元素添加到队列的尾部
void QueueAppend(Queue *this, void *data, size_t data_size);
// 从队列的头部移除并返回一个元素
void *QueuePop(Queue *this);
// 检查队列是否为空
bool QueueIsEmpty(Queue *this);
// 获取队列长度
size_t QueueSize(Queue *this);
// 返回队列的头部元素，但不移除它
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

// 创建一个新的队列
bool QueueCreate(Queue *this);
// 入队操作
void QueueAppend(Queue *this, void* data, size_t data_size);
// 出队操作
void* QueuePop(Queue *this);
// 检查队列是否为空
bool QueueIsEmpty(Queue *this);
// 检查队列是否已满
bool QueueIsFull(Queue *this);
// 获取队列长度
size_t QueueSize(Queue *this);

#endif

#endif
