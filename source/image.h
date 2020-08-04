#ifndef IMAGE_H
#define IMAGE_H

#include "matrix.h"
#include "color_rgb.h"

typedef Matrix Image;

Image *createImage(int width, int height);

void setImageColor(Image *image, int row, int column, ColorRGB color);

ColorRGB getImageColor(Image *image, int row, int column);

void freeImage(Image *image);

#endif
