/*
 *  GraphData.c
 *
 *  Created on: Feb 12, 2020
 *  Author: Elijah Hewer
 */

#include "Graphics.h"
#include "RegisterDefs.h"
#include <stdlib.h>

/*
 *  This function creates a horizontal bar graph with a 1px border between the points specified 
 * 
 *  Requires a width and height of at least 3 pixels
 *  Percent is an int between 0 and 100
 */
void barGraphHor(int topLeftX, int topLeftY, int botRightX, int botRightY, int dataColour, int bgColour, int percent) {
    // check dimension requirement
    if ( (abs(topLeftX - botRightX) < 2) || (abs(topLeftY - botRightY) < 2) ) return;
    
    int lengthToGraph = ((botRightX-1) - (topLeftX+1)) * ((percent % 101)/100.0);
    FillRect(topLeftX, topLeftY, botRightX, botRightY, bgColour);
    FillRect(topLeftX+1, topLeftY+1, topLeftX+1+lengthToGraph, botRightY-1, dataColour);
}

/*
 *  This function creates a vertical bar graph with a 1px border between the points specified 
 * 
 *  Requires a width and height of at least 3 pixels
 *  Percent is an int between 0 and 100
 */
void barGraphVert(int topLeftX, int topLeftY, int botRightX, int botRightY, int dataColour, int bgColour, int percent) {
    // check dimension requirement
    if ( (abs(topLeftX - botRightX) < 2) || (abs(topLeftY - botRightY) < 2) ) return;
    
    int lengthToGraph = ((botRightY-1) - (topLeftY+1)) * ((percent % 101)/100.0);
    FillRect(topLeftX, topLeftY, botRightX, botRightY, bgColour);
    FillRect(topLeftX+1, botRightY-1-lengthToGraph, botRightX-1, botRightY-1, dataColour);
}
