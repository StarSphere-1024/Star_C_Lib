#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>
#include <stdint.h>
#define size_t uint64_t

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

typedef struct Queue_Fuc_t
{
    Node *(*newNode)(void *data);
    Queue *(*createQueue)(void);
    void (*append)(Queue *q, void *data, size_t data_size);
    void *(*pop)(Queue *q);
    bool (*empty)(Queue *q);
    void *(*peek)(Queue *q);
    size_t (*getSize)(Queue *q);
} Queue_Fuc_t;

#else

#define MAX_QUEUE_SIZE 128 // 定义队列最大容量

typedef struct Queue
{
    void *data[MAX_QUEUE_SIZE]; // 存储数据的数组
    size_t front;                  // 队头指针
    size_t rear;                   // 队尾指针
    size_t size;                   // 队列当前大小
} Queue;

typedef struct Queue_Fuc_t
{
    Queue *(*createQueue)(void);
    void (*append)(Queue *q, void *data, size_t data_size);
    void *(*pop)(Queue *q);
    bool (*empty)(Queue *q);
    size_t (*size)(Queue *q);
    bool (*isFull)(Queue *q);
} Queue_Fuc_t;

#endif
extern const Queue_Fuc_t Queue_Fuc;

#endif
