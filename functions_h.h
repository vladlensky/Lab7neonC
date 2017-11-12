#pragma once
enum read_status {
    READ_OK = 0,
    READ_COULDNT_OPEN,
    READ_INVALID_BIT_COUNT
};
enum read_status from_bmp(FILE* in, struct bmp_header* const bmp_h, struct image* const read);
enum write_status {
    WRITE_OK = 0,
    WRITE_COULDNT_CRETE
};
enum write_status to_bmp(FILE* out, struct bmp_header* const bmp_h, struct image* const img);
struct image* rotate_image_rigth(struct image const * source);
struct image* rotate_image_left(struct image const * source);
struct bmp_header* rotate_header(struct bmp_header const * old_header);