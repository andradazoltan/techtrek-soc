/*
 * Screens.c
 *
 *  Created on: Feb 11, 2020
 *  Author: Andrada Zoltan
 */

#include "Screens.h"
#include "Graphics.h"
#include "Wifi.h"

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
        .func = &drawHelpScreen,
        .rect = {
            .topLeftXCoord = 565,
            .topLeftYCoord = 387,
            .bottomRightXCoord = 770,
            .bottomRightYCoord = 465
        }
    }
};

object_t helpScreen[] = {
    { //  Button
        .type = RECT,
        .colour = MAROON,
        .text = "Unconscious",
        .textXCoord = 90,
        .func = &lua_postHelp,
        .rect = {
            .topLeftXCoord = 70,
            .topLeftYCoord = 150,
            .bottomRightXCoord = 275,
            .bottomRightYCoord = 228
        }
    },
    { //  Button
        .type = RECT,
        .colour = MAROON,
        .text = "Broken Bone",
        .textXCoord = 320,
        .func = &lua_postHelp,
        .rect = {
            .topLeftXCoord = 300,
            .topLeftYCoord = 150,
            .bottomRightXCoord = 505,
            .bottomRightYCoord = 228
        }
    },
    { //  Button
        .type = RECT,
        .colour = MAROON,
        .text = "Major Cut",
        .textXCoord = 565,
        .func = &lua_postHelp,
        .rect = {
            .topLeftXCoord = 530,
            .topLeftYCoord = 150,
            .bottomRightXCoord = 735,
            .bottomRightYCoord = 228
        }
    },
    { //  Button
        .type = RECT,
        .colour = MAROON,
        .text = "Too Dark",
        .textXCoord = 110,
        .func = &lua_postHelp,
        .rect = {
            .topLeftXCoord = 70,
            .topLeftYCoord = 265,
            .bottomRightXCoord = 275,
            .bottomRightYCoord = 343
        }
    },
    { //  Button
        .type = RECT,
        .colour = MAROON,
        .text = "Exhaustion",
        .textXCoord = 325,
        .func = &lua_postHelp,
        .rect = {
            .topLeftXCoord = 300,
            .topLeftYCoord = 265,
            .bottomRightXCoord = 505,
            .bottomRightYCoord = 343
        }
    },
    { // Other Button
        .type = RECT,
        .colour = MAROON,
        .text = "Other",
        .textXCoord = 595,
        .func = &lua_postHelp,
        .rect = {
            .topLeftXCoord = 530,
            .topLeftYCoord = 265,
            .bottomRightXCoord = 735,
            .bottomRightYCoord = 343
        }
    }
};

/************************* Functions ***************************/
void initColours(void) {
    ProgramPalette(CHOCOLATE, 0x00683707);
    ProgramPalette(DARK_GREEN, 0x00005800);
    ProgramPalette(RED, 0x00FF0000);
}

void drawMainScreen(void) {
    currScreen = MAIN_SCREEN;

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

void drawHelpScreen(void) {
    currScreen = HELP_SCREEN;

    // Fill the screen with a solid red colour
    FillScreen(RED);

    // Title
    OutGraphicsCharFont3(240, 20, WHITE, RED, "What is your emergency?", 0);

    // Create 6 buttons
    createObjects(helpScreen, 6);
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
            // Don't use FillCircle() unless you want a trippy concentric circle
            Circle(temp.circle.centerXCoord, temp.circle.centerYCoord, temp.circle.radius, temp.colour);
            Fill(temp.circle.centerXCoord, temp.circle.centerYCoord, temp.circle.radius, temp.colour);
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
