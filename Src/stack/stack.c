#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 创建一个 Stack 并分配内存
bool StackCreate(Stack *this, size_t size, size_t valueSize)
{
    this->size = size;
    this->valueSize = valueSize;
    this->data = malloc(valueSize * size);
    if (this->data == NULL)
    {
        fprintf(stderr, "Memory allocation failed for stack data.\n");
        return false;
    }
    memset(this->data, 0, valueSize * size);
    this->len = 0;
    return true;
}

// 扩容 Stack
bool StackResize(Stack *this, size_t newSize)
{
    void *newData = realloc(this->data, newSize * this->valueSize);
    if (newData == NULL)
    {
        fprintf(stderr, "Memory reallocation failed.\n");
        return false;
    }
    this->data = newData;
    this->size = newSize;
    return true;
}

// 将一个元素压入栈顶
bool StackPush(Stack *this, void *value)
{
    if (this->len == this->size)
    {
         // 自动扩容
        size_t newSize = this->size * 2; // 扩大两倍
        if (!StackResize(this, newSize))
        {
            // 如果扩容失败，返回 false
            return false;
        }
    }
    memcpy(this->data + (this->len * this->valueSize), value, this->valueSize);
    this->len++;
    return true;
}

// 从栈顶弹出一个元素
void *StackPop(Stack *this)
{
    if (this->len == 0)
    {
        fprintf(stderr, "Stack is empty.\n");
        return NULL;
    }
    this->len--;
    return this->data + (this->len * this->valueSize);
}

// 获取栈顶元素而不弹出
void *StackPeek(Stack *this)
{
    if (this->len == 0)
    {
        fprintf(stderr, "Stack is empty.\n");
        return NULL;
    }
    return this->data + ((this->len - 1) * this->valueSize);
}

// 检查栈是否为空
bool StackIsEmpty(Stack *this)
{
    return this->len == 0;
}

// 清空栈
void StackClear(Stack *this)
{
    this->len = 0;
    // 可以选择释放数据并重新分配，但这不是必需的
}

// 获取栈的长度
size_t StackLength(Stack *this)
{
    return this->len;
}

// 删除 Stack 并释放内存
void StackDelete(Stack *this)
{
    if (this->data != NULL)
    {
        free(this->data);
        this->data = NULL;
        this->size = 0;
        this->len = 0;
        this->valueSize = 0;
    }
}

