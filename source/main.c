#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "perlin_noise_bmp_generator.h"

static void printInvalidArgumentCountMessage(void);
static void printInvalidArgumentsMessage(void);
static void printHelpMessage(void);
static void printUsageMessage(void);
static void printVersionMessage(void);
static void printGenerationErrorMessage(void);

int main(int argc, char **argv)
{
	if (argc < 2 || argc > 4)
	{
		printInvalidArgumentCountMessage();

		return EXIT_FAILURE;
	}

	if (argc == 2)
	{
		if (strcmp(argv[1], "help") != 0)
		{
			printInvalidArgumentsMessage();

			return EXIT_FAILURE;
		}

		printHelpMessage();

		return EXIT_SUCCESS;
	}

	int width = atoi(argv[1]);
	int height = atoi(argv[2]);

	if (width <= 0 || height <= 0)
	{
		printInvalidArgumentsMessage();

		return EXIT_FAILURE;
	}

	int cellSize = 32;

	if (argc == 4)
	{
		cellSize = atoi(argv[3]);

		if (cellSize <= 0)
		{
			printInvalidArgumentsMessage();

			return EXIT_FAILURE;
		}
	}

	srand(time(NULL));

	FILE *bmp = generatePerlinNoiseBMP("noise.bmp", width, height, cellSize);

	if (bmp == NULL)
	{
		printGenerationErrorMessage();

		return EXIT_FAILURE;
	}

	fclose(bmp);

	return EXIT_SUCCESS;
}

static void printInvalidArgumentCountMessage(void)
{
	printf("Invalid argument count.\n");

	printUsageMessage();
}

static void printInvalidArgumentsMessage(void)
{
	printf("Invalid arguments.\n");

	printUsageMessage();
}

static void printHelpMessage(void)
{
	printf("VSFMPPNTFBUBG: Very simple free multi-platform Perlin noise 24 bit uncompressed bitmap generator.\n");
	printUsageMessage();
	printVersionMessage();
}

static void printUsageMessage(void)
{
	printf("Usage: vsfmppntfbubg <width> <height> [cell size]\n");
}

static void printVersionMessage(void)
{
	printf("Version: 2020.08.1\n");
}

static void printGenerationErrorMessage(void)
{
	printf("A problem has occured while generating the perlin noise file.\n");
}
