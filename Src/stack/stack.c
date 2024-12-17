/**
 * @file 栈相关操作函数的实现
 * @brief 这个文件实现了栈（Stack）的一系列操作函数，包括创建、扩容、元素压入弹出、查看栈状态以及删除等功能，用于操作自定义的栈数据结构。
 */

#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 创建栈并为栈的数据存储区域分配内存。
 *
 * @param this 指向要创建的栈结构体的指针，通过该指针在函数内部初始化栈的各个成员变量，并分配存储数据的内存空间。
 * @param size 栈大小，表示栈最多能容纳的元素个数。
 * @param valueSize 元素大小，即每个要存储在栈中的元素所占用的字节数，用于准确分配内存空间以及后续的元素操作（如复制等）。
 * @return bool 如果内存分配成功，完成栈的创建及初始化工作，返回`true`；若内存分配失败，则输出错误提示信息到标准错误输出，并返回`false`。
 */
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

/**
 * @brief 对已创建的栈进行扩容操作，重新分配内存以增大栈的容量。
 *
 * @param this 指向要扩容的栈结构体的指针，通过该指针获取当前栈的相关信息，并在扩容成功后更新栈的成员变量。
 * @param newSize 新的栈大小，即扩容后栈最多能容纳的元素个数，新大小应大于当前栈的大小才能实现有效的扩容。
 * @return bool 如果内存重新分配（`realloc`操作）成功，更新栈的相关成员变量并返回`true`；若内存重新分配失败，则输出错误提示信息到标准错误输出，并返回`false`。
 */
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

/**
 * @brief 将一个元素压入栈顶。如果栈已满，会自动尝试进行扩容操作（扩大为当前容量的两倍），然后再压入元素。
 *
 * @param this 指向目标栈结构体的指针，将元素压入该栈的栈顶位置。
 * @param value 元素指针，指向要压入栈的元素所在的内存位置，函数会根据`valueSize`（元素大小）来复制该元素到栈顶。
 * @return bool 如果元素成功压入栈顶（包括扩容后成功压入的情况），则返回`true`；若扩容操作失败导致无法压入元素，则返回`false`。
 */
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

/**
 * @brief 从栈顶弹出一个元素，将栈顶指针下移一位，并返回弹出的元素的指针（调用者需注意根据元素类型正确使用该指针）。
 *
 * @param this 指向目标栈结构体的指针，从该栈的栈顶弹出元素。
 * @return void* 返回指向弹出的元素的指针，如果栈为空，会输出错误提示信息到标准错误输出，并返回`NULL`。
 */
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

/**
 * @brief 获取栈顶元素而不将其从栈中弹出，返回栈顶元素的指针（调用者需注意根据元素类型正确使用该指针）。
 *
 * @param this 指向目标栈结构体的指针，获取该栈的栈顶元素指针。
 * @return void* 返回指向栈顶元素的指针，如果栈为空，会输出错误提示信息到标准错误输出，并返回`NULL`。
 */
void *StackPeek(Stack *this)
{
    if (this->len == 0)
    {
        fprintf(stderr, "Stack is empty.\n");
        return NULL;
    }
    return this->data + ((this->len - 1) * this->valueSize);
}

/**
 * @brief 检查栈是否为空，即判断栈中是否没有存放任何元素。
 *
 * @param this 指向要检查的栈结构体的指针。
 * @return bool 如果栈的长度（`len`成员变量）为`0`，则返回`true`，表示栈为空；否则返回`false`。
 */
bool StackIsEmpty(Stack *this)
{
    return this->len == 0;
}

/**
 * @brief 清空栈，将栈的长度（已存放元素个数）设置为`0`，但不会释放栈的数据存储区域内存（可根据实际需求决定是否释放并重新分配内存）。
 *
 * @param this 指向要清空的栈结构体的指针。
 */
void StackClear(Stack *this)
{
    this->len = 0;
    // 可以选择释放数据并重新分配，但这不是必需的
}

/**
 * @brief 获取栈的长度，也就是栈中当前已存放的元素个数。
 *
 * @param this 指向要获取长度的栈结构体的指针。
 * @return size_t 返回栈中当前存放的元素个数（通过栈结构体的`len`成员变量获取）。
 */
size_t StackLength(Stack *this)
{
    return this->len;
}

/**
 * @brief 删除栈并释放为栈的数据存储区域分配的内存，同时将栈结构体的相关成员变量重置为初始值。
 *
 * @param this 指向要删除的栈结构体的指针，通过该指针释放内存并重置成员变量。
 */
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