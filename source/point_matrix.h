#ifndef POINT_MATRIX_H
#define POINT_MATRIX_H

#include "matrix.h"
#include "point.h"

typedef Matrix PointMatrix;

PointMatrix *createPointMatrix(int width, int height);

void setMatrixPoint(PointMatrix *pointMatrix, int row, int column, Point point);

Point getMatrixPoint(PointMatrix *pointMatrix, int row, int column);

void freePointMatrix(PointMatrix *pointMatrix);

#endif
