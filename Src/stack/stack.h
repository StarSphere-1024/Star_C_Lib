#ifndef __STACK_H__
#define __STACK_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct Stack
{
    size_t size, len, valueSize;
    void *data;
} Stack;

bool StackCreate(Stack *this, size_t size, size_t valueSize);

bool StackResize(Stack *this, size_t newSize);

bool StackPush(Stack *this, void *value);

void *StackPop(Stack *this);

void *StackPeek(Stack *this);

bool StackIsEmpty(Stack *this);

void StackClear(Stack *this);

size_t StackLength(Stack *this);

void StackDelete(Stack *this);

#endif

