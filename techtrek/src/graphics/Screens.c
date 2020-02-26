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
#include "WiFi.h"
#include "Keyboard.h"
#include <string.h>

// Extern variables
int people_count = 0;
int currScreen = MAIN_SCREEN;

// Current percentage displayed on ratings graph
int16_t graphPercent = 50;

// Warnings buffer
char warnings[5][42];
int currWarning = 0;
char ids[5][30];

// Private function prototypes
void createObjects(object_t objs[], int numObjs);
void sendRating(void);

/*
 * Main Screen Objects
 */
object_t mainScreen[] = {
    { // Info Button
        .type = RECT,
        .colour = CHOCOLATE,
        .text = "INFO",
        .textXCoord = 630,
        .func = &drawInfoScreen,
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
        .func = &drawWarningsScreen,
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
            .bottomRightYCoord = 70
        }
    }
};

object_t infoScreen[] = {
    { // horizontal bar graph
        .type = GRAPH_HOR,
        .colour = DARK_ORANGE,
        .func = NULL,
        .graph_hor = {
            .topLeftXCoord = 290,
            .topLeftYCoord = 320,
            .bottomRightXCoord = 590,
            .bottomRightYCoord = 395,
            .percent = &graphPercent
        }
    },
    { // decrement button
        .type = RECT,
        .colour = DARK_BLUE,
        .text = "Worse",
        .textXCoord = 320,
        .func = &shiftGraphLeft,
        .rect = {
            .topLeftXCoord = 290,
            .topLeftYCoord = 410,
            .bottomRightXCoord = 430,
            .bottomRightYCoord = 450
        }
    },
    { // increment Button
        .type = RECT,
        .colour = DARK_BLUE,
        .text = "Better",
        .textXCoord = 470,
        .func = &shiftGraphRight,
        .rect = {
            .topLeftXCoord = 450,
            .topLeftYCoord = 410,
            .bottomRightXCoord = 590,
            .bottomRightYCoord = 450
        }
    },
    { // Back Button
        .type = RECT,
        .colour = DARK_BLUE,
        .text = "<- BACK",
        .textXCoord = 660,
        .func = &drawMainScreen,
        .rect = {
            .topLeftXCoord = 580,
            .topLeftYCoord = 0,
            .bottomRightXCoord = 780,
            .bottomRightYCoord = 70
        }
    },
    { // Enter Button
        .type = RECT,
        .colour = CHOCOLATE,
        .text = "SUBMIT",
        .textXCoord = 645,
        .func = &sendRating,
        .rect = {
            .topLeftXCoord = 620,
            .topLeftYCoord = 410,
            .bottomRightXCoord = 760,
            .bottomRightYCoord = 450
        }
    },
};

object_t warningsScreen[] = {
    { // Back Button
        .type = RECT,
        .colour = RED,
        .text = "<- BACK",
        .textXCoord = 660,
        .func = &drawMainScreen,
        .rect = {
            .topLeftXCoord = 580,
            .topLeftYCoord = 0,
            .bottomRightXCoord = 780,
            .bottomRightYCoord = 70
        }
    }
};

