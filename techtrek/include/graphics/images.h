/*  
 *  images.h
 *  
 *  Created on: February 22, 2020
 *  Author: Elijah Hewer
 *  
 *  For help with BMP file format:
 *  https://en.wikipedia.org/wiki/BMP_file_format
 */
#ifndef IMAGES_H_
#define IMAGES_H_

#include <stdint.h>
#include <stdio.h>

// Pack structure together for reading
#pragma pack(push,1)
typedef struct {
    // Bitmap file header
    uint16_t  type;             // Is always "BM", which is 0x4d42
    uint32_t  size;             // File size in bytes
    uint16_t  reserved1;        // Reserved (unused)
    uint16_t  reserved2;        // Reserved (unused)
    uint32_t  offset;           // Bytes from beginning of file to start of pixel data

    // DIB header (info header)
    uint32_t  dib_header_size;  // DIB header size in bytes (will change based on header type)
    int32_t   width;            // Image width in pixels
    int32_t   height;           // Image height in pixels
                                /*
                                 * Note that this is signed because a positive value
                                 * has the origin in the BOTTOM-LEFT corner, but a negative
                                 * value has the origin in the UPPER-LEFT corner
                                 */
    uint16_t  planes;           // # of colour planes
    uint16_t  bits_ppx;         // # of bits per pixel (colour depth)
    uint32_t  compression;      // Type of compression method (0 is no compression)
    uint32_t  image_size;       // Image size in bytes
    int32_t   x_res_pxpm;       // Horizontal resolution in pixels per meter
    int32_t   y_res_pxpm;       // Vertical resolution in pixels per meter
    uint32_t  palette_colors;   // # of colours in the colour palette
    uint32_t  important_colors; // # of important colours
} BMPHeader;
#pragma pack(pop)

typedef struct {
    BMPHeader header;
    unsigned char* data;
} BMP;

// Function Definitions
int initImage(char* filename, BMP * mybmp);
void drawImage(int topLeftX, int topLeftY, int Colour, BMP* myBMP);
int freeImage(BMP* myBMP);

#endif /* IMAGES_H_ */
