#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#ifdef QUEUE_TYPE_LIST

Node *QueueNewNode(void *data)
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

bool QueueCreate(Queue *this)
{
    this = (Queue *)malloc(sizeof(Queue));
    if (this == NULL)
    {
        return false;
    }
    this->front = this->rear = NULL;
    return true;
}

void QueueAppend(Queue *this, void *data, size_t data_size)
{
    Node *node = QueueNewNode(data);
    node->data = malloc(data_size);
    if (!node->data)
    {
        perror("内存分配失败");
        free(node); // 释放节点内存
        exit(EXIT_FAILURE);
    }
    memcpy(node->data, data, data_size);
    node->next = NULL;

    if (this->rear == NULL)
    {
        this->front = this->rear = node;
    }
    else
    {
        this->rear->next = node;
        this->rear = node;
    }
}

void *QueuePop(Queue *this)
{
    if (this->front == NULL)
    {
        printf("队列为空\n");
        exit(1);
    }
    Node *node = this->front;
    this->front = this->front->next;
    if (this->front == NULL)
    {
        this->rear = NULL;
    }
    void *data = node->data;
    free(node);
    return data;
}

bool QueueIsEmpty(Queue *this)
{
    return (this->front == NULL);
}

size_t QueueSize(Queue *this)
{
    size_t count = 0;
    Node *current = this->front;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}

void *QueuePeek(Queue *this)
{
    if (QueueIsEmpty(this))
    {
        printf("队列为空\n");
        exit(1);
    }
    return this->front->data;
}
#else

// 创建一个新的队列
bool QueueCreate(Queue *this)
{
    this = (Queue *)malloc(sizeof(Queue));
    if (!this)
    {
        perror("内存分配失败");
        return false;
    }
    this->front = this->rear = 0;
    this->size = 0;
    return true;
}

// 入队操作
void QueueAppend(Queue *this, void *data, size_t data_size)
{
    if (QueueIsFull(this))
    {
        printf("队列已满！\n");
        return;
    }

    // 分配内存空间
    this->data[this->rear] = malloc(data_size);
    if (!this->data[this->rear])
    {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }

    // 复制数据
    memcpy(this->data[this->rear], data, data_size);

    // 更新队尾指针
    this->rear = (this->rear + 1) % MAX_QUEUE_SIZE; // 循环使用数组空间
    this->size++;
}

// 出队操作
void *QueuePop(Queue *this)
{
    if (QueueIsEmpty(this))
    {
        printf("队列为空！\n");
        return NULL;
    }

    void *data = this->data[this->front];
    this->data[this->front] = NULL; // 释放原数据指针

    // 更新队头指针
    this->front = (this->front + 1) % MAX_QUEUE_SIZE; // 循环使用数组空间
    this->size--;

    return data;
}

// 检查队列是否为空
bool QueueIsEmpty(Queue *this)
{
    return (this->size == 0);
}

// 检查队列是否已满
bool QueueIsFull(Queue *this)
{
    return (this->size == MAX_QUEUE_SIZE);
}

// 获取队列长度
size_t QueueSize(Queue *this)
{
    return this->size;
}

#endif