#ifndef _IMAGE_H_
#define _IMAGE_H_
#include <stdint.h>

#pragma pack(push, 2)

typedef struct bmp_header_t {
        uint16_t bfType;                /* little/big-endian*/
        uint32_t bfSize;                /* file size in bytes*/
        uint32_t bfReserved;            /* must be 0*/
        uint32_t bOffBits;              /* pixel data location from header*/
        uint32_t biSize;                /* struct size in bytes */
        uint32_t biWidth;               /* raster width in pixels*/
        uint32_t biHeight;              /* raster height in pixels*/
        uint16_t biPlanes;
        uint16_t biBitCount;            /* bits per pixel*/
        uint32_t biCompression;         /* method of pixels storage */
        uint32_t biSizeImage;           /* size of pixel data in bytes*/
        uint32_t biXPelsPerMeter;       /* horyzonatl pixels per meter */
        uint32_t biYPelsPerMeter;       /* horyzontal pixels per meter */
        uint32_t biClrUsed;             /* size of color table*/
        uint32_t biClrImportant;
} bmp_header_t;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct pixel_t {
        char r;
        char g;
        char b;
} pixel_t;

#pragma pack(pop)

typedef struct image_t {
        uint32_t width;
        uint32_t height;
        pixel_t* pixels;
} image_t;

typedef enum {
        ok = 0,
        cant_open,
        not_bmp,
        cant_create
} error;



error deserialize_bmp(FILE* const image_file, image_t* image);

error serialize_bmp(image_t* image, char* file_name);

void rotate_pixels(image_t* image);

void rotate_bmp(image_t* image);

error open_rotate_save(char* image_file_name, char* new_file_name);

int rotate(char* file_name);

#endif
