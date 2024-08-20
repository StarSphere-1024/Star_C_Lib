#ifndef __VECTOR_H__
#define __VECTOR_H__


#include <stdbool.h>
#include <stdint.h>

typedef struct Vector
{
    size_t size, len, valueSize;
    void *data;
} Vector;

void VectorCreate(Vector *this, size_t size, size_t valueSize);

void VectorResize(Vector *this, size_t newSize);

void VectorSetValue(Vector *this, size_t index, void *value);

void *VectorGetValue(Vector *this, size_t index);

void VectorPushBack(Vector *this, void *value);

void VectorDelete(Vector *this);

bool VectorIsFull(Vector *this);

size_t VectorSize(Vector *this);

size_t VectorLen(Vector *this);

#endif