/************************* Functions ***************************/
void initColours(void) {
    ProgramPalette(CHOCOLATE, 0x00683707);
    ProgramPalette(DARK_GREEN, 0x00005800);
    ProgramPalette(RED, 0x00FF0000);
    ProgramPalette(DARK_BLUE, 0x0006315A);
    ProgramPalette(DARK_SLATE_BLUE, 0x00141414);
    ProgramPalette(NAVY_REPEAT, 0x00050828);

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
    OutGraphicsCharFont4(90, 40, WHITE, GREEN, "Spanish Banks Trail", 0);
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

void drawInfoScreen(void) {
    currScreen = INFO_SCREEN;

    // Fill the screen with a solid colour
    FillScreen(NAVY_REPEAT);

    // Header
    FillRect(0, 0, XRES, 70, DARK_BLUE);

    // Make time box and fill in with time
    FillRect(65, 90, 315, 200, DARK_SLATE_BLUE);

    // Make date box and fill in with date
    FillRect(65, 220, 315, 300, DARK_SLATE_BLUE);

    // Make weather box
    FillRect(335, 90, 755, 170, DARK_SLATE_BLUE);

    // Make population box
    FillRect(335, 190, 755, 300, DARK_SLATE_BLUE);

    // text over graph
    OutGraphicsCharFont3(60, 350, WHITE, NAVY_REPEAT, "How is the", 0);
    OutGraphicsCharFont3(50, 400, WHITE, NAVY_REPEAT, "trail today?", 0);

    // Create buttons and graphs
    createObjects(infoScreen, 5);

    // Print weather heading
    OutGraphicsCharFont3(345, 100, GREEN, DARK_SLATE_BLUE, "WEATHER:", 0);

    // Get the people count and print it
    OutGraphicsCharFont3(345, 200, GREEN, DARK_SLATE_BLUE, "TRAIL POPULATION:", 0);
    char people[20];
    sprintf(people, "%d people", people_count);
    OutGraphicsCharFont4(450, 240, BLACK, DARK_SLATE_BLUE, people, 0);

    // Get the time and date
    OutGraphicsCharFont3(75, 100, GREEN, DARK_SLATE_BLUE, "TIME:", 0);
    OutGraphicsCharFont3(75, 230, GREEN, DARK_SLATE_BLUE, "DATE:", 0);

    // Get the weather and print it
    char weather[1024] = "";
    lua_getWeather(weather);
    OutGraphicsCharFont2(370, 130, BLACK, DARK_SLATE_BLUE, &weather[16], 0);
}

void drawMapScreen(void) {
    currScreen = MAP_SCREEN;

    // Fill the screen with a solid "white smoke" colour
    FillScreen(WHITE_SMOKE);

    // Header
    FillRect(0, 0, XRES, 70, CADET_BLUE);
    OutGraphicsCharFont3(50, 14, WHITE, POWDER_BLUE, "Trail Map", 0);

    // Draw fake map (for now)
    FillRect(400, 80, 770, 450, GREEN);
    Line(400, 80, 770, 300, CHOCOLATE);
    Line(400, 400, 770, 350, BROWN);
    Line(550, 450, 720, 60, CHOCOLATE);
    Circle(650, 100, 20, CHOCOLATE);
    FillCircle(450, 200, 30, BLUE);
    OutGraphicsCharFont5(520, 220, WHITE, BLACK, "MAP", 0);
    
    // Create buttons and graphs
    createObjects(mapScreen, 1);
}

void drawWarningsScreen(void) {
    currScreen = WARNINGS_SCREEN;

    // Fill the screen
    FillScreen(ORANGE);

    // Header
    FillRect(0, 0, XRES, 70, RED);
    OutGraphicsCharFont4(100, 14, WHITE, RED, "TODAY'S WARNINGS", 0);

    // Create objects
    createObjects(warningsScreen, 1);

    // Get the latest response
    char response[100];
    lua_getWarnings(response);

    // Index of previous warning
    int prevWarning = (currWarning == 0) ? 4 : currWarning - 1;

    // Pull out the ID of the warning and check that we haven't seen this one yet
    char *tok = strtok(response, "-");
    if (tok != NULL && (strcmp(ids[prevWarning], tok) != 0))
    {
        // Copy the id
        strcpy(ids[currWarning], tok);

        // Copy the actual message
        tok = strtok(NULL, "");
        strcpy(warnings[currWarning], tok);
        printf("%s\n", tok);
    }

    // Update the currWarning index
    if (currWarning == 4)
        currWarning = 0;
    else
        currWarning++;

    // Draw the warnings
    drawWarnings();
}

void shiftGraphLeft(void) {
    if (graphPercent != 0) {
        graphPercent = graphPercent - 10;
    }

    // Change colour to reflect percentage
    if (graphPercent < 25)
        infoScreen[0].colour = RED;
    else if (graphPercent < 50)
        infoScreen[0].colour = DARK_ORANGE;
    else if (graphPercent < 75)
        infoScreen[0].colour = ORANGE; // Looks like yellow
    else
        infoScreen[0].colour = GREEN;

    createObjects(infoScreen, 5);
}

void shiftGraphRight(void) {
    if (graphPercent != 100) {
        graphPercent = graphPercent + 10;
    }

    // Change colour to reflect percentage
    if (graphPercent < 25)
        infoScreen[0].colour = RED;
    else if (graphPercent < 50)
        infoScreen[0].colour = DARK_ORANGE;
    else if (graphPercent < 75)
        infoScreen[0].colour = ORANGE;
    else
        infoScreen[0].colour = GREEN;

    createObjects(infoScreen, 5);
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
                    temp.colour, BLACK, *temp.graph_hor.percent);
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


/*----------- Application Functions For When Button is Pressed --------*/

/*
 * Post a new trail rating to the server, and reset the graph to its
 * default value.
 */
void sendRating(void) {
    int score = graphPercent / 20;

    // Reset the graph
    graphPercent = 50;
    infoScreen[0].colour = DARK_ORANGE;

    createObjects(infoScreen, 5);

    // Send the rating
    lua_postRating(score);
}

/*
 * Determine which emergency button was called and call postHelp
 * with that argument.
 */
void sendEmergency(int x, int y) {
    for (int i = 0; i < 6; i++) {
        if (IsObjectPressed(x, y, helpScreen[i])) {
            char msg[42];
            sprintf(msg, "EMERGENCY: %s", helpScreen[i].text);

            // Replace all spaces with a period since space is a special character in a query
            for (int j = 0; j < 42; j++) {
                if (msg[j] == ' ')
                    msg[j] = '.';
            }
            lua_postHelp(msg);
        }
    }
}

/*
 * Print out the current list of warnings to the screen.
 */
void drawWarnings(void) {
    int y = 80;
    for (int i = 0; i < 5; i++) {
        OutGraphicsCharFont3(50, y, BLACK, ORANGE, warnings[i], 0);
        y += 80;
    }
}
