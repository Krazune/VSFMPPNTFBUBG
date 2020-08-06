#ifndef FLOAT_MATRIX_H
#define FLOAT_MATRIX_H

#include "matrix.h"

typedef Matrix FloatMatrix;

FloatMatrix *createFloatMatrix(int width, int height);

void setMatrixFloat(FloatMatrix *floatMatrix, int row, int column, float value);

float getMatrixFloat(FloatMatrix *floatMatrix, int row, int column);

void freeFloatMatrix(FloatMatrix *floatMatrix);

#endif
