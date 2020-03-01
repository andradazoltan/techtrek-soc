/*
 *  TouchScreen.h
 *
 *  Created on: Jan 28, 2020
 *  Author: Ash Tan
 */

#ifndef TOUCHSCREEN_H_
#define TOUCHSCREEN_H_

// A data type to hold a point/coordinate
typedef struct {
  int x, y;
} Point;

// Function Definitions
void touch_uart_init(void);
void ReadTouchScreen(void);

int ScreenTouched(void);
void WaitForTouch(void);
Point GetPress(void);
int ScreenReleased(void);
void WaitForRelease(void);
Point GetRelease(void);

#endif /* TOUCHSCREEN_H_ */
