#ifndef __VECTOR_H__
#define __VECTOR_H__


#include <stdbool.h>
#include <stdint.h>

typedef struct Vector
{
    size_t size, len, valueSize;
    void *data;
} Vector;

bool VectorCreate(Vector *this, size_t size, size_t valueSize);

bool VectorResize(Vector *this, size_t newSize);

bool VectorSetValue(Vector *this, size_t index, void *value);

void *VectorGetValue(Vector *this, size_t index);

bool VectorPushBack(Vector *this, void *value);

void VectorDelete(Vector *this);

bool VectorIsFull(Vector *this);

size_t VectorSize(Vector *this);

size_t VectorLen(Vector *this);

#endif


