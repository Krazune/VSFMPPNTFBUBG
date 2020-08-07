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
#include "math.h"

#include "perlin_noise_generator.h"
#include "point_matrix.h"

static PointMatrix *generateRandomDirectionMatrix(int width, int height, int cellSize);

static float getPerlinNoisePointValue(float relativeRow, float relativeColumn, Point topLeftDirection, Point topRightDirection, Point bottomRightDirection, Point bottomLeftDirection);

static float fade(float t);

static float linearInterpolation(float valueA, float valueB, float weight);

FloatMatrix *generatePerlinNoiseMatrix(int width, int height, int cellSize)
{
	PointMatrix *randomDirectionMatrix = generateRandomDirectionMatrix(width, height, cellSize);
	FloatMatrix *noiseMatrix = createFloatMatrix(width, height);

	for (int row = 0; row < height; ++row)
	{
		for (int column = 0; column < width; ++column)
		{
			float relativeRow = row % cellSize / (float)cellSize;
			float relativeColumn = column % cellSize / (float)cellSize;

			int topLeftDirectionRow = row / cellSize;
			int topLeftDirectionColumn = column / cellSize;

			Point topLeftDirection = getMatrixPoint(randomDirectionMatrix, topLeftDirectionRow, topLeftDirectionColumn);
			Point topRightDirection = getMatrixPoint(randomDirectionMatrix, topLeftDirectionRow, topLeftDirectionColumn + 1);
			Point bottomRightDirection = getMatrixPoint(randomDirectionMatrix, topLeftDirectionRow + 1, topLeftDirectionColumn + 1);
			Point bottomLeftDirection = getMatrixPoint(randomDirectionMatrix, topLeftDirectionRow + 1, topLeftDirectionColumn);

			float pointValue = getPerlinNoisePointValue(relativeRow, relativeColumn, topLeftDirection, topRightDirection, bottomRightDirection, bottomLeftDirection);

			setMatrixFloat(noiseMatrix, row, column, pointValue);
		}
	}

	freePointMatrix(randomDirectionMatrix);

	return noiseMatrix;
}

static PointMatrix *generateRandomDirectionMatrix(int width, int height, int cellSize)
{
	Point possibleDirections[4];

	possibleDirections[0].x = 1;
	possibleDirections[0].y = 1;

	possibleDirections[1].x = -1;
	possibleDirections[1].y = -1;

	possibleDirections[2].x = -1;
	possibleDirections[2].y = 1;

	possibleDirections[3].x = 1;
	possibleDirections[3].y = -1;

	int directionMatrixWidth = ceil((float)width / cellSize) + 1;
	int directionMatrixHeight = ceil((float)height / cellSize) + 1;

	PointMatrix *directionMatrix = createPointMatrix(directionMatrixWidth, directionMatrixHeight);

	for (int row = 0; row < directionMatrixHeight; ++row)
	{
		for (int column = 0; column < directionMatrixWidth; ++column)
		{
			int randomDirectionIndex = rand() % 4;

			Point point;

			point.x = possibleDirections[randomDirectionIndex].x;
			point.y = possibleDirections[randomDirectionIndex].y;

			setMatrixPoint(directionMatrix, row, column, point);
		}
	}

	return directionMatrix;
}

static float getPerlinNoisePointValue(float relativeRow, float relativeColumn, Point topLeftDirection, Point topRightDirection, Point bottomRightDirection, Point bottomLeftDirection)
{
	Point topLeftVector = { relativeRow, relativeColumn };
	Point topRightVector = { relativeRow, relativeColumn - 1 };
	Point bottomRightVector = { relativeRow - 1, relativeColumn - 1 };
	Point bottomLeftVector = { relativeRow - 1, relativeColumn };

	float topLeftInfluence = dotProduct(topLeftVector, topLeftDirection);
	float topRightInfluence = dotProduct(topRightVector, topRightDirection);
	float bottomRightInfluence = dotProduct(bottomRightVector, bottomRightDirection);
	float bottomLeftInfluence = dotProduct(bottomLeftVector, bottomLeftDirection);

	float fadedRow = fade(relativeRow);
	float fadedColumn = fade(relativeColumn);

	float topInterpolation = linearInterpolation(topLeftInfluence, topRightInfluence, fadedColumn);
	float bottomInterpolation = linearInterpolation(bottomLeftInfluence, bottomRightInfluence, fadedColumn);
	float finalInterpolation = linearInterpolation(topInterpolation, bottomInterpolation, fadedRow);

	return finalInterpolation;
}

static float fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10); // Fade function as defined by Ken Perlin (6t^5 - 15t^4 + 10t^3).
}

static float linearInterpolation(float valueA, float valueB, float weight)
{
	return valueA + weight * (valueB - valueA);
}
