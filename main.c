#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER


#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

BFHeader bf;

int main(int argc, char *argv[]) {

	FILE *fp;

	if (argc < 2) {
		printf("Usage: %s <filename> \n", argv[0]);
		exit(1);
	}

	fp = fopen(argv[1], "rb");
	if (!fp) {
		printf("Could not open image!\n");
		return(0);
	}
	
	fillHeader(&bf, fp);

	/*printf("%c%c\n", bf.signature[0], bf.signature[1]);
	printf("%X\n", bf.imageSize);
	printf("%hX\n", bf.reserved1);
	printf("%hX\n", bf.reserved2);
	printf("%X\n", bf.pixelDataOffset);*/

	if (bf.BIHeader.bitsPerPixel != 1) {
		puts("Works only with monochromatic images\n");
		fclose(fp);
		exit(1);
	}
	int totalWidth = bf.BIHeader.bitsPerPixel * bf.BIHeader.imageWidth;
	int BytesPerRow = ceiling(totalWidth, 32) * 4;


	return(0);
}