#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>

#define LEFT 1
#define RIGHT 0



#pragma pack(push, 2)

typedef struct bmp_header_t {
	uint16_t bfType; 		/* 424D/4D42 */
	uint32_t bfSize; 		/* file size in bytes*/
	uint32_t bfReserved; 		/* must be 0*/
	uint32_t bOffBits; 		/* pixel data location from header*/
	uint32_t biSize; 		/* struct size in bytes */
	uint32_t biWidth; 		/* raster width in pixels*/
	uint32_t biHeight; 		/* raster height in pixels*/
	uint16_t biPlanes; 
	uint16_t biBitCount;	 	/* bits per pixel*/
	uint32_t biCompression; 	/* method of pixels storage */
	uint32_t biSizeImage; 		/* size of pixel data in bytes*/
	uint32_t biXPelsPerMeter;	/* horyzonatl pixels per meter */
	uint32_t biYPelsPerMeter; 	/* horyzontal pixels per meter */
	uint32_t biClrUsed; 		/* size of color table*/
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
	
		
	
error deserialize_bmp(FILE* const image_file, image_t* image) {
	size_t iter, offset;
	bmp_header_t* header = (bmp_header_t*) malloc(sizeof(bmp_header_t));
	fread(header, sizeof(bmp_header_t), 1, image_file);

	if((header->bfType != 0x4D42) && (header->bfType != 0x424D)) return not_bmp;
	
	image->width = header->biWidth;
	image->height = header->biHeight;
	image->pixels = (pixel_t*) malloc(sizeof(pixel_t) * image->width * image->height);
	
	fseek(image_file, header->bOffBits, SEEK_SET);
	offset = 4 - ((image->width*3) % 4);
	for(iter = 0; iter < image->height; iter++) {
		fread(image->pixels + iter * image->width, image->width * sizeof(pixel_t) , 1, image_file);
		fseek(image_file, offset , SEEK_CUR);
	}
	free(header);
	return ok;
}	

error serialize_bmp(image_t* image, char* file_name) {
	size_t iter;
	pixel_t* pixels;
	uint8_t lostbytes = 4 -((image->width*3) % 4);
	uint8_t* nullpull = (uint8_t*) calloc(lostbytes, sizeof(uint8_t));
	bmp_header_t* header = (bmp_header_t*) malloc(sizeof(bmp_header_t));
	FILE* file = fopen(file_name, "wb");
	if(!file) return cant_create;
	pixels = image->pixels;
	
	
	header->bfType = 0x4D42;
	header->bfSize = sizeof(bmp_header_t) + image->height * image->width + (image->height * (image->width % 4));
	header->bfReserved = 0;
	header->bOffBits = sizeof(bmp_header_t);
	header->biSize = 0x28;
	header->biWidth = image->width;
	header->biHeight = image->height;
	header->biPlanes = 1;
	header->biBitCount = 24;
	header->biCompression = 0;
	header->biSizeImage = image->height * image->width + (image->height * (image->width % 4));
	header->biXPelsPerMeter = 0;
	header->biYPelsPerMeter = 0;
	header->biClrUsed = 0;
	header->biClrImportant = 0;

	fwrite(header, header->bOffBits, 1, file);
	fseek(file, header->bOffBits, SEEK_SET);

	for(iter = 0; iter < header->biHeight; iter++) {
		fwrite(pixels, header->biWidth * 3, 1, file);
		pixels += header->biWidth;
		fwrite(nullpull, lostbytes, 1, file);
	}

	free(nullpull);
	fclose(file);
	return ok;
}

void rotate_pixels(image_t* image) {	
	int64_t i, j;
	pixel_t* pixels = malloc(sizeof(pixel_t) * image->width * image->height);
	pixel_t* start_pixels_pointer = pixels;
	for(i = image->width - 1; i >= 0; i--) 
		for(j = 0; j < image->height; j++) {
			*pixels = *(image->pixels + (image->width * j + i));
			pixels++;
		}
	pixels = start_pixels_pointer;
	image->pixels = pixels;
}

void rotate_bmp(image_t* image) {
	image_t* new_image = (image_t*) malloc(sizeof(image_t));
	rotate_pixels(image);
	new_image->width = image->height;
	new_image->height = image->width;
	new_image->pixels = image->pixels;
	*image = *new_image;
	free(new_image);
}

error open_rotate_save(char* image_file_name, char* new_file_name, int side, size_t rot_num) {
	size_t iter;
	image_t* image = (image_t*) malloc(sizeof(image_t));
	FILE* file = fopen(image_file_name, "rb");
	if(!file) return cant_open;
	if(deserialize_bmp(file, image) == not_bmp) return not_bmp;
	for(iter = 0; iter < rot_num; iter++) {	
		if(side == RIGHT)
			rotate_bmp(image);
		if(side == LEFT) {
			rotate_bmp(image);
			rotate_bmp(image);
			rotate_bmp(image);
		}
	}
	if(serialize_bmp(image, new_file_name) == cant_create) return cant_create;
	fclose(file);
	free(image);
	return ok;
}
	
	

int rotate(char* file_name, int side, size_t rot_num, char* output) {
	switch(open_rotate_save(file_name, output, side, rot_num)) {
		case 0: 
			printf("%s", "Everything is OK, your file ready\n"); 
			return 1;
		case 1: 
			printf("%s", "Can't open file\n");
			return 0;
		case 2: 
			printf("%s", "It's not BMP\n");
			return 0;
		case 3: 
			printf("%s", "Can't create file\n");
			return 0;
	}
			
	return 0;
}	








		
		
	
	


















	



