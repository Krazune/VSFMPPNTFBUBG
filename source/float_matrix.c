#include <assert.h>

#include "float_matrix.h"

FloatMatrix *createFloatMatrix(int width, int height)
{
	assert(width > 0 && height > 0);

	return createMatrix(width, height, sizeof (FloatMatrix));
}

void setMatrixFloat(FloatMatrix *floatMatrix, int row, int column, float value)
{
	setMatrixElement(floatMatrix, row, column, &value);
}

float getMatrixFloat(FloatMatrix *floatMatrix, int row, int column)
{
	float *floatPointer = getMatrixElementPointer(floatMatrix, row, column);

	return *floatPointer;
}

void freeFloatMatrix(FloatMatrix *floatMatrix)
{
	freeMatrix(floatMatrix);
}
