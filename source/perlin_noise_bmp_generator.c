#include <stdio.h>

#include "perlin_noise_bmp_generator.h"
#include "perlin_noise_generator.h"
#include "image.h"
#include "bmp_generator.h"

static void convertPerlinMatrixToImage(FloatMatrix *perlinNoiseMatrix, Image *image);

FILE *generatePerlinNoiseBMP(char fileName[], int width, int height, int cellSize)
{
	FloatMatrix *perlinNoiseMatrix = generatePerlinNoiseMatrix(width, height, cellSize);

	if (perlinNoiseMatrix == NULL)
	{
		return NULL;
	}

	Image *noiseImage = createImage(width, height);

	if (noiseImage == NULL)
	{
		freeFloatMatrix(perlinNoiseMatrix);

		return NULL;
	}

	convertPerlinMatrixToImage(perlinNoiseMatrix, noiseImage);

	freeFloatMatrix(perlinNoiseMatrix);

	FILE *bmp = generateBMP(fileName, noiseImage);

	freeImage(noiseImage);

	return bmp;
}

static void convertPerlinMatrixToImage(FloatMatrix *perlinNoiseMatrix, Image *image)
{
	for (int row = 0; row < image->height; ++row)
	{
		for (int column = 0; column < image->width; ++column)
		{
			float pointValue = (getMatrixFloat(perlinNoiseMatrix, row, column) + 1) / 2; // Perlin noise is generated in [-1, 1] range, so it must be changed into [0, 1].
			int8_t colorValue = pointValue * 255;

			ColorRGB newColor = { colorValue, colorValue, colorValue };

			setImageColor(image, row, column, newColor);
		}
	}
}
