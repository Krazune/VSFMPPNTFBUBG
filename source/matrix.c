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
