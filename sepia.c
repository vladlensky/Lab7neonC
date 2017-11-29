#include <stdint.h>
#include <stdlib.h>
#include "structs_h.h"
#include "sepia.h"
float *convert(struct image *image){
    int i = 0;
    float *px = malloc(image->width*image->height* sizeof(float)*3);
    for(;i<image->width*image->height*3;i+=3){
        px[i] = (float)image->data->r;
        px[i+1] = (float)image->data->g;
        px[i+2] = (float)image->data->b;
    }
    return px;
}
void unconvert(float const *arr,struct image *image){
    struct pixel *px = malloc(image->width*image->height* sizeof(struct pixel));
    int i = 0;
    for(;i<image->width*image->height*4;i+=4){
        px[i].r = (uint8_t)arr[i];
        px[i].g = (uint8_t)arr[i+1];
        px[i].b = (uint8_t)arr[i+2];
    }
    image->data = px;
}
void sepia_c_inplace( struct image* img ) {
    uint32_t x, y;
    float *arr = convert(img);
    float bgr[36] = {0.272, 0.349, 0.393,0.272, 0.349, 0.393,0.272, 0.349, 0.393,0.272, 0.349, 0.393,
                     0.543, 0.686, 0.769,0.543, 0.686, 0.769,0.543, 0.686, 0.769,0.543, 0.686, 0.769,
                     0.131, 0.168, 0.189,0.131, 0.168, 0.189,0.131, 0.168, 0.189,0.131, 0.168, 0.189};
    float32x4_t mul1 = vld1q_f32(bgr);
    float32x4_t mul2 = vld1q_f32(bgr+4);
    float32x4_t mul3 = vld1q_f32(bgr+8);
    float max[4] = {255,255,255,255};
    float32x4_t Max = vld1q_f32(max);
    float *resultArr = malloc(img->width*img->height* sizeof(float)*3);
    for (y = 0; y < img->height; y++)
        for (x = 0; x < img->width; x++) {
            float32x4_t first = vdupq_n_f32(arr[(x+y*img->width)*3]);
            float32x4_t second = vdupq_n_f32(arr[(x+y*img->width)*3+1]);
            float32x4_t third = vdupq_n_f32(arr[(x+y*img->width)*3+2]);
            first = vmulq_f32(first,mul1);
            second = vmulq_f32(second,mul2);
            third = vmulq_f32(third,mul3);
            first = vaddq_f32(first,second);
            first = vaddq_f32(first,third);
            first = vminq_f32(first,Max);
            float32x2_t low =  vget_low_f32 (first);
            float32x2_t high =  vget_high_f32 (first);
            resultArr[(x+y*img->width)*3] = vget_lane_f32(low,0);
            resultArr[(x+y*img->width)*3+1] = vget_lane_f32(low,1);
            resultArr[(x+y*img->width)*3+2] = vget_lane_f32(high,0);
            //sepia_one(img->data + y * img->width + x);
        }
    unconvert(resultArr,img);
}

static unsigned char sat( uint64_t x) {
    if (x < 256) return x; return 255;
}
static void sepia_one( struct pixel* const pixel ) {
    static const float c[3][3] = {
            {.393f, .769f, .189f},
            {.349f, .686f, .168f},
            {.272f, .543f, .131f}};
    struct pixel const old = *pixel;
    float result,* i;
    for (; i<i+size;i+=3)
    {
        float32x4_t first = vld1q_f32(i);
        float32x4_t second = vld1q_f32(i+1);
        float32x4_t third = vld1q_f32(i+2);
    }
    pixel->r = sat(
            old.r * c[0][0] + old.g * c[0][1] + old.b * c[0][2]
    );
    pixel->g = sat(
            old.r * c[1][0] + old.g * c[1][1] + old.b * c[1][2]
    );
    pixel->b = sat(
            old.r * c[2][0] + old.g * c[2][1] + old.b * c[2][2]
    );
}