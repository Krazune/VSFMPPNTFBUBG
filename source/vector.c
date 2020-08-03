#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "vector.h"

#define DEFAULT_VECTOR_ALLOCATED_CAPACITY 128 // Arbitrary value.

static Vector *createRawVector(int initialElementCount, int allocatedCapacity, size_t elementSize);

static void *resizeVectorCapacity(Vector *vector, int newAllocatedCapacity);

Vector *createVector(size_t elementSize)
{
	return createRawVector(0, DEFAULT_VECTOR_ALLOCATED_CAPACITY, elementSize);
}

Vector *createSizedVector(int initialElementCount, size_t elementSize)
{
	return createRawVector(initialElementCount, 2 * initialElementCount, elementSize);
}

int addVectorElement(Vector *vector, void *newElement)
{
	if ((vector->elementCount + 1) > vector->allocatedCapacity)
	{
		if (resizeVectorCapacity(vector, vector->allocatedCapacity * 2) == NULL)
		{
			return vector->elementCount;
		};
	}

	void *destinationPointer = getVectorElementPointer(vector, vector->elementCount);

	memcpy(destinationPointer, newElement, vector->elementSize);

	vector->elementCount++;

	return vector->elementCount;
}

void setVectorElement(Vector *vector, void *newElement, int index)
{
	memcpy(getVectorElementPointer(vector, index), newElement, vector->elementSize);
}

void *getVectorElementPointer(Vector *vector, int index)
{
	unsigned char *elementsBytes = (unsigned char *)(vector->elements);

	void *elementPointer = elementsBytes + index * vector->elementSize;

	return elementPointer;
}

bool isEmptyVector(Vector *vector)
{
	return vector->elementCount == 0;
}

int resizeVector(Vector *vector, int newCount)
{
	if (newCount > vector->allocatedCapacity)
	{
		if (resizeVectorCapacity(vector, newCount * 2) == NULL)
		{
			return vector->elementCount;
		}
	}

	vector->elementCount = newCount;

	return newCount;
}

void freeVector(Vector *vector)
{
	if (vector == NULL)
	{
		return;
	}

	free(vector->elements);
	free(vector);
}

static Vector *createRawVector(int initialElementCount, int allocatedCapacity, size_t elementSize)
{
	assert(initialElementCount <= allocatedCapacity);

	void *newElements = malloc(elementSize * allocatedCapacity);

	if (newElements == NULL)
	{
		return NULL;
	}

	Vector *newVector = malloc(sizeof (Vector));

	if (newVector == NULL)
	{
		free(newElements);

		return NULL;
	}

	newVector->elementCount = initialElementCount;
	newVector->elementSize = elementSize;
	newVector->allocatedCapacity = allocatedCapacity;
	newVector->elements = newElements;

	return newVector;
}

static void *resizeVectorCapacity(Vector *vector, int newAllocatedCapacity)
{
	void *newElementsPointer = realloc(vector->elements, newAllocatedCapacity * vector->elementSize);

	if (newElementsPointer == NULL)
	{
		return NULL;
	}

	vector->elements = newElementsPointer;

	if (newAllocatedCapacity < vector->elementCount)
	{
		vector->elementCount = newAllocatedCapacity;
	}

	vector->allocatedCapacity = newAllocatedCapacity;

	return newElementsPointer;
}
