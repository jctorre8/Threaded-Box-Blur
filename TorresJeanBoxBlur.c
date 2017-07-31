////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

//TODO: finish me


////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

#pragma warning(disable: 4996)

//problem assumptions
#define BMP_HEADER_SIZE_BYTES 14
#define BMP_DIB_HEADER_SIZE_BYTES 40
#define MAXIMUM_IMAGE_SIZE 256

//bmp compression methods
//none:
#define BI_RGB 0

//TODO: finish me


////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES

typedef struct BMP_Header{
  uint8_t signature[2];    //ID field
  uint32_t size;   //Size of the BMP file
  uint16_t reserved1;    //Application specific
  uint16_t reserved2;    //Application specific
  uint32_t offset_pixel_array;  //Offset where the pixel array (bitmap data) can be found
} BMP_Header;

typedef struct DIB_Header{
  uint32_t headerSize;
  uint32_t width;
  uint32_t height;
  uint16_t planes;    
  uint16_t bitsPerPixel;    
  uint32_t compression;  
  uint32_t imageSize;
  uint32_t xPixelsPM;
  uint32_t yPixelsPM;
  uint32_t colors;
  uint32_t colorCount;
} DIB_Header;

typedef struct Pixel{
  uint8_t b;    
  uint8_t g;    
  uint8_t r;
} Pixel;

//TODO: finish me



////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE


//TODO: finish me

void calculateAveragePixel(Pixel ** image,  Pixel ** blurred_image, int width, int height, int x, int y);
uint8_t calculateAverageB(Pixel ** image, int width, int height, int x, int y);
uint8_t calculateAverageG(Pixel ** image, int width, int height, int x, int y);


