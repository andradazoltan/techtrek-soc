/*
 * Screens.h
 *
 *  Created on: Feb 11, 2020
 *  Author: Andrada Zoltan
 */

#ifndef SCREENS_H_
#define SCREENS_H_

#include <stdint.h>
#include <stdio.h>
#include "Colours.h"
#include "Fonts.h"

/*
 * List of screens
 */
enum screens{
    MAIN_SCREEN,
    HELP_SCREEN
};

/*
 * Enum defining the types of objects the user can interact
 * with on the screen.
 */
typedef enum {
    RECT,
    CIRCLE
} object_type_t;

/*
 * Objects that the user can interact with will be defined and tracked using
 * this struct. Allows us to keep track of where on the screen the object is
 * located and what action should be performed when the object is pressed.
 */
typedef struct
{
    object_type_t type;
    int colour;
    const char *text;
    int textXCoord;
    void (*const func)(void); // Pointer to function that is called when the object is pressed

    struct rect
    {
        int16_t topLeftXCoord;
        int16_t topLeftYCoord;
        int16_t bottomRightXCoord;
        int16_t bottomRightYCoord;
    } rect;

    struct circle
    {
        int16_t centerXCoord;
        int16_t centerYCoord;
        int16_t radius;
    } circle;
    
} object_t;

// External Variables
extern int currScreen;
extern object_t mainScreen[5]; // Main Screen Objects
extern object_t helpScreen[6]; // Help Screen Objects

// Function Definitions
void initColours(void);
void drawMainScreen(void);
void drawHelpScreen(void);
int IsObjectPressed(int x, int y, object_t obj);

#endif /* SCREENS_H_ */
