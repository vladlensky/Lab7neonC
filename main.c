#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "structs_h.h"
#include "functions_h.h"
#include "sepia.h"

int main() {
	FILE *inptr = fopen("/home/vladlensky/CLionProjects/lab5c/Graphics_duck.bmp", "rb");
	FILE *outptr = fopen("/home/vladlensky/CLionProjects/lab5c/Duck2.bmp", "wb");
	struct bmp_header* bh_ptr = (struct bmp_header*)malloc(sizeof(struct bmp_header));
	struct image* img_ptr = (struct image*)malloc(sizeof(struct image));
	from_bmp(inptr, bh_ptr, img_ptr);
    sepia_c_inplace(img_ptr);
	to_bmp(outptr, bh_ptr, img_ptr);
	fclose(inptr);
	fclose(outptr);
}