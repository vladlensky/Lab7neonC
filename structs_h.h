#pragma once
#pragma pack(push,1)
typedef struct bmp_header {
	uint16_t bfType;
	uint32_t bfileSize;
	uint32_t bfReserved;
	uint32_t bOffBits;
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} bmp_header_t;

#pragma pack(push,1)
struct pixel {
	uint8_t b;
	uint8_t g;
	uint8_t r;
};
#pragma pack(pop)
typedef struct image {
	uint64_t width, height;
	struct pixel* data;
} image_t;

typedef struct pixel_f {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} pixel_t;