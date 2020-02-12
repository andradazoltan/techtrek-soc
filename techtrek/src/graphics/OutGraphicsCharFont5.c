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
void OutGraphicsCharFont5(int x, int y, int colour, int backgroundcolour, const char* text, int Erase)
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
            theRow = FONT5_YPIXELS;
            theColumn = FONT5_XPIXELS;

            for(row = 0; row < (theRow * 4) ; row ++)   {
                BitMask = 128;
                pixels = Font38x59[(theCharacter * FONT5_YPIXELS * 4) + row];

                // Fix x position based on if reading upper or lower bits
                int xPos;
                if (row % 4 == 0) {
                    xPos = theX;
                }
                else if (row % 4 == 1) {
                    xPos = theX + 8;
                }
                else if (row % 4 == 2){
                    xPos = theX + 16;
                }
                else {
                    xPos = theX + 24;
                }

                for (column = 0; column < 8; column++) {
                    if(Erase == 1)
                        WriteAPixel(xPos + column, theY+(row/4), backgroundcolour);
                    else if((pixels & BitMask))                                                // if valid pixel, then write it
                        WriteAPixel(xPos + column, theY+(row/4), theColour) ;

                    BitMask = BitMask >> 1 ;
                }
            }
        }

        // Update the starting X position for the next character
        theX += (FONT5_XPIXELS - 1);
	}
}
