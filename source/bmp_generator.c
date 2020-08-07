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
#include <stdio.h>
#include <stdint.h>

#include "bmp_generator.h"
#include "image.h"
#include "color_rgb.h"

#define PIXEL_DATA_OFFSET 54
#define DIB_HEADER_SIZE 40
#define BYTES_PER_PIXEL 3

static size_t writeBMPIdentifier(FILE *stream);
static size_t writeSize(FILE *stream, Image *image);
static int32_t getBMPSize(Image *image);
static int getRowPaddingQuantity(int width);
static size_t writePixelDataOffset(FILE *stream);

static size_t writeDIBHeaderSize(FILE *stream);
static size_t writeDimensions(FILE *stream, Image *image);
static size_t writePlanesQuantity(FILE *stream);
static size_t writeBitsPerPixel(FILE *stream);

static size_t writePixelData(FILE *stream, Image *image);
static size_t writePixel(FILE *stream, ColorRGB pixelColor);
static size_t writeRowPadding(FILE *stream, int paddingQuantity);

static size_t write16BitLittleEndian(int16_t value, FILE *stream);
static size_t write32BitLittleEndian(int32_t value, FILE *stream);
static size_t writeZeroBytes(FILE *stream, int quantity);

FILE *generateBMP(char fileName[], Image *image)
{
	FILE *newBMP = fopen(fileName, "wb");

	if (newBMP == NULL)
	{
		return NULL;
	}

	// Bitmap file header.
	writeBMPIdentifier(newBMP);
	writeSize(newBMP, image);
	writeZeroBytes(newBMP, 4); // Reserved fields.
	writePixelDataOffset(newBMP);

	// Bitmap information header (BITMAPINFOHEADER).
	writeDIBHeaderSize(newBMP);
	writeDimensions(newBMP, image);
	writePlanesQuantity(newBMP);
	writeBitsPerPixel(newBMP);
	writeZeroBytes(newBMP, 4);	// Compression method (0 = no compression).
	writeZeroBytes(newBMP, 4);	// Image size (0 can be used when no compression is used).
	writeZeroBytes(newBMP, 8);	// Image resolution (0 = application dependent).
	writeZeroBytes(newBMP, 4);	// Color count (0 = 2^n).
	writeZeroBytes(newBMP, 4);	// Important colors (0 = all colors are important).

	// Pixel storage.
	writePixelData(newBMP, image);

	return newBMP;
}

static size_t writeBMPIdentifier(FILE *stream)
{
	char fileType[] = "BM";

	return fwrite(fileType, sizeof (char), 2, stream);
}

static size_t writeSize(FILE *stream, Image *image)
{
	int32_t fileSize = getBMPSize(image);

	return write32BitLittleEndian(fileSize, stream);
}

static int32_t getBMPSize(Image *image)
{
	int rowPadding = getRowPaddingQuantity(image->width);
	int pixelCount = image->width * image->height;

	return PIXEL_DATA_OFFSET + pixelCount * BYTES_PER_PIXEL + rowPadding * image->height;
}

static int getRowPaddingQuantity(int width)
{
	return (4 - (width * BYTES_PER_PIXEL) % 4) % 4;
}

static size_t writePixelDataOffset(FILE *stream)
{
	return write32BitLittleEndian(PIXEL_DATA_OFFSET, stream);
}

static size_t writeDIBHeaderSize(FILE *stream)
{
	return write32BitLittleEndian(DIB_HEADER_SIZE, stream);
}

static size_t writeDimensions(FILE *stream, Image *image)
{
	size_t bytesWritten = write32BitLittleEndian(image->width, stream);

	if (bytesWritten != 4)
	{
		return bytesWritten;
	}

	return write32BitLittleEndian(image->height, stream);
}

static size_t writePlanesQuantity(FILE *stream)
{
	return write16BitLittleEndian(1, stream);
}

static size_t writeBitsPerPixel(FILE *stream)
{
	return write16BitLittleEndian(BYTES_PER_PIXEL * 8, stream);
}

static size_t writePixelData(FILE *stream, Image *image)
{
	int paddingQuantity = getRowPaddingQuantity(image->width);

	size_t bytesWritten = 0;

	for (int row = image->height - 1; row >= 0; --row) // Pixel data is written upside down.
	{
		for (int column = 0; column < image->width; ++column)
		{
			bytesWritten += writePixel(stream, getImageColor(image, row, column));
		}

		bytesWritten += writeRowPadding(stream, paddingQuantity);
	}

	return bytesWritten;
}

static size_t writePixel(FILE *stream, ColorRGB pixelColor)
{
	size_t bytesWritten = 0;

	bytesWritten += fwrite(&(pixelColor.blue), sizeof (int8_t), 1, stream);

	if (bytesWritten != 1)
	{
		return bytesWritten;
	}

	bytesWritten += fwrite(&(pixelColor.green), sizeof (int8_t), 1, stream);

	if (bytesWritten != 2)
	{
		return bytesWritten;
	}

	bytesWritten += fwrite(&(pixelColor.red), sizeof (int8_t), 1, stream);

	return bytesWritten;
}

static size_t writeRowPadding(FILE *stream, int paddingQuantity)
{
	return writeZeroBytes(stream, paddingQuantity);
}

static size_t write16BitLittleEndian(int16_t value, FILE *stream)
{
	int8_t secondHalf = value & 0xFF;
	size_t bytesWritten = fwrite(&secondHalf, sizeof (int8_t), 1, stream);

	if (bytesWritten != 1)
	{
		return bytesWritten;
	}

	int8_t firstHalf = (value >> 8) & 0xFF;

	bytesWritten +=fwrite(&firstHalf, sizeof (int8_t), 1, stream);

	return bytesWritten;
}

static size_t write32BitLittleEndian(int32_t value, FILE *stream)
{
	int16_t firstHalf = (value >> 16) & 0xFFFF;
	int16_t secondHalf = value & 0xFFFF;
	size_t bytesWritten = 0;

	bytesWritten = write16BitLittleEndian(secondHalf, stream);

	if (bytesWritten != 2)
	{
		return bytesWritten;
	}

	bytesWritten += write16BitLittleEndian(firstHalf, stream);

	return bytesWritten;
}

static size_t writeZeroBytes(FILE *stream, int quantity)
{
	int8_t zeroByte = 0;
	size_t bytesWritten = 0;

	for (int currentQuantity = 0; currentQuantity < quantity; ++currentQuantity)
	{
		if (fwrite(&zeroByte, sizeof (int8_t), 1, stream) != 1)
		{
			return bytesWritten;
		}

		++bytesWritten;
	}

	return bytesWritten;
}
