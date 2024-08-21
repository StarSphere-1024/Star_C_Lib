#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 创建一个 Vector 并分配内存
bool VectorCreate(Vector *this, size_t size, size_t valueSize)
{
    this->size = size;
    this->valueSize = valueSize;
    this->data = malloc(valueSize * size);
    if (this->data == NULL)
    {
        fprintf(stderr, "Memory allocation failed for vector data.\n");
        return false;
    }
    memset(this->data, 0, valueSize * size);
    this->len = 0;
    return true;
}

// 重新设置Vector大小并分配内存
bool VectorResize(Vector *this, size_t newSize)
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

// 设置 Vector 中指定索引处的值
bool VectorSetValue(Vector *this, size_t index, void *value)
{
    if (index > this->size)
    {
        fprintf(stderr, "Error: Index out of bounds in VectorSetValue.\n");
        return false;
    }
    if (index > this->len - 1)
    {
        this->len = index + 1;
    }
    memcpy(this->data + (index * this->valueSize), value, this->valueSize);
    return true;
}

// 获取 Vector 中指定索引处的值
void *VectorGetValue(Vector *this, size_t index)
{
    if (index > this->len - 1 || this->data == NULL)
    {
        fprintf(stderr, "Error: Index out of bounds in VectorGetValue.\n");
        return NULL;
    }
    return this->data + (index * this->valueSize);
}

// 在 Vector 的末尾添加一个值
bool VectorPushBack(Vector *this, void *value)
{
    if (this->len == this->size)
    {
        // 自动扩容
        size_t newSize = this->size * 2; // 扩大两倍
        if (!VectorResize(this, newSize))
        {
            // 如果扩容失败，返回 false
            return false;
        }
    }
    memcpy(this->data + (this->len * this->valueSize), value, this->valueSize);
    this->len++;
    return true;
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
