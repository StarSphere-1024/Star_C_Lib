#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#ifdef QUEUE_TYPE_LIST
// 创建一个新的队列节点
Node * newNode(void * data);
// 创建一个新的队列
Queue *createQueue();
// 将一个元素添加到队列的尾部
void append(Queue *q, void *data, size_t data_size);
// 从队列的头部移除并返回一个元素
void *pop(Queue *q);
// 检查队列是否为空
bool empty(Queue *q);
// 获取队列长度
size_t size(Queue *q);
// 返回队列的头部元素，但不移除它
void *peek(Queue *q);

Queue_Fuc_t Queue_Fuc = {
    .newNode = newNode,
    .createQueue = createQueue,
    .append = append,
    .pop = pop,
    .empty = empty,
    .peek = peek
};


Node *newNode(void *data)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
    {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    return node;
}


Queue *createQueue()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void append(Queue *q, void *data, size_t data_size)
{
    Node *node = newNode(data);
    node->data = malloc(data_size);
    if (!node->data)
    {
        perror("内存分配失败");
        free(node); // 释放节点内存
        exit(EXIT_FAILURE);
    }
    memcpy(node->data, data, data_size);
    node->next = NULL;

    if (q->rear == NULL)
    {
        q->front = q->rear = node;
    }
    else
    {
        q->rear->next = node;
        q->rear = node;
    }
}


void *pop(Queue *q)
{
    if (q->front == NULL)
    {
        printf("队列为空\n");
        exit(1);
    }
    Node *node = q->front;
    q->front = q->front->next;
    if (q->front == NULL)
    {
        q->rear = NULL;
    }
    void *data = node->data;
    free(node);
    return data;
}


bool empty(Queue *q)
{
    return (q->front == NULL);
}


size_t size(Queue *q)
{
    size_t count = 0;
    Node *current = q->front;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}


void *peek(Queue *q)
{
    if (empty(q))
    {
        printf("队列为空\n");
        exit(1);
    }
    return q->front->data;
}
#else
// 创建一个新的队列
Queue* createQueue();
// 入队操作
void append(Queue *q, void* data, size_t data_size);
// 出队操作
void* pop(Queue *q);
// 检查队列是否为空
bool empty(Queue *q);
// 检查队列是否已满
bool isFull(Queue *q);
// 获取队列长度
size_t size(Queue *q);

const Queue_Fuc_t Queue_Fuc = {
    .createQueue = createQueue,
    .append = append,
    .pop = pop,
    .empty = empty,
    .isFull=isFull,
    .size=size
};

// 创建一个新的队列
Queue* createQueue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    if (!q) {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }
    q->front = q->rear = 0; 
    q->size = 0;
    return q;
}

// 入队操作
void append(Queue *q, void* data, size_t data_size) {
    if (isFull(q)) {
        printf("队列已满！\n");
        return;
    }

    // 分配内存空间
    q->data[q->rear] = malloc(data_size);
    if (!q->data[q->rear]) {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }

    // 复制数据
    memcpy(q->data[q->rear], data, data_size);

    // 更新队尾指针
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE; // 循环使用数组空间
    q->size++; 
}

// 出队操作
void* pop(Queue *q) {
    if (empty(q)) {
        printf("队列为空！\n");
        return NULL;
    }

    void* data = q->data[q->front];
    q->data[q->front] = NULL; // 释放原数据指针

    // 更新队头指针
    q->front = (q->front + 1) % MAX_QUEUE_SIZE; // 循环使用数组空间
    q->size--;

    return data;
}

// 检查队列是否为空
bool empty(Queue *q) {
    return (q->size == 0);
}

// 检查队列是否已满
bool isFull(Queue *q) {
    return (q->size == MAX_QUEUE_SIZE);
}

// 获取队列长度
size_t size(Queue *q) {
    return q->size;
}

#endif