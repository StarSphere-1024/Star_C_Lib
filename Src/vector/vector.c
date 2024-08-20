#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 创建一个 Vector 并分配内存
void VectorCreate(Vector *this, size_t size, size_t valueSize)
{
    this->size = size;
    this->valueSize = valueSize;
    this->data = malloc(valueSize * size);
    if (this->data == NULL)
    {
        fprintf(stderr, "Memory allocation failed for vector data.\n");
        exit(EXIT_FAILURE);
    }
    memset(this->data, 0, valueSize * size);
    this->len = 0;
}

// 重新设置Vector大小并分配内存
void VectorResize(Vector *this, size_t newSize)
{
    void *newData = realloc(this->data, newSize * this->valueSize);
    if (newData == NULL)
    {
        fprintf(stderr, "Memory reallocation failed.\n");
        exit(EXIT_FAILURE);
    }
    this->data = newData;
    this->size = newSize;
}

// 设置 Vector 中指定索引处的值
void VectorSetValue(Vector *this, size_t index, void *value)
{
    if (index > this->size)
    {
        return;
    }
    if (index > this->len - 1)
    {
        this->len = index + 1;
    }
    memcpy(this->data + (index * this->valueSize), value, this->valueSize);
}

// 获取 Vector 中指定索引处的值
void *VectorGetValue(Vector *this, size_t index)
{
    if (index > this->len - 1 || this->data == NULL)
    {
        return NULL;
    }
    return this->data + (index * this->valueSize);
}

// 在 Vector 的末尾添加一个值
void VectorPushBack(Vector *this, void *value)
{
    if (this->len == this->size)
    {
        return;
    }
    memcpy(this->data + (this->len * this->valueSize), value, this->valueSize);
    this->len++;
}

// 删除 Vector 并释放内存
void VectorDelete(Vector *this)
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

// 判断 Vector 是否已满
bool VectorIsFull(Vector *this)
{
    return this->len == this->size;
}

bool VectorIsEmpty(Vector *this)
{
    return this->len == 0;
}

// 返回 Vector 的容量
size_t VectorSize(Vector *this)
{
    return this->size;
}

// 返回 Vector 的长度
size_t VectorLen(Vector *this)
{
    return this->len;
}
