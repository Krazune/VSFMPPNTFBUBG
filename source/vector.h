#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
	int elementCount;
	int allocatedCapacity;
	size_t elementSize;
	void *elements;
} Vector;

Vector *createVector(size_t elementSize);
Vector *createSizedVector(int initialElementCount, size_t elementSize);

int addVectorElement(Vector *vector, void *newElement);

void setVectorElement(Vector *vector, void *newElement, int index);

void *getVectorElementPointer(Vector *vector, int index);

bool isEmptyVector(Vector *vector);

int resizeVector(Vector *vector, int newCount);

void freeVector(Vector *vector);

#endif
