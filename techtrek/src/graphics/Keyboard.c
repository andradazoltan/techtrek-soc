/*
 *  KeyBoard.c
 *
 *  Created on: Feb 16, 2020
 *  Author: Andrada Zoltan
 */

#include "Graphics.h"
#include "Colours.h"
#include "RegisterDefs.h"
#include "Fonts.h"
#include <stdint.h>

#define MAX_KEYS_PER_ROW    10                        // Maximum number of keys in a keyboard row (top row)
#define KEY_X_SIZE          ((XRES - 50) / MAX_KEYS_PER_ROW) // Number of pixels a key can take up horizontally
#define KEY_Y_SIZE          ((YRES - 40) / (2 * 3))          // Number of pixels a key can take up vertically
#define KEY_SPACING         3                         // Border around a key before next key can start
#define MAX_CHARS           42                        // Maximum characters in the text box

/*
 * Records the starting coordinate of the keyboard.
 * This is updated when drawKeyboard() is called.
 */
uint16_t keyboardXStart = 0;
uint16_t keyboardYStart = 0;

/*
 * Records the current position and length of the cursor.
 */
uint16_t cursorXPos = 0;
uint16_t cursorYPos = 0;
uint16_t cursorLen = 0;

/*
 * Records the bounding X-coordinates of the text box.
 * This is updated when drawTextBox() is called.
 */
uint16_t tbXStart = 0;
uint16_t tbXEnd = 0;

/*
 *
 */
char hazardBuf[MAX_CHARS] = {0};
uint8_t currIdx = 0;

typedef struct keys {
    char keyText[MAX_KEYS_PER_ROW];
    int16_t topLeftYCoord;
    int16_t bottomRightYCoord;
} keys_t;

keys_t keys[] = {
    {
        .keyText = "QWERTYUIOP",
        .topLeftYCoord = 0 + KEY_SPACING,
        .bottomRightYCoord = KEY_Y_SIZE - KEY_SPACING
    },
    {
        .keyText = "ASDFGHJKLx",
        .topLeftYCoord = KEY_Y_SIZE + KEY_SPACING,
        .bottomRightYCoord = 2 * KEY_Y_SIZE - KEY_SPACING
    },
    {
        .keyText = "ZXCVBNM  \n",
        .topLeftYCoord = 2 * KEY_Y_SIZE + KEY_SPACING,
        .bottomRightYCoord = 3 * KEY_Y_SIZE - KEY_SPACING
    }
};


/*
 *  Draw the keyboard
 */
void drawKeyboard(int x, int y) {
    for (int row = 0; row < 3; row++) {
        // Get the current row
        keys_t curr_row = keys[row];

        // For each key in the row, draw its surrounding box and text
        uint8_t firstSpace = 1;
        for (int key = 0; curr_row.keyText[key] != '\0'; key++) {
            // Position of the box
            int x1 = x + KEY_SPACING + (key * KEY_X_SIZE);
            int y1 = y + curr_row.topLeftYCoord;
            int x2 = x1 + KEY_X_SIZE - (2 * KEY_SPACING);
            int y2 = y + curr_row.bottomRightYCoord;

            FillRect(x1, y1, x2, y2, MAROON);

            char to_print[6] = {curr_row.keyText[key], '\0', '\0', '\0', '\0', '\0'};
            if (curr_row.keyText[key] == 'x') { // Backspace key
                to_print[0] = '<';
                to_print[1] = '-';

                OutGraphicsCharFont3(x1 + KEY_X_SIZE / 2 - 15, y1 + KEY_Y_SIZE / 2 - 15, WHITE, MAROON, to_print, 0);
            }
            else if (curr_row.keyText[key] == ' ') { // Space key
                if (firstSpace) {
                    firstSpace = 0;
                }
                else {
                    to_print[0] = 's';
                    to_print[1] = 'p';
                    to_print[2] = 'a';
                    to_print[3] = 'c';
                    to_print[4] = 'e';

                    // Draw the box larger to connect to previous space key
                    x1 -= (2 * KEY_SPACING);
                    FillRect(x1, y1, x2, y2, MAROON);
                    firstSpace = 0;

                    // Write out the key text
                    OutGraphicsCharFont3(x1 - KEY_X_SIZE / 2 + 5, y1 + KEY_Y_SIZE / 2 - 15, WHITE, MAROON, to_print, 0);
                }
            }
            else if (curr_row.keyText[key] == '\n') { // Enter key
                to_print[0] = 'O';
                to_print[1] = 'K';

                // Redraw box to be a different colour
                FillRect(x1, y1, x2, y2, PINK);

                OutGraphicsCharFont3(x1 + KEY_X_SIZE / 2 - 15, y1 + KEY_Y_SIZE / 2 - 15, WHITE, MAROON, to_print, 0);
            }
            else {
                OutGraphicsCharFont3(x1 + KEY_X_SIZE / 2 - 10, y1 + KEY_Y_SIZE / 2 - 15, WHITE, MAROON, to_print, 0);
            }
        }
    }

    keyboardXStart = x;
    keyboardYStart = y;
}

