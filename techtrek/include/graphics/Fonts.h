/*
 *  Fonts.h
 *
 *  Created on: Feb 6, 2020
 *  Author: Elijah Hewer & Andrada Zoltan
 */

#ifndef FONTS_H_
#define FONTS_H_

// font Definitions
#define XRES 800    // column pixel count
#define YRES 480    // row pixel count

// Font 1
extern const unsigned char Font5x7[][7];

// Font 2
#define FONT2_XPIXELS	10				// width of Font2 characters in pixels (no spacing)
#define FONT2_YPIXELS	14				// height of Font2 characters in pixels (no spacing)
extern const unsigned short int Font10x14[][14];

// Font 3
#define FONT3_XPIXELS 16
#define FONT3_YPIXELS 27
extern const unsigned char Font16x27[];

// Font 4
#define FONT4_XPIXELS 22
#define FONT4_YPIXELS 40
extern const unsigned char Font22x40[];

// Font 5
#define FONT5_XPIXELS 38
#define FONT5_YPIXELS 59
extern const unsigned char Font38x59[];

void OutGraphicsCharFont5(int x, int y, int colour, int backgroundcolour, const char* text, int Erase);
void OutGraphicsCharFont4(int x, int y, int colour, int backgroundcolour, const char* text, int Erase);
void OutGraphicsCharFont3(int x, int y, int colour, int backgroundcolour, const char* text, int Erase);
void OutGraphicsCharFont2(int x, int y, int colour, int backgroundcolour, int c, int Erase);
void OutGraphicsCharFont1(int x, int y, int fontcolour, int backgroundcolour, int c, int Erase);

#endif /* FONTS_H_ */
