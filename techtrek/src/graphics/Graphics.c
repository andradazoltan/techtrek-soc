/*
 *  Graphics.c
 *
 *  Created on: Feb 6, 2020
 *  Author: Elijah Hewer
 */

#include "Graphics.h"
#include "RegisterDefs.h"

/*********************************************************************************************
 * This function fills the screen with the desired colour
*********************************************************************************************/
void FillScreen(int Colour)
{
    WAIT_FOR_GRAPHICS;

    GraphicsColourReg = Colour;      // Set pixel colour
    GraphicsCommandReg = FScreen;    // Send "fill screen" command to graphics controller
}

/**********************************************************************************
** Subroutine to program a hardware (graphics chip) palette number with an RGB value
** e.g. ProgramPalette(RED, 0x00FF0000) ;
************************************************************************************/
void ProgramPalette(int PaletteNumber, int RGB)
{
    WAIT_FOR_GRAPHICS;
    GraphicsColourReg = PaletteNumber;
    GraphicsX1Reg = RGB >> 16;          // Program red value in ls.8 bit of X1 reg
    GraphicsY1Reg = RGB;                // Program green and blue into ls 16 bit of Y1 reg
    GraphicsCommandReg = ProgramPaletteColour; // Issue command
}

/*********************************************************************************************
* This function read a single pixel from the x,y coords specified and returns its colour
* Note returned colour is a byte and represents a palette number (0-255) not a 24 bit RGB value
*********************************************************************************************/
int ReadAPixel(int x, int y)
{
    WAIT_FOR_GRAPHICS;

    GraphicsX1Reg = x;
    GraphicsY1Reg = y;
    GraphicsCommandReg = GetAPixel;     // Send "get pixel" command to graphics controller

    WAIT_FOR_GRAPHICS;                  // Wait for graphics to be done reading pixel
    return (int)(GraphicsColourReg);    // Return the colour
}

/*******************************************************************************************
* This function writes a single pixel to the x,y coords specified using the specified colour
* Note colour is a byte and represents a palette number (0-255) not a 24 bit RGB value
********************************************************************************************/
void WriteAPixel(int x, int y, int Colour)
{
    WAIT_FOR_GRAPHICS;

    GraphicsX1Reg = x;
    GraphicsY1Reg = y;
    GraphicsColourReg = Colour;      // Set pixel colour
    GraphicsCommandReg = PutAPixel;	 // Send "write pixel" command to graphics controller
}

/********************************************************************************************* 
 * This function draw a horizontal line, 1 pixel at a time starting at the x,y coords specified
*********************************************************************************************/
void HLine(int x1, int y1, int length, int Colour)
{
    WAIT_FOR_GRAPHICS;

    // Write coordinates
    GraphicsX1Reg = x1;
    GraphicsY1Reg = y1;
    GraphicsX2Reg = x1 + length;

    GraphicsColourReg = Colour;      // Set pixel colour
    GraphicsCommandReg = DrawHLine;  // Send "draw H line" command to graphics controller
}

/********************************************************************************************* 
 * This function draw a vertical line, 1 pixel at a time starting at the x,y coords specified
*********************************************************************************************/
void VLine(int x1, int y1, int length, int Colour)
{
    WAIT_FOR_GRAPHICS;

    // Write coordinates
    GraphicsX1Reg = x1;
    GraphicsY1Reg = y1;
    GraphicsY2Reg = y1 + length;

    GraphicsColourReg = Colour;      // Set pixel colour
    GraphicsCommandReg = DrawVLine;  // Send "draw V line" command to graphics controller
}

