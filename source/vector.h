/*
	MIT License

	Copyright (c) 2020 Miguel Sousa

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
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
