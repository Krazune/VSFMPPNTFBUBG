#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>

#include "vector.h"

typedef struct
{
	int width;
	int height;
	Vector *elementsVector;
} Matrix;

Matrix *createMatrix(int width, int height, size_t elementSize);

void *getMatrixElementPointer(Matrix *matrix, int row, int column);

void setMatrixElement(Matrix *matrix, int row, int column, void *newElement);

void freeMatrix(Matrix *matrix);

#endif