/********************************************************************************************* 
 * This function draws a hollow rectangle between the points specified
*********************************************************************************************/
void Rect(int x1, int y1, int x2, int y2, int Colour)
{
    WAIT_FOR_GRAPHICS;
    GraphicsX1Reg = x1;
    GraphicsY1Reg = y1;
    GraphicsX2Reg = x2;
    GraphicsColourReg = Colour;      // Set pixel colour
    GraphicsCommandReg = DrawHLine;	  // Send "draw H line" command to graphics controller

    WAIT_FOR_GRAPHICS;
    GraphicsX1Reg = x1;
    GraphicsY1Reg = y2;
    GraphicsX2Reg = x2;
    GraphicsColourReg = Colour;      // Set pixel colour
    GraphicsCommandReg = DrawHLine;  // Send "draw H line" command to graphics controller

    WAIT_FOR_GRAPHICS;
    GraphicsX1Reg = x1;
    GraphicsY1Reg = y1;
    GraphicsY2Reg = y2;
    GraphicsColourReg = Colour;      // Set pixel colour
    GraphicsCommandReg = DrawVLine;  // Send "draw V line" command to graphics controller

    WAIT_FOR_GRAPHICS;
    GraphicsX1Reg = x2;
    GraphicsY1Reg = y1;
    GraphicsY2Reg = y2;
    GraphicsColourReg = Colour;      // Set pixel colour
    GraphicsCommandReg = DrawVLine;  // Send "draw V line" command to graphics controller
}

/********************************************************************************************* 
 * This function draws a filled rectangle between the points specified
*********************************************************************************************/
void FillRect(int x1, int y1, int x2, int y2, int Colour)
{
    for (int y = y1; y <= y2; y++) {
        WAIT_FOR_GRAPHICS;

        // Write coordinates
        GraphicsX1Reg = x1;
        GraphicsY1Reg = y;
        GraphicsX2Reg = x2;

        GraphicsColourReg = Colour;      // Set pixel colour
        GraphicsCommandReg = DrawHLine;  // Send "draw H line" command to graphics controller
    }
}

/*******************************************************************************
** Implementation of Bresenhams algorithms
*******************************************************************************/
void Line(int x1, int y1, int x2, int y2, int Colour)
{
    WAIT_FOR_GRAPHICS;

    // Write coordinates
    GraphicsX1Reg = x1;				
    GraphicsY1Reg = y1;
    GraphicsX2Reg = x2;
    GraphicsY2Reg = y2;

    GraphicsColourReg = Colour;      // Set pixel colour
    GraphicsCommandReg = DrawLine;   // Send "draw line" command to graphics controller
}

void Triangle(int x1, int y1, int x2, int y2, int x3, int y3, int Colour)
{
    WAIT_FOR_GRAPHICS;
    GraphicsX1Reg = x1;				
    GraphicsY1Reg = y1;
    GraphicsX2Reg = x2;
    GraphicsY2Reg = y2;
    GraphicsColourReg = Colour;      // Set pixel colour
    GraphicsCommandReg = DrawLine;   // Send "draw line" command to graphics controller

    WAIT_FOR_GRAPHICS;
    GraphicsX1Reg = x1;				
    GraphicsY1Reg = y1;
    GraphicsX2Reg = x3;
    GraphicsY2Reg = y3;
    GraphicsColourReg = Colour;      // Set pixel colour
    GraphicsCommandReg = DrawLine;   // Send "draw line" command to graphics controller

    WAIT_FOR_GRAPHICS;
    GraphicsX1Reg = x2;				
    GraphicsY1Reg = y2;
    GraphicsX2Reg = x3;
    GraphicsY2Reg = y3;
    GraphicsColourReg = Colour;      // Set pixel colour
    GraphicsCommandReg = DrawLine;   // Send "draw line" command to graphics controller
}

void Circle(int x1, int y1, int radius, int Colour)
{
    WAIT_FOR_GRAPHICS;

    // Write coordinates
    GraphicsX1Reg = x1;				
    GraphicsY1Reg = y1;
    GraphicsX2Reg = radius;

    GraphicsColourReg = Colour;      // Set pixel colour
    GraphicsCommandReg = DrawCircle; // Send "draw circle" command to graphics controller
}

// NOTE: DOES NOT WORK AS WELL AS Circle() FOLLOWED BY Fill()
// Draws concentric circles with decreasing radii
void FillCircle(int x1, int y1, int radius, int Colour)
{
    for (int i = radius; i > 0; i--) {
        WAIT_FOR_GRAPHICS;

        // Write coordinates
        GraphicsX1Reg = x1;				
        GraphicsY1Reg = y1;
        GraphicsX2Reg = i;

        GraphicsColourReg = Colour;      // Set pixel colour
        GraphicsCommandReg = DrawCircle; // Send "draw circle" command to graphics controller
    }
}
