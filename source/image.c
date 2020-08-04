#include <assert.h>

#include "image.h"
#include "matrix.h"
#include "color_rgb.h"

Image *createImage(int width, int height)
{
	assert(width > 0 && height > 0);

	return createMatrix(width, height, sizeof (ColorRGB));
}

void setImageColor(Image *image, int row, int column, ColorRGB color)
{
	setMatrixElement(image, row, column, &color);
}

ColorRGB getImageColor(Image *image, int row, int column)
{
	ColorRGB *color = getMatrixElementPointer(image, row, column);

	return *color;
}

void freeImage(Image *image)
{
	freeMatrix(image);
}
