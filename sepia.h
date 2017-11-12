#pragma once
void sepia_c_inplace( struct image* img );
static unsigned char sat( uint64_t x);
static void sepia_one( struct pixel* const pixel );
void sepia_asm(void *source, int length, void *dest);