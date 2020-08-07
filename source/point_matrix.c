#include <assert.h>

#include "point_matrix.h"
#include "matrix.h"
#include "point.h"

PointMatrix *createPointMatrix(int width, int height)
{
	assert(width > 0 && height > 0);

	return createMatrix(width, height, sizeof (PointMatrix));
}

void setMatrixPoint(PointMatrix *pointMatrix, int row, int column, Point point)
{
	setMatrixElement(pointMatrix, row, column, &point);
}

Point getMatrixPoint(PointMatrix *pointMatrix, int row, int column)
{
	Point *pointPointer = getMatrixElementPointer(pointMatrix, row, column);

	return *pointPointer;
}

void freePointMatrix(PointMatrix *pointMatrix)
{
	freeMatrix(pointMatrix);
}
