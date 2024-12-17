/**
 * @file vector.h 相关函数实现
 * @brief 此文件包含了对自定义向量（Vector）数据结构进行操作的一系列函数，涵盖创建、调整大小、设置与获取元素值、添加元素、判断状态以及释放内存等功能。
 */

#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 创建一个向量（Vector）实例并为其数据存储区域分配内存。
 *
 * @param this 指向要创建的向量结构体的指针，通过该指针在函数内部初始化向量的各个成员变量，如容量（size）、元素大小（valueSize）以及已存储元素个数（len）等，并分配用于存储数据的内存空间。
 * @param size 向量的初始容量大小，即最多能容纳的元素个数，以元素个数为单位进行计量。
 * @param valueSize 每个元素所占用的字节数，用于准确分配内存空间以及后续对元素进行复制等操作，确保内存操作的正确性。
 * @return bool 如果内存分配成功，完成向量的创建及初始化工作，将返回`true`；若内存分配失败，则输出错误提示信息到标准错误输出，并返回`false`。
 */
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

/**
 * @brief 重新设置向量（Vector）的大小，并根据新的大小重新分配内存。
 *
 * @param this 指向要调整大小的向量结构体的指针，通过该指针获取当前向量的相关信息（如原数据指针、元素大小等），并在内存重新分配成功后更新向量的容量（size）等成员变量。
 * @param newSize 新的向量大小，即调整后向量最多能容纳的元素个数，新大小可以大于或小于当前的容量大小，用于按需改变向量的存储能力。
 * @return bool 如果内存重新分配（`realloc`操作）成功，更新向量的相关成员变量并返回`true`；若内存重新分配失败，则输出错误提示信息到标准错误输出，并返回`false`。
 */
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

/**
 * @brief 在向量（Vector）中设置指定索引处的值，若索引超出当前已存储元素个数范围，可能会自动更新已存储元素个数（len）。
 *
 * @param this 指向目标向量结构体的指针，要在该向量的指定索引位置设置元素值。
 * @param index 要设置值的元素的索引，从`0`开始计数，用于定位向量中的具体位置，但需注意索引不能超出向量的容量（size）范围，否则会报错并返回`false`。
 * @param value 指向要设置的值所在内存位置的指针，函数会根据元素大小（valueSize）将该值复制到向量中指定索引对应的内存位置。
 * @return bool 如果索引合法且值设置成功，则返回`true`；若索引超出向量容量范围，会输出错误提示信息到标准错误输出，并返回`false`。
 */
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

/**
 * @brief 获取向量（Vector）中指定索引处的元素值，若索引超出已存储元素个数范围或者向量数据指针为`NULL`，则返回`NULL`并输出错误提示信息。
 *
 * @param this 指向目标向量结构体的指针，从该向量中获取指定索引位置的元素值。
 * @param index 要获取值的元素的索引，从`0`开始计数，用于定位向量中的具体位置，但需注意索引不能超出已存储元素个数（len）范围，否则会报错并返回`NULL`。
 * @return void* 返回指向指定索引处元素值的指针，如果索引合法且向量数据存在，则返回对应内存位置的指针；若索引超出范围或向量数据为空，会输出错误提示信息到标准错误输出，并返回`NULL`。
 */
void *VectorGetValue(Vector *this, size_t index)
{
    if (index > this->len - 1 || this->data == NULL)
    {
        fprintf(stderr, "Error: Index out of bounds in VectorGetValue.\n");
        return NULL;
    }
    return this->data + (index * this->valueSize);
}

/**
 * @brief 在向量（Vector）的末尾添加一个元素值，若向量已满，会自动进行扩容操作（扩大为当前容量的两倍）后再添加元素。
 *
 * @param this 指向目标向量结构体的指针，将元素添加到该向量的末尾位置。
 * @param value 指向要添加的元素值所在内存位置的指针，函数会根据元素大小（valueSize）将该值复制到向量末尾的内存位置。
 * @return bool 如果元素成功添加到向量末尾（包括扩容后成功添加的情况），则返回`true`；若扩容操作失败导致无法添加元素，则返回`false`。
 */
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

/**
 * @brief 删除向量（Vector）并释放为其数据存储区域分配的内存，同时将向量结构体的相关成员变量重置为初始值，以完成资源的回收和清理工作。
 *
 * @param this 指向要删除的向量结构体的指针，通过该指针释放内存并重置成员变量。
 */
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

/**
 * @brief 判断向量（Vector）是否已满，即已存储元素个数是否等于其容量大小。
 *
 * @param this 指向要检查的向量结构体的指针。
 * @return bool 如果向量中已存储元素个数（len）等于其容量大小（size），则返回`true`，表示向量已满；否则返回`false`。
 */
bool VectorIsFull(Vector *this)
{
    return this->len == this->size;
}

/**
 * @brief 判断向量（Vector）是否为空，即已存储元素个数是否为`0`。
 *
 * @param this 指向要检查的向量结构体的指针。
 * @return bool 如果向量中已存储元素个数（len）为`0`，则返回`true`，表示向量为空；否则返回`false`。
 */
bool VectorIsEmpty(Vector *this)
{
    return this->len == 0;
}

/**
 * @brief 返回向量（Vector）的容量大小，即最多能容纳的元素个数。
 *
 * @param this 指向要获取容量的向量结构体的指针。
 * @return size_t 返回向量的容量大小（通过向量结构体的`size`成员变量获取）。
 */
size_t VectorSize(Vector *this)
{
    return this->size;
}

/**
 * @brief 返回向量（Vector）的长度，也就是当前已存储的元素个数。
 *
 * @param this 指向要获取长度的向量结构体的指针。
 * @return size_t 返回向量的长度（通过向量结构体的`len`成员变量获取）。
 */
size_t VectorLen(Vector *this)
{
    return this->len;
}