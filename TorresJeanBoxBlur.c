////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

typedef struct {
  char signature[2];    //ID field
  int size;   //Size of the BMP file
  short reserved1;    //Application specific
  short reserved2;    //Application specific
  int offset_pixel_array;  //Offset where the pixel array (bitmap data) can be found
} BMP_Header;

typedef struct {
  int headerSize;
  int width;
  int height;
  short planes;    
  short bitsPerPixel;    
  int compression;  
  int imageSize;
  int xPixelsPM;
  int yPixelsPM;
  int colors;
  int colorCount;
} DIB_Header;

typedef struct {
  unsigned char b;    
  unsigned char g;    
  unsigned char r;
} Pixel;

//TODO: finish me



////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE


//TODO: finish me

unsigned char calculateAverageR(Pixel ** image, int width, int height, int x, int y);
unsigned char calculateAverageB(Pixel * image, int width, int height, int x, int y);
unsigned char calculateAverageG(Pixel * image, int width, int height, int x, int y);


void main(int argc, char* argv[]) {
  if(argc != 2){
    printf("Please input the name of the picture you want to blurr.\n");
    return;
  }

  int i, j;
  char  filename[30];
  strcpy(filename, argv[1]);
  printf("Successfully copied input. \n\n");

  //sample code to read first 14 bytes of BMP file format
  FILE* file = fopen(filename, "rb");
  BMP_Header header;
  DIB_Header diHeader;
  unsigned int buffer;
  int fnameSize = strlen(filename);
  char newFileName[4+fnameSize];

  newFileName[0] = 'B';
  newFileName[1] = 'l';
  newFileName[2] = 'r';
  newFileName[3] = '_';
  for(i = 0; i < fnameSize+1; i++){
    newFileName[4+i] = filename[i];
  }

  printf("%s\n\n", newFileName);

  //read bitmap file header (14 bytes)
  fread(&header.signature, sizeof(char)*2, 1, file);
  fread(&header.size, sizeof(int), 1, file);
  fread(&header.reserved1, sizeof(short), 1, file);
  fread(&header.reserved2, sizeof(short), 1, file);
  fread(&header.offset_pixel_array, sizeof(int), 1, file);

  printf("Printing the BMP Header: \n");
  printf("signature: %c%c\n", header.signature[0], header.signature[1]);
  printf("size: %d\n", header.size);
  printf("reserved1: %d\n", header.reserved1);
  printf("reserved2: %d\n", header.reserved2);
  printf("offset_pixel_array: %d\n", header.offset_pixel_array);

  fread(&diHeader.headerSize, sizeof(int), 1, file);
  fread(&diHeader.width, sizeof(int), 1, file);
  fread(&diHeader.height, sizeof(int), 1, file);
  fread(&diHeader.planes, sizeof(short), 1, file);
  fread(&diHeader.bitsPerPixel, sizeof(short), 1, file);
  fread(&diHeader.compression, sizeof(int), 1, file);
  fread(&diHeader.imageSize, sizeof(int), 1, file);
  fread(&diHeader.xPixelsPM, sizeof(int), 1, file);
  fread(&diHeader.yPixelsPM, sizeof(int), 1, file);
  fread(&diHeader.colors, sizeof(int), 1, file);
  fread(&diHeader.colorCount, sizeof(int), 1, file);

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

  Pixel ** image = (Pixel **) malloc(diHeader.width*diHeader.height*sizeof(Pixel));

  for (i = 0; i < diHeader.height; i++) {
      for (j = 0; j < diHeader.width; j++) {
        printf("Printing Array [%d][%d]; \n",i*diHeader.width ,j);
        printf("Printing Pointer [%d]; \n",i*diHeader.width +j);
        *(image + i*diHeader.width + j) = (Pixel *) malloc(sizeof(Pixel));
      }
    }
  printf("\nPrinting Normal image: \n\n");
  for (i = diHeader.height-1; i >= 0; i--) {
      for (j = 0; j < diHeader.width; j++) {
          fread(&(*(image + i*j + j))->r, 1, 1, file);
          fread(&(*(image + i*j + j))->b, 1, 1, file);
          fread(&(*(image + i*j + j))->g, 1, 1, file);
          //printf("Printing pixel Width: %d Height: %d \n", j,i);
          //printf("r: %x g: %x b: %x \n", image[j][i].r, image[j][i].g, image[j][i].b);
      }
      fread(&buffer, (4-(4%diHeader.width)), 1, file);
      //printf("Buffer: %d\n", buffer);
      //printf("\n");
  }

  /*
  FILE* newFile = fopen(newFileName, "wb");
  Pixel newImage[diHeader.width][diHeader.height];

  fwrite(&header.signature, sizeof(char)*2, 1, newFile);
  fwrite(&header.size, sizeof(int), 1, newFile);
  fwrite(&header.reserved1, sizeof(short), 1, newFile);
  fwrite(&header.reserved2, sizeof(short), 1, newFile);
  fwrite(&header.offset_pixel_array, sizeof(int), 1, newFile);
  fwrite(&diHeader.headerSize, sizeof(int), 1, newFile);
  fwrite(&diHeader.width, sizeof(int), 1, newFile);
  fwrite(&diHeader.height, sizeof(int), 1, newFile);
  fwrite(&diHeader.planes, sizeof(short), 1, newFile);
  fwrite(&diHeader.bitsPerPixel, sizeof(short), 1, newFile);
  fwrite(&diHeader.compression, sizeof(int), 1, newFile);
  fwrite(&diHeader.imageSize, sizeof(int), 1, newFile);
  fwrite(&diHeader.xPixelsPM, sizeof(int), 1, newFile);
  fwrite(&diHeader.yPixelsPM, sizeof(int), 1, newFile);
  fwrite(&diHeader.colors, sizeof(int), 1, newFile);
  fwrite(&diHeader.colorCount, sizeof(int), 1, newFile);

  printf("\nPrinting Blurred image: \n\n");
  for (i = diHeader.height-1; i >= 0; i--) {
      for (j = 0; j < diHeader.width; j++) {
          fwrite(&image[j][i].r, 1, 1, newFile);
          fwrite(&image[j][i].b, 1, 1, newFile);
          fwrite(&image[j][i].g, 1, 1, newFile);
          //printf("Printing pixel Width: %d Height: %d \n", j,i);
          //printf("r: %x g: %x b: %x \n", image[j][i].r, image[j][i].g, image[j][i].b);
      }
      fwrite(&buffer, (4-(4%diHeader.width)), 1, newFile);
      //printf("Buffer: %d\n", buffer);
      //printf("\n");
  }
  */

  unsigned char test;
  test = calculateAverageR(image, diHeader.width, diHeader.height, 0, 2);

    for (i = 0; i < diHeader.height; i++) {
      for (j = 0; j < diHeader.width; j++) {
        free(image + i*diHeader.width + j);
      }
    }

  free(image);
  fclose(file);
  //fclose(newFile);
}

unsigned char calculateAverageR(Pixel ** image, int width, int height, int x, int y){
  int count = 0;
  int average = 0;
  int i, j;

  for (i = x-2; i <= x+2; i++) {
    if(i < 0 || i > width)
      continue;
      for (j = y-2; j <= y+2; j++) {
        if(j < 0 || j > height)
          continue;
        printf("x: %d y: %d", i, j);

      }
      printf("\n");
  }

}
unsigned char calculateAverageB(Pixel * image, int width, int height, int x, int y){

}
unsigned char calculateAverageG(Pixel * image, int width, int height, int x, int y){

}