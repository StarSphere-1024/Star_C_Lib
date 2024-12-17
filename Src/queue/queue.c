/**
 * @file 队列相关操作函数的实现
 * @brief 这个文件根据不同的队列类型定义（由`QUEUE_TYPE_LIST`宏控制），实现了队列（Queue）的一系列操作函数，包括创建、添加元素、移除元素、判断队列状态以及获取队列长度等功能。
 */

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#ifdef QUEUE_TYPE_LIST

/**
 * @brief 创建一个新的队列节点。
 *
 * @param data 要存储在节点内部的数据指针，该数据将被节点持有（注意可能需要根据实际情况管理其内存生命周期）。
 * @return Node* 返回创建好的队列节点指针，如果内存分配失败，程序将输出错误信息并以错误码`EXIT_FAILURE`退出。
 */
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

/**
 * @brief 创建一个新的队列（基于链表实现的队列）。
 *
 * @param this 指向要创建的队列结构体的指针（用于接收创建好的队列实例地址）。
 * @return bool 如果队列创建成功（内存分配成功），则返回`true`；否则返回`false`。
 */
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

/**
 * @brief 将一个元素添加到基于链表实现的队列的尾部。
 *
 * @param this 指向目标队列结构体的指针，该队列是元素要添加进去的队列。
 * @param data 指向要添加到队列的数据的指针，此数据将被复制到新创建的队列节点中（需要保证其内存有效且可访问）。
 * @param data_size 要添加的数据的大小（字节数），用于准确复制数据以及为节点内数据分配合适内存。
 */
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

/**
 * @brief 从基于链表实现的队列的头部移除并返回一个元素。
 *
 * @param this 指向目标队列结构体的指针，从该队列的头部移除元素。
 * @return void* 返回移除的头部元素的数据指针，调用者需要根据实际情况正确管理该数据的内存（例如，如果是动态分配的数据，可能需要后续释放内存），如果队列为空，程序将输出提示信息并以错误码`1`退出。
 */
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

/**
 * @brief 检查基于链表实现的队列是否为空。
 *
 * @param this 指向要检查的队列结构体的指针。
 * @return bool 如果队列的头部指针为`NULL`（即没有元素），则返回`true`，表示队列为空；否则返回`false`。
 */
bool QueueIsEmpty(Queue *this)
{
    return (this->front == NULL);
}

/**
 * @brief 获取基于链表实现的队列的长度（元素个数）。
 *
 * @param this 指向要获取长度的队列结构体的指针。
 * @return size_t 返回队列中当前包含的元素个数。
 */
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

/**
 * @brief 返回基于链表实现的队列的头部元素，但不移除它。
 *
 * @param this 指向目标队列结构体的指针，获取该队列头部元素的数据指针。
 * @return void* 返回队列头部元素的数据指针，调用者需要保证队列非空，否则程序将输出提示信息并以错误码`1`退出。
 */
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

// 假设这里有MAX_QUEUE_SIZE宏定义表示队列最大容量，例如：
// #define MAX_QUEUE_SIZE 100

/**
 * @brief 创建一个新的队列（基于数组实现的队列）。
 *
 * @param this 指向要创建的队列结构体的指针（用于接收创建好的队列实例地址）。
 * @return bool 如果队列创建成功（内存分配成功），则返回`true`；否则返回`false`。
 */
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

/**
 * @brief 将一个元素添加到基于数组实现的队列的尾部。
 *
 * @param this 指向目标队列结构体的指针，该队列是元素要添加进去的队列。
 * @param data 指向要添加到队列的数据的指针，此数据将被复制到队列数组的相应位置（需要保证其内存有效且可访问）。
 * @param data_size 要添加的数据的大小（字节数），用于准确复制数据以及为队列内数据分配合适内存。
 */
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

/**
 * @brief 从基于数组实现的队列的头部移除并返回一个元素。
 *
 * @param this 指向目标队列结构体的指针，从该队列的头部移除元素。
 * @return void* 返回移除的头部元素的数据指针，调用者需要根据实际情况正确管理该数据的内存（例如，如果是动态分配的数据，可能需要后续释放内存），如果队列为空，将输出提示信息并返回`NULL`。
 */
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

/**
 * @brief 检查基于数组实现的队列是否为空。
 *
 * @param this 指向要检查的队列结构体的指针。
 * @return bool 如果队列的元素个数为`0`（即`size`成员为`0`），则返回`true`，表示队列为空；否则返回`false`。
 */
bool QueueIsEmpty(Queue *this)
{
    return (this->size == 0);
}

/**
 * @brief 检查基于数组实现的队列是否已满。
 *
 * @param this 指向要检查的队列结构体的指针。
 * @return bool 如果队列的元素个数等于预定义的最大容量（`MAX_QUEUE_SIZE`），则返回`true`，表示队列已满；否则返回`false`。
 */
bool QueueIsFull(Queue *this)
{
    return (this->size == MAX_QUEUE_SIZE);
}

/**
 * @brief 获取基于数组实现的队列的长度（元素个数）。
 *
 * @param this 指向要获取长度的队列结构体的指针。
 * @return size_t 返回队列中当前包含的元素个数（通过`size`成员获取）。
 */
size_t QueueSize(Queue *this)
{
    return this->size;
}

#endif