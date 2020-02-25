/*
 *  OutGraphicsCharFont3.c
 *
 *  Created on: 12 February 2020
 *  Author: Andrada Zoltan
 */

#include "Fonts.h"
#include "Graphics.h"

/******************************************************************************************************************************
** This function draws a single ASCII character at the coord specified using the colour specified
** OutGraphicsCharFont2(100,100, RED, 'A', TRUE, FALSE, 1, 1) ;	// display upper case 'A' in RED at coords 100,100, erase background
** no scroll, scale x,y= 1,1
**
******************************************************************************************************************************/
void OutGraphicsCharFont3(int x, int y, int colour, int backgroundcolour, const char* text, int Erase)
{
	register int 	row,
					column,
					theX = x,
					theY = y ;
	register int 	pixels ;
	register char 	theColour = colour  ;
	register int 	BitMask,
					theRow, theColumn;

	for (int i = 0; text[i] != '\0'; i++) {
	    int theCharacter = text[i];

	    if(((short)(x) > (short)(XRES-1)) || ((short)(y) > (short)(YRES-1)))  // if start off edge of screen don't bother
	            continue;

        if(((short)(theCharacter) >= (short)(' ')) && ((short)(theCharacter) <= (short)('~'))) {            // if printable character
            theCharacter -= 0x20 ;                                                                          // subtract hex 20 to get index of first printable character (the space character)
            theRow = FONT3_YPIXELS;
            theColumn = FONT3_XPIXELS;

            for(row = 0; row < (theRow * 2) ; row ++)   {
                BitMask = 128;
                pixels = Font16x27[(theCharacter * FONT3_YPIXELS * 2) + row];

                // Fix x position based on if reading upper or lower bits
                int xPos;
                if (row % 2 == 0) {
                    xPos = theX;
                }
                else {
                    xPos = theX + 8;
                }

                for (column = 0; column < (theColumn / 2); column++) {
                    if (Erase == 1) {
                        WriteAPixel(xPos + column, theY+(row/2), backgroundcolour) ;
                    }
                    else if((pixels & BitMask))                                                      // if valid pixel, then write it
                        WriteAPixel(xPos + column, theY+(row/2), theColour);

                    BitMask = BitMask >> 1 ;
                }
            }
        }

        // Update the starting X position for the next character
        theX += (FONT3_XPIXELS - 1);
	}
}
