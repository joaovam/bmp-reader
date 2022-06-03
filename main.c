#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER


#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "bmp.h"

BFHeader bf;
//char** repBMP;//bmp representation in memory

int main(int argc, char *argv[]) {

	FILE *fp;
	
	int totalWidth = 0;
	int bytesPerRow = 0;
	int x, y,currentByte = 0,cont = 0,quantBits = 0,repeatedBits = 0;
	unsigned char currentBit = 1;

	

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
		puts("It works only with monochromatic images\n");
		fclose(fp);
		exit(1);
	}

	totalWidth = bf.BIHeader.bitsPerPixel * bf.BIHeader.imageWidth;
	bytesPerRow = ceiling(totalWidth, 32) * 4;

	//repBMP = (unsigned char**)malloc(bf.BIHeader.imageHeigth * sizeof(unsigned char*));

	/*printf("Total Width: %d\n", totalWidth);
	printf("Bytes per Row: %d\n", bytesPerRow);*/

	fseek(fp, bf.pixelDataOffset,SEEK_SET);

	for (y = 0; y < bf.BIHeader.imageHeigth; y++) {

		//repBMP[y] = (unsigned char*)malloc(totalWidth * sizeof(char) + 1);
		//memset(repBMP[y], '\0', sizeof(char) * totalWidth + 1);

		quantBits = repeatedBits = 0;

		for (x = 0; x < bytesPerRow; x++) {

			fread(&currentByte, 1, 1, fp);

			for (cont = 7; cont >= 0; cont--) {

				//printf("%d\n", checkBit(&currentByte, cont));
				if (quantBits < totalWidth) {
					if (checkBit(&currentByte, cont) == currentBit) {
						repeatedBits++;
					}else {
						printf("%d ", repeatedBits);
						repeatedBits = 1;
						currentBit = !currentBit;
					}
					if (repeatedBits >= 0xff) {
						exit(0);
					}
				}
				quantBits++;
			}
		}
		printf("%d ", repeatedBits);
	}

	//for (y = bf.BIHeader.imageHeigth -1 ; y >=0 ; y--) {
	//	printf("%s\n", repBMP[y]);
	//}

	return(0);
}