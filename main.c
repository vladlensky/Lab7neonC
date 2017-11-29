#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "structs_h.h"
#include "functions_h.h"
#include "sepia.h"
#include <math.h>
#include "arm_neon.h"
//uint32_t vector_add_of_n(uint32_t* ptr, uint32_t items)
//{
//    uint32_t result,* i;
//    uint32x2_t vec64a, vec64b;
//    uint32x4_t vec128 = vdupq_n_u32(0); // clear accumulators
//
//    for (i=ptr; i<(ptr+(items/4));i+=4)
//    {
//        float32x4_t first = vld1q_f32(i);
//        float32x4_t second = vld1q_f32(i);
//        float32x4_t third = vld1q_f32(i);
//         // load 4x 32 bit values
//    }
//
//    vec64a = vget_low_u32(vec128); // split 128 bit vector
//    vec64b = vget_high_u32(vec128); // into 2x 64 bit vectors
//
//    vec64a = vadd_u32 (vec64a, vec64b); // add 64 bit vectors together
//
//    result = vget_lane_u32(vec64a, 0); // extract lanes and
//    result += vget_lane_u32(vec64a, 1); // add together scalars
//    return result;
//}
float *convert(struct image *image){
    uint64_t i = 0;
    float *px = malloc(image->width*image->height* sizeof(float)*3);
    for(;i<image->width*image->height*3;i+=3){
        px[i] = (float)image->data[i/3].r;
        px[i+1] = (float)image->data[i/3].g;
        px[i+2] = (float)image->data[i/3].b;
    }
    return px;
}
void unconvert(float const *arr,struct image *image){
    struct pixel *px = malloc(image->width*image->height* sizeof(struct pixel));
    uint64_t  i = 0;
    for(;i<image->width*image->height*3;i+=3){
        px[i/3].b = (uint8_t)arr[i];
        px[i/3].g = (uint8_t)arr[i+1];
        px[i/3].r = (uint8_t)arr[i+2];
    }
    image->data = px;
}
void sepia_c_inplace( struct image* img ) {
    uint32_t x, y;
    float *arr = convert(img);

    unconvert(arr,img);
    float bgr[36] = {0.272, 0.349, 0.393,0.272, 0.349, 0.393,0.272, 0.349, 0.393,0.272, 0.349, 0.393,
                     0.543, 0.686, 0.769,0.543, 0.686, 0.769,0.543, 0.686, 0.769,0.543, 0.686, 0.769,
                     0.131, 0.168, 0.189,0.131, 0.168, 0.189,0.131, 0.168, 0.189,0.131, 0.168, 0.189};
    float32x4_t mul1 = vld1q_f32(bgr);
    float32x4_t mul2 = vld1q_f32(bgr+12);
    float32x4_t mul3 = vld1q_f32(bgr+24);
    float max[4] = {255,255,255,255};
    float32x4_t Max = vld1q_f32(max);
    float *resultArr = malloc(img->width*img->height* sizeof(float)*3);
    for (y = 0; y < img->height; y++) {
        for (x = 0; x < img->width; x++) {
            float32x4_t first = vdupq_n_f32(arr[(x + y * img->width) * 3]);
            float32x4_t second = vdupq_n_f32(arr[(x + y * img->width) * 3 + 1]);
            float32x4_t third = vdupq_n_f32(arr[(x + y * img->width) * 3 + 2]);
            first = vmulq_f32(first, mul1);
            second = vmulq_f32(second, mul2);
            third = vmulq_f32(third, mul3);
            first = vaddq_f32(first, second);
            first = vaddq_f32(first, third);
            first = vminq_f32(first, Max);
            float32x2_t low = vget_low_f32(first);
            float32x2_t high = vget_high_f32(first);
            resultArr[(x + y * img->width) * 3] = vget_lane_f32(low, 0);
            resultArr[(x + y * img->width) * 3 + 1] = vget_lane_f32(low, 1);
            resultArr[(x + y * img->width) * 3 + 2] = vget_lane_f32(high, 0);
            //sepia_one(img->data + y * img->width + x);
        }
    }
    unconvert(resultArr,img);
}
int main() {
	FILE *input = fopen("/home/vladlensky/CLionProjects/lab5c/tiger.bmp", "rb");
    FILE *output = fopen("/home/vladlensky/CLionProjects/lab5c/Duck1.bmp", "wb");
    FILE *outputAssembler = fopen("/home/vladlensky/CLionProjects/lab5c/Duck2.bmp", "wb");
	struct bmp_header* bh_ptr = (struct bmp_header*)malloc(sizeof(struct bmp_header));
	struct image* img_ptr = (struct image*)malloc(sizeof(struct image));
    from_bmp(input, bh_ptr, img_ptr);
    sepia_c_inplace(img_ptr);
    to_bmp(output, bh_ptr, img_ptr);
    //Cепии + подсчёт времени
//    double begin, end;
//    begin = clock();
//    int i = 0;
//    struct image *newImage;
//    int N = 10;
//    double * cTime  = malloc(sizeof(double)*N);
//    double * sseTime  = malloc(sizeof(double)*N);
//    double medianaC = 0,mediumC = 0,mediumOtC = 0;
//    double medianaSSE = 0,mediumSSE = 0,mediumOtSSE = 0;
//    for(;i<N;i++) {
//        input = fopen("/home/vladlensky/CLionProjects/lab5c/tiger.bmp", "rb");
//        from_bmp(input, bh_ptr, img_ptr);
//        begin = clock();
//        newImage = sepia(img_ptr);
//        end = clock();
//        mediumSSE +=  (end - begin) / CLOCKS_PER_SEC;
//        sseTime[i] = (end - begin) / CLOCKS_PER_SEC;
//        begin = clock();
//        sepia_c_inplace(img_ptr);
//        end = clock();
//        mediumC += (end - begin) / CLOCKS_PER_SEC;
//        cTime[i] = (end - begin) / CLOCKS_PER_SEC;
//    }
//    medianaSSE = sseTime[N/2];
//    medianaC = cTime[N/2];
//    mediumC /= N;
//    mediumSSE /= N;
//    for(;i<N;i++){
//        mediumOtC += (cTime[i] - mediumC)*(cTime[i] - mediumC);
//        mediumOtSSE += (sseTime[i] - mediumSSE)*(sseTime[i] - mediumSSE);
//    }
//    mediumOtC = sqrt(mediumOtC/N);
//    mediumOtSSE = sqrt(mediumOtSSE/N);
//    double mistakesC = 0;
//    double * mistakesSSE = malloc(sizeof(double)*img_ptr->width*img_ptr->height);
//    printf("Медиана для C: %lf\n",medianaC);
//    printf("Среднее для C: %lf\n",mediumC);
//    printf("Среднекв. откл. для C: %lf\n",mediumOtC);
//    printf("Медиана для SSE: %lf\n",medianaSSE);
//    printf("Среднее для SSE: %lf\n",mediumSSE);
//    printf("Среднекв. откл. для SSE: %lf\n",mediumOtSSE);
//    for(i=0;i<img_ptr->width*img_ptr->height;i++){
//        mistakesC = img_ptr->data[i].b-newImage->data[i].b;
//        mistakesC += img_ptr->data[i].g-newImage->data[i].g;
//        mistakesC += img_ptr->data[i].r-newImage->data[i].r;
//    }
//    printf("Средние ошибки в пикселях:%lf\n",mistakesC/img_ptr->width/img_ptr->height);
//    to_bmp(output, bh_ptr, img_ptr);
//    to_bmp(outputAssembler, bh_ptr, newImage);
//	fclose(input);
//    fclose(output);
//    fclose(outputAssembler);
}