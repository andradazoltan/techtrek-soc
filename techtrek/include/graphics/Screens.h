/*
 * Screens.h
 *
 *  Created on: Feb 11, 2020
 *  Author: Andrada Zoltan & Elijah Hewer
 */

#ifndef SCREENS_H_
#define SCREENS_H_

#include <stdint.h>
#include <stdio.h>
#include "Colours.h"
#include "Fonts.h"

extern int people_count;

/*
 * List of screens
 */
enum screens{
    MAIN_SCREEN,
    HAZARD_SCREEN,
    HELP_SCREEN,
    MAP_SCREEN,
    INFO_SCREEN,
    WARNINGS_SCREEN
};

/*
 * Enum defining the types of objects the user can interact
 * with on the screen.
 */
typedef enum {
    RECT,
    CIRCLE,
    GRAPH_HOR
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

    struct graph_hor
    {
        int16_t topLeftXCoord;
        int16_t topLeftYCoord;
        int16_t bottomRightXCoord;
        int16_t bottomRightYCoord;
        int16_t *percent;
    } graph_hor;
    
} object_t;

// External Variables
extern int currScreen;
extern object_t mainScreen[5];  // Main Screen Objects
extern object_t helpScreen[6];  // Help Screen Objects
extern object_t mapScreen[1];   // Map Screen Objects
extern object_t infoScreen[5];  // Info Screen Objects
extern object_t warningsScreen[1]; // Warnings Screen Objects

extern char warnings[5][42];
extern int currWarning;

// Function Definitions
void initColours(void);
void drawMainScreen(void);
void drawHazardScreen(void);
void drawHelpScreen(void);
void drawInfoScreen(void);
void drawMapScreen(void);
void drawWarningsScreen(void);
void shiftGraphLeft(void);
void shiftGraphRight(void);
int IsObjectPressed(int x, int y, object_t obj);

void sendEmergency(int x, int y);
void drawWarnings(void);

#endif /* SCREENS_H_ */
