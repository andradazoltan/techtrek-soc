/*
 * Screens.c
 *
 * Creates different screens for the GUI
 *
 * Created on: Feb 11, 2020
 * Author: Andrada Zoltan
 */

#include "Screens.h"
#include "Graphics.h"
#include "Wifi.h"
#include "Keyboard.h"
#include <string.h>

int currScreen = MAIN_SCREEN;

// to be used by pointer
int16_t graphPercent = 50;

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
        .func = &drawMapScreen,
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
        .func = &drawHazardScreen,
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
        .textXCoord= 630,
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
        .func = (void*)&lua_postHelp,
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
        .func = (void*)&lua_postHelp,
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
        .func = (void*)&lua_postHelp,
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
        .func = (void*)&lua_postHelp,
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
        .func = (void*)&lua_postHelp,
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
        .func = (void*)&lua_postHelp,
        .rect = {
            .topLeftXCoord = 530,
            .topLeftYCoord = 265,
            .bottomRightXCoord = 735,
            .bottomRightYCoord = 343
        }
    }
};

object_t mapScreen[] = {
    { // horizontal bar graph
        .type = GRAPH_HOR,
        .colour = ORANGE,
        .func = NULL,
        .graph_hor = {
            .topLeftXCoord = 50,
            .topLeftYCoord = 120,
            .bottomRightXCoord = 350,
            .bottomRightYCoord = 195,
            .percent = &graphPercent
        }
    },
    { // Back Button
        .type = RECT,
        .colour = CADET_BLUE,
        .text = "<- BACK",
        .textXCoord = 660,
        .func = &drawMainScreen,
        .rect = {
            .topLeftXCoord = 580,
            .topLeftYCoord = 0,
            .bottomRightXCoord = 780,
            .bottomRightYCoord = 75
        }
    },
    { // decrement button
        .type = RECT,
        .colour = BLACK,
        .text = "Worse",
        .textXCoord = 60,
        .func = &shiftGraphLeft,
        .rect = {
            .topLeftXCoord = 50,
            .topLeftYCoord = 220,
            .bottomRightXCoord = 190,
            .bottomRightYCoord = 298
        }
    },
    { // increment Button
        .type = RECT,
        .colour = BLACK,
        .text = "Better",
        .textXCoord = 220,
        .func = &shiftGraphRight,
        .rect = {
            .topLeftXCoord = 210,
            .topLeftYCoord = 220,
            .bottomRightXCoord = 350,
            .bottomRightYCoord = 298
        }
    }
};

/************************* Functions ***************************/
void initColours(void) {
    ProgramPalette(CHOCOLATE, 0x00683707);
    ProgramPalette(DARK_GREEN, 0x00005800);
    ProgramPalette(RED, 0x00FF0000);
    ProgramPalette(CADET_BLUE, 0x005F9EA0);
    ProgramPalette(WHITE_SMOKE, 0x00F5F5F5);
    ProgramPalette(PALE_GREEN, 0x0098FB98);
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

    // Print weather at the bottom
    char weather[1024] = {0};
    lua_getWeather(weather);

    char *tok = strtok(&weather[16], "a");
    OutGraphicsCharFont3(50, 300, WHITE, GREEN, tok, 0);
    tok = strtok(NULL, "");
    OutGraphicsCharFont3(50, 400, WHITE, GREEN, tok + 3 * sizeof(char), 0);
}

void drawHazardScreen(void) {
    currScreen = HAZARD_SCREEN;

    // Fill the screen with a solid red colour
    FillScreen(RED);

    // Title
    OutGraphicsCharFont4(240, 20, WHITE, RED, "Report a Hazard", 0);

    // Draw the full keyboard
    drawKeyboard(34, 250);

    // Draw the text box
    drawTextBox(65, 100, 750, 170);
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

void drawMapScreen(void) {
    currScreen = MAP_SCREEN;

    // Fill the screen with a solid "white smoke" colour
    FillScreen(WHITE_SMOKE);

    // Header
    FillRect(0, 0, XRES, 75, CADET_BLUE);
    OutGraphicsCharFont3(50, 14, WHITE, POWDER_BLUE, "Trail Map", 0);

    // Draw fake map (for now)
    FillRect(400, 80, 770, 450, GREEN);
    Line(400, 80, 770, 300, CHOCOLATE);
    Line(400, 400, 770, 350, BROWN);
    Line(550, 450, 720, 60, CHOCOLATE);
    Circle(650, 100, 20, CHOCOLATE);
    FillCircle(450, 200, 30, BLUE);
    OutGraphicsCharFont5(520, 220, WHITE, BLACK, "MAP", 0);

    // text over graph
    OutGraphicsCharFont3(50, 80, BLACK, WHITE_SMOKE, "How is the trail today?", 0);
    
    // Create buttons and graphs
    createObjects(mapScreen, 4);
}

void shiftGraphLeft(void) {
    graphPercent = graphPercent - 10;
    createObjects(mapScreen, 4);
}

void shiftGraphRight(void) {
    graphPercent = graphPercent + 10;
    createObjects(mapScreen, 4);
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
            Fill(temp.circle.centerXCoord, temp.circle.centerYCoord, temp.colour, temp.colour);
        }
        else if (temp.type == GRAPH_HOR) {
            barGraphHor(temp.graph_hor.topLeftXCoord, temp.graph_hor.topLeftYCoord,
                    temp.graph_hor.bottomRightXCoord, temp.graph_hor.bottomRightYCoord,
                    temp.colour, WHITE, *temp.graph_hor.percent);
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