void main(int argc, char* argv[]) {
  if(argc != 3){
    printf("Please input the name of the picture you want to blurr and the desired name of the blurred image.\n");
    return;
  }

  int i, j;
  char  filename[256];
  strcpy(filename, argv[1]);

  //sample code to read first 14 bytes of BMP file format
  FILE* file = fopen(filename, "rb");

  if(file == NULL){
    printf("FILE NOT FOUND!! Please input the name of the picture you want to blurr.\n");
    return;
  }
  BMP_Header header;
  DIB_Header diHeader;
  int fnameSize = strlen(filename);
  char newFileName[256];
  strcpy(newFileName, argv[2]);

  //read bitmap file header (14 bytes)
  fread(header.signature, sizeof(uint8_t)*2, 1, file);
  fread(&header.size, sizeof(uint32_t), 1, file);
  fread(&header.reserved1, sizeof(uint16_t), 1, file);
  fread(&header.reserved2, sizeof(uint16_t), 1, file);
  fread(&header.offset_pixel_array, sizeof(uint32_t), 1, file);

  printf("Printing the BMP Header: \n");
  printf("signature: %c%c\n", header.signature[0], header.signature[1]);
  printf("size: %d\n", header.size);
  printf("reserved1: %d\n", header.reserved1);
  printf("reserved2: %d\n", header.reserved2);
  printf("offset_pixel_array: %d\n", header.offset_pixel_array);

  fread(&diHeader.headerSize, sizeof(uint32_t), 1, file);
  fread(&diHeader.width, sizeof(uint32_t), 1, file);
  fread(&diHeader.height, sizeof(uint32_t), 1, file);
  fread(&diHeader.planes, sizeof(uint16_t), 1, file);
  fread(&diHeader.bitsPerPixel, sizeof(uint16_t), 1, file);
  fread(&diHeader.compression, sizeof(uint32_t), 1, file);
  fread(&diHeader.imageSize, sizeof(uint32_t), 1, file);
  fread(&diHeader.xPixelsPM, sizeof(uint32_t), 1, file);
  fread(&diHeader.yPixelsPM, sizeof(uint32_t), 1, file);
  fread(&diHeader.colors, sizeof(uint32_t), 1, file);
  fread(&diHeader.colorCount, sizeof(uint32_t), 1, file);

  printf("\nPrinting the DIB Header: \n");
  printf("DIB Header Size: %d\n", diHeader.headerSize);
  printf("Image Width: %d\n", diHeader.width);
  printf("Image Height: %d\n", diHeader.height);
  printf("Planes: %d\n", diHeader.planes);
  printf("Bits per Pixel: %d\n", diHeader.bitsPerPixel);
  printf("Compression: %d\n", diHeader.compression);
  printf("Image Size: %d\n", diHeader.imageSize);
  printf("X Pixels per Meter: %d\n", diHeader.xPixelsPM);
  printf("Y Pixels per Meter: %d\n", diHeader.yPixelsPM);
  printf("Colors in Color Table: %d\n", diHeader.colors);
  printf("Important Color Count: %d\n", diHeader.colorCount);

  Pixel ** image = (Pixel **) malloc(diHeader.width*diHeader.height*sizeof(Pixel*));
  Pixel ** blurred_image = (Pixel **) malloc(diHeader.width*diHeader.height*sizeof(Pixel*));
  uint32_t buffer_size = ((3*diHeader.width)%4);
  uint8_t buffer[buffer_size];


  for (i = 0; i < diHeader.height*diHeader.width; i++) {
        image[i] = malloc(sizeof(Pixel));
        blurred_image[i] = malloc(sizeof(Pixel));
  }
  
  printf("\nLoading Normal image. \n\n");
  for (i = 0; i < diHeader.height; i++) {
      for (j = 0; j < diHeader.width; j++) {
          fread(&((*(image + i*diHeader.width + j))->r), 1, 1, file);
          fread(&((*(image + i*diHeader.width + j))->g), 1, 1, file);
          fread(&((*(image + i*diHeader.width + j))->b), 1, 1, file);
      }
     fread(buffer, buffer_size, 1, file);
      //printf("\n");
  }

  for (i = 0; i < diHeader.height; i++) {
      for (j = 0; j < diHeader.width; j++) {
        calculateAveragePixel(image, blurred_image, diHeader.width, diHeader.height, j, i);
      }
    }

  FILE* newFile = fopen(newFileName, "wb");
  Pixel newImage[diHeader.width][diHeader.height];

  fwrite(&header.signature, sizeof(char)*2, 1, newFile);
  fwrite(&header.size, sizeof(int), 1, newFile);
  fwrite(&header.reserved1, sizeof(uint16_t), 1, newFile);
  fwrite(&header.reserved2, sizeof(uint16_t), 1, newFile);
  fwrite(&header.offset_pixel_array, sizeof(int), 1, newFile);
  fwrite(&diHeader.headerSize, sizeof(int), 1, newFile);
  fwrite(&diHeader.width, sizeof(int), 1, newFile);
  fwrite(&diHeader.height, sizeof(int), 1, newFile);
  fwrite(&diHeader.planes, sizeof(uint16_t), 1, newFile);
  fwrite(&diHeader.bitsPerPixel, sizeof(uint16_t), 1, newFile);
  fwrite(&diHeader.compression, sizeof(int), 1, newFile);
  fwrite(&diHeader.imageSize, sizeof(int), 1, newFile);
  fwrite(&diHeader.xPixelsPM, sizeof(int), 1, newFile);
  fwrite(&diHeader.yPixelsPM, sizeof(int), 1, newFile);
  fwrite(&diHeader.colors, sizeof(int), 1, newFile);
  fwrite(&diHeader.colorCount, sizeof(int), 1, newFile);

  printf("\nPrinting Blurred image: \n\n");
  for (i = 0; i < diHeader.height; i++) {
      for (j = 0; j < diHeader.width; j++) {
          fwrite(&((*(blurred_image + i*diHeader.width + j))->r), 1, 1, newFile);
          fwrite(&((*(blurred_image + i*diHeader.width + j))->g), 1, 1, newFile);
          fwrite(&((*(blurred_image + i*diHeader.width + j))->b), 1, 1, newFile);
      }
      fwrite(buffer, buffer_size, 1, newFile);
  }
  

  
  for (i = 0; i < diHeader.height*diHeader.width; i++) {
      free(image[i]);
      free(blurred_image[i]);
  }
    

  free(image);
  free(blurred_image);
  fclose(file);
  fclose(newFile);
}

void calculateAveragePixel(Pixel ** image, Pixel ** blurred_image, int width, int height, int x, int y){
  int count = 0;
  int averageR = 0;
  int averageG = 0;
  int averageB = 0;
  int i, j;

  for (i = y-1; i <= y+1; i++) {
    if(i < 0 || i >= width)
      continue;
      for (j = x-1; j <= x+1; j++) {
        if(j < 0 || j >= height)
          continue;

        averageR += (*(image + i*width + j))->r;
        averageG += (*(image + i*width + j))->g;
        averageB += (*(image + i*width + j))->b;
        count++;
      }
  }
  averageR = averageR/count; 
  averageG = averageG/count; 
  averageB = averageB/count;  

  (*(blurred_image + y*width + x))->r = averageR;
  (*(blurred_image + y*width + x))->g = averageG;
  (*(blurred_image + y*width + x))->b = averageB;

}
uint8_t calculateAverageB(Pixel ** image, int width, int height, int x, int y){

}
uint8_t calculateAverageG(Pixel ** image, int width, int height, int x, int y){

}