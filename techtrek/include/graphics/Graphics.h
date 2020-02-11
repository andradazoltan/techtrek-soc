/*
 *  Graphics.h
 *
 *  Created on: Feb 6, 2020
 *  Author: Elijah Hewer
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "Colours.h"

// This macro pauses until the graphics chip status register indicates that it is idle
#define WAIT_FOR_GRAPHICS       while((GraphicsStatusReg & 0x0001) != 0x0001);

// Commands to the Graphics Modules
#define DrawHLine               1
#define DrawVLine               2
#define DrawLine                3
#define DrawCircle              4
#define FScreen                 5
#define PutAPixel               0xA
#define GetAPixel               0xB
#define ProgramPaletteColour    0x10

// Function Definitions;
void FillScreen(int Colour);
void ProgramPalette(int PaletteNumber, int RGB);

int ReadAPixel(int x, int y);
void WriteAPixel(int x, int y, int Colour);

void HLine(int x1, int y1, int length, int Colour);
void VLine(int x1, int y1, int length, int Colour);
void Rect(int x1, int y1, int x2, int y2, int Colour);
void FillRect(int x1, int y1, int x2, int y2, int Colour);
void Line(int x1, int y1, int x2, int y2, int Colour);
void Triangle(int x1, int y1, int x2, int y2, int x3, int y3, int Colour);
void Circle(int x1, int y1, int radius, int Colour);
void FillCircle(int x1, int y1, int radius, int Colour);

// Fill.cpp Definitions
typedef struct { int x,y;} XYPixel ;    // data structure to store a coordinate
int PushPixel(XYPixel p1);
int PopPixel(XYPixel *theXYPixel);
int IsStackEmpty(void);
void Fill(int _x, int _y, int _FillColour, int _BoundaryColour);


#endif /* GRAPHICS_H_ */
