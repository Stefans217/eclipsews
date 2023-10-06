/**************************************************************

 The program reads a BMP image file and creates a new
 image that is the negative or desaturated of the input file.

 **************************************************************/
/*
 * processing time report:
 * 2: 387ms
 *
 * 4: 366ms
 *
 * 8: 390ms
 *
 * 16: 400ms
 *
 *
 */
#include "qdbmp.h"
#include <stdio.h>
#include <omp.h>

typedef enum {desaturate, negative} ImgProcessing ;

void processSection(BMP* bmp, ImgProcessing processingType, int my_start, int my_end, UINT height){

	UCHAR r,g,b;

	for (int x = my_start; x < my_end; ++x) {
		for (int y = 0; y < height; ++y) {
			/* Get pixel's RGB values */
			BMP_GetPixelRGB(bmp, x, y, &r, &g, &b);

			/* Write new RGB values */
			if(processingType == negative)
				BMP_SetPixelRGB(bmp, x, y, 255 - r, 255 - g, 255 - b);
			else if(processingType == desaturate){
				UCHAR gray = r * 0.3 + g * 0.59 + b * 0.11;
				BMP_SetPixelRGB(bmp, x, y, gray, gray, gray);
			}

		}
	}
}


/* Creates a negative image of the input bitmap file */
int main() {
	const char* inFile = "okanagan.bmp";
	const char* outFile = "okanagan_processed.bmp";
	const ImgProcessing processingType = desaturate; //or negative

	UCHAR r, g, b;
	UINT width, height;
	BMP* bmp;

	/* Read an image file */
	bmp = BMP_ReadFile(inFile);
	BMP_CHECK_ERROR(stdout, -1);

	/* Get image's dimensions */
	width = BMP_GetWidth(bmp);
	height = BMP_GetHeight(bmp);

	double t = omp_get_wtime();
	
	/* Iterate through all the image's pixels */

	int num_threads = 4;
	int numPerThread = width/num_threads;
	int remaining = width % num_threads;
	printf("num_threads: %d, ", num_threads);
	printf("numPerThread: %d, ", numPerThread);
	printf("remaining: %d", remaining);
#pragma omp parallel num_threads(num_threads)
	{
		int id = omp_get_thread_num();
		int my_start = id*numPerThread;
		int my_end = (id == num_threads - 1) ? width : my_start + numPerThread;//if this thread is the last thread then compute remaining elements

		processSection(bmp, processingType, my_start, my_end, height);
/*
#pragma omp critical
		{

		printf("\n==================\n");
		printf("id: %d, ", id);
		printf("my_start: %d, ", my_start);
		printf("my_end: %d, ", my_end);
		//printf("ranx: %d, ", ranx);
		//printf("rany: %d", rany);
		}
*/
	}

	//process remaining pixels if remaining > 0. replaced this with if statement in my_end declaration
	//processSection(bmp, processingType, width - remaining, width, height);

	/* calculate and print processing time*/
	t = 1000 * (omp_get_wtime() - t);
	printf("\nFinished image processing in %.1f ms.", t);

	/* Save result */
	BMP_WriteFile(bmp, outFile);
	BMP_CHECK_ERROR(stdout, -2);

	/* Free all memory allocated for the image */
	BMP_Free(bmp);

	return 0;
}