void drawTextBox(int x1, int y1, int x2, int y2) {
    // Draw the text box
    FillRect(x1, y1, x2, y2, BLACK);

    // Draw the cursor and keep track of it
    cursorXPos = x1 + 10;
    cursorYPos = y1 + 10;
    cursorLen = (y2 - y1 - 20);
    VLine(cursorXPos, cursorYPos, cursorLen, WHITE);

    // Record the box bounding coordinates
    tbXStart = x1;
    tbXEnd = x2;
}

/*
 * Called when a touch screen release has been detected while on the HAZARD_SCREEN.
 * This function will determine which key was released, and print it to the text box
 * while updating the cursor at the same time.
 */
void keyRelease(int x, int y) {
    keys_t row;

    // The recorded row Y-coordinates are relative to the starting position where the keyboard was drawn
    int yMod = y - keyboardYStart;

    // First find the row that was pressed
    if ((yMod >= keys[0].topLeftYCoord) && (yMod <= keys[0].bottomRightYCoord)) {
        row = keys[0];
    }
    else if ((yMod >= keys[1].topLeftYCoord) && (yMod <= keys[1].bottomRightYCoord)) {
        row = keys[1];
    }
    else if ((yMod >= keys[2].topLeftYCoord) && (yMod <= keys[2].bottomRightYCoord)) {
        row = keys[2];
    }
    else
        return;

    // Get the key in the row that was pressed
    char to_print[2] = {row.keyText[(x - keyboardXStart) / KEY_X_SIZE], '\0'};

    // Erase old cursor position
    VLine(cursorXPos, cursorYPos, cursorLen, BLACK);

    // Print the character to the screen and update the cursor
    if (to_print[0] == 'x') { // Backspace key
        if ((cursorXPos - FONT3_XPIXELS) > tbXStart) { // Don't let it erase beyond bounds of box
            cursorXPos -= FONT3_XPIXELS;
            OutGraphicsCharFont3(cursorXPos, cursorYPos + 10, WHITE, BLACK, to_print, 1);

            // Remove last character from the hazardBuf
            if (currIdx > 0) {
                currIdx--;
                hazardBuf[currIdx] = '\0';
            }
        }
    }
    else if (to_print[0] == '\n') { // Enter key
        // Clear the text box
        drawTextBox(tbXStart, cursorYPos - 10, tbXEnd, cursorYPos + cursorLen + 10);

        // Call wifi function

        // Clear the hazardBuf buffer
        for (int i = 0; i < MAX_CHARS; i++) {
            hazardBuf[i] = '\0';
        }
        currIdx = 0;
    }
    else {
        if ((cursorXPos + FONT3_XPIXELS) < tbXEnd) { // Don't let it print outside the box
            OutGraphicsCharFont3(cursorXPos, cursorYPos + 10, WHITE, BLACK, to_print, 0);
            cursorXPos += FONT3_XPIXELS;

            // Add a character to the hazardBuf
            hazardBuf[currIdx] = to_print[0];
            currIdx++;
        }
    }

    // Redraw new cursor position
    VLine(cursorXPos, cursorYPos, cursorLen, WHITE);
}
