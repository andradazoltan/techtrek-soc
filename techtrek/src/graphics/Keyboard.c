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
#define KEY_Y_SIZE          ((YRES - 40) / (3 * 4))          // Number of pixels a key can take up vertically
#define KEY_SPACING         3                         // Border around a key before next key can start

int keyboardXStart = 0;
int keyboardYStart = 0;

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
        .keyText = "ASDFGHJKL ",
        .topLeftYCoord = KEY_Y_SIZE + KEY_SPACING,
        .bottomRightYCoord = 2 * KEY_Y_SIZE - KEY_SPACING
    },
    {
        .keyText = "ZXCVBNM  \n",
        .topLeftYCoord = 2 * (KEY_Y_SIZE + KEY_SPACING),
        .bottomRightYCoord = 3 * KEY_Y_SIZE - KEY_SPACING
    }
};


/*
 *  Draw Keyboard
 */
void drawKeyboard(int x, int y) {
    for (int row = 0; row < 4; row++) {
        keys_t curr_row = keys[row];

        for (int key = 0; curr_row.keyText[key] != '\0'; key++) {
            char to_print[2] = {curr_row.keyText[key], '\0'};
            int x1 = x + KEY_SPACING + (key * KEY_X_SIZE);
            int y1 = y + curr_row.topLeftYCoord + (row * KEY_Y_SIZE);
            int x2 = x1 + KEY_X_SIZE - (2 * KEY_SPACING);
            int y2 = y1 + KEY_X_SIZE - (2 * KEY_SPACING);

            FillRect(x1, y1, x2, y2, MAROON);
            OutGraphicsCharFont3(x1 + KEY_X_SIZE / 2 - 10, y1 + KEY_Y_SIZE / 2, WHITE, MAROON, to_print, 0);
        }
    }

    keyboardXStart = x;
    keyboardYStart = y;
}

void keyPress(int x, int y) {
    keys_t row;
    int yMod = y - keyboardYStart;
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

    char c = row.keyText[(x - keyboardXStart) / KEY_X_SIZE];
    printf("%c\n",c);
}
