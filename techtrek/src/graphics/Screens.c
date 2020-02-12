/*
 * Screens.c
 *
 *  Created on: Feb 11, 2020
 *  Author: Andrada Zoltan
 */

#include "Screens.h"
#include "Graphics.h"
#include "Wifi.h"
#include "Fonts.h"

int currScreen = MAIN_SCREEN;

void createObjects(object_t objs[], int numObjs);

/*
 * Main Screen Objects
 */
object_t mainScreen[] = {
    { // Info Button
        .type = RECT,
        .colour = CHOCOLATE,
        .text = "INFO",
        .textXCoord = 630,
        .func = NULL,
        .rect = {
            .topLeftXCoord = 565,
            .topLeftYCoord = 15,
            .bottomRightXCoord = 770,
            .bottomRightYCoord = 93
        }
    },
    { // Warnings Button
        .type = RECT,
        .colour = CHOCOLATE,
        .text = "WARNINGS",
        .textXCoord = 610,
        .func = NULL,
        .rect = {
            .topLeftXCoord = 565,
            .topLeftYCoord = 108,
            .bottomRightXCoord = 770,
            .bottomRightYCoord = 186
        }
    },
    { // Map Button
        .type = RECT,
        .colour = CHOCOLATE,
        .text = "MAP",
        .textXCoord = 635,
        .func = NULL,
        .rect = {
            .topLeftXCoord = 565,
            .topLeftYCoord = 201,
            .bottomRightXCoord = 770,
            .bottomRightYCoord = 279
        }
    },
    { // Report Hazard Button
        .type = RECT,
        .colour = RED,
        .text = "REPORT HAZARD",
        .textXCoord = 570,
        .func = NULL,
        .rect = {
            .topLeftXCoord = 565,
            .topLeftYCoord = 294,
            .bottomRightXCoord = 770,
            .bottomRightYCoord = 372
        }
    },
    { // Help Button
        .type = RECT,
        .colour = RED,
        .text = "HELP",
        .textXCoord = 630,
        .func = &lua_postHelp,
        .rect = {
            .topLeftXCoord = 565,
            .topLeftYCoord = 387,
            .bottomRightXCoord = 770,
            .bottomRightYCoord = 465
        }
    }
};



void initColours(void) {
    ProgramPalette(CHOCOLATE, 0x00683707);
    ProgramPalette(DARK_GREEN, 0x00005800);
    ProgramPalette(RED, 0x00FF0000);
}

void drawMainScreen(void) {
    // Fill the screen with a solid green colour
    FillScreen(GREEN);

    // Create a panel on the right of the screen
    FillRect(550, 0, XRES, YRES, DARK_GREEN);


    // Create five buttons, these need to be tracked
    createObjects(mainScreen, 5);

    // Print trail name
    OutGraphicsCharFont4(140, 40, WHITE, GREEN, "Pacific Spirit", 0);
    OutGraphicsCharFont4(100, 90, WHITE, GREEN, "Regional Park Trail", 0);

    // Draw a tree

    // Print weather at the bottom
}

void createObjects(object_t objs[], int numObjs) {
    for (int i = 0; i < numObjs; i++) {
        object_t temp = objs[i];

        if (temp.type == RECT) {
            FillRect(temp.rect.topLeftXCoord, temp.rect.topLeftYCoord,
                    temp.rect.bottomRightXCoord, temp.rect.bottomRightYCoord, temp.colour);

            int yCoord = (temp.rect.topLeftYCoord + temp.rect.bottomRightYCoord) / 2 - (FONT3_YPIXELS / 2);
            OutGraphicsCharFont3(temp.textXCoord, yCoord, WHITE, temp.colour, temp.text, 0);
        }
        else if (temp.type == CIRCLE) {
            FillCircle(temp.circle.centerXCoord, temp.circle.centerYCoord, temp.circle.radius, temp.colour);
        }
    }
}

int IsObjectPressed(int x, int y, object_t obj) {
    if (obj.type == RECT) {
        return (obj.rect.topLeftXCoord <= x && obj.rect.bottomRightXCoord >= x &&
                obj.rect.topLeftYCoord <= y && obj.rect.bottomRightYCoord >= y);
    }
    else
        return 0;
}
