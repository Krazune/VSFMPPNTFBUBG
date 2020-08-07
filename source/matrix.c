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
#include <assert.h>
#include <stdlib.h>

#include "matrix.h"
#include "vector.h"

static int getMatrixIndex(int row, int column, int width);

Matrix *createMatrix(int width, int height, size_t elementSize)
{
	assert(width > 0 && height > 0);

	Vector *newElementsVector = createSizedVector(width * height, elementSize);

	if (newElementsVector == NULL)
	{
		return NULL;
	}

	Matrix *newMatrix = malloc(sizeof (Matrix));

	if (newMatrix == NULL)
	{
		freeVector(newElementsVector);

		return NULL;
	}

	newMatrix->width = width;
	newMatrix->height = height;
	newMatrix->elementsVector = newElementsVector;

	return newMatrix;
}

void *getMatrixElementPointer(Matrix *matrix, int row, int column)
{
	int elementIndex = getMatrixIndex(row, column, matrix->width);

	return getVectorElementPointer(matrix->elementsVector, elementIndex);
}

void setMatrixElement(Matrix *matrix, int row, int column, void *newElement)
{
	int elementIndex = getMatrixIndex(row, column, matrix->width);

	setVectorElement(matrix->elementsVector, newElement, elementIndex);
}

void freeMatrix(Matrix *matrix)
{
	if (matrix == NULL)
	{
		return;
	}

	freeVector(matrix->elementsVector);
	free(matrix);
}

static int getMatrixIndex(int row, int column, int width)
{
	return row * width + column;
}
