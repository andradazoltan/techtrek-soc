/*
 *  TouchScreen.cpp
 *  Drives UART Touchscreen
 *
 *  Created on: Jan 28, 2020
 *  Author: Ash Tan
 */

#include "TouchScreen.h"
#include "RegisterDefs.h"
#include "Screens.h"

// Local Function Prototypes
static int putcharTS(int c);
static int getcharTS(void);
static int TS_TestForReceivedData(void);
static void TS_Flush(void);

/*****************************************************************************
** Initialize touch screen controller
*****************************************************************************/
void InitTouch(void)
{
    TouchScreen_LineControlReg |= 0x80;                             // Enable access to the baud rate registers
    TouchScreen_DivisorLatchLSB = 0x45;                             // Set Divisor latch (LSB and MSB) to get required baud rate
    TouchScreen_DivisorLatchMSB = 0x01;
    TouchScreen_LineControlReg = 0x03;                              // Configure other settings: 8 bit data, 1 stop bit, no parity, etc.
    TouchScreen_FifoControlReg = TouchScreen_FifoControlReg | 0x06; // Reset the Fifo's
    TouchScreen_FifoControlReg = TouchScreen_FifoControlReg ^ 0x06; // Clear all bits in the FiFo control registers

    putcharTS(0x55);
    putcharTS(0x01);
    putcharTS(0x12);
}

/*****************************************************************************
** Touch screen thread
*****************************************************************************/
void ReadTouchScreen(void) {
    while (1) {
        // Wait for screen to be touched and released
        WaitForTouch();
        WaitForRelease();

        Point p = GetRelease();

        object_t *objs;
        int numObjects;
        switch (currScreen) {
        case MAIN_SCREEN:
            objs = mainScreen;
            numObjects = 5;
            break;
        case HELP_SCREEN:
            objs = helpScreen;
            numObjects = 6;
            break;
        }

        for (int i = 0; i < numObjects; i++) {
            if (IsObjectPressed(p.x, p.y, objs[i])) {
                if (objs[i].func != NULL) {
                    objs[i].func();
                }
                break;
            
            case HELP_SCREEN:

                break;

        }

    }
}

/*****************************************************************************
** Test if screen touched
*****************************************************************************/
int ScreenTouched(void)
{
    char data = getcharTS();

    // Check for pen down (~bit 7 = 1 & bit 0 = 1)
    return ((data >> 7) % 2 == 1 && (data % 2) == 1);
}

/*****************************************************************************
** Wait for screen to be touched
*****************************************************************************/
void WaitForTouch(void)
{
    while(!ScreenTouched());
}

/*****************************************************************************
* This function waits for a touch screen press event and returns X,Y coord
*****************************************************************************/
Point GetPress(void)
{
    Point p1;

    // Wait for a pen down command
    WaitForTouch();

    // Get X,Y coordinate of the point
    char lowerX = getcharTS();
    char upperX = getcharTS();
    char lowerY = getcharTS();
    char upperY = getcharTS();
    float x = (upperX << 7) | lowerX;
    float y = (upperY << 7) | lowerY;

    // Calibrate the coordinate to the screen resolution
    x = x * 800 / 4096;
    y = y * 480 / 4096;
    p1.x = x;
    p1.y = y;
    
    return p1;
}

/*****************************************************************************
** Test if screen released
*****************************************************************************/
int ScreenReleased(void)
{
    char data = getcharTS();
    
    //Check for pen up (~bit 7 == 1 & bit 0 == 0)
    return ((data >> 7) % 2 == 1 && (data % 2) == 0);
}

/*****************************************************************************
** Wait for screen to be released
*****************************************************************************/
void WaitForRelease(void)
{
    while(!ScreenReleased());
}

/*****************************************************************************
* This function waits for a touch screen release event and returns X,Y coord
*****************************************************************************/
Point GetRelease(void)
{
    Point p1;
    
    // Wait for a pen up command
    WaitForRelease();

    // Get X,Y coordinate of the point
    char lowerX = getcharTS();
    char upperX = getcharTS();
    char lowerY = getcharTS();
    char upperY = getcharTS();
    float x = (upperX << 7) | lowerX;
    float y = (upperY << 7) | lowerY;

    // Calibrate the coordinate to the screen resolution
    x = x * 800 / 4096;
    y = y * 480 / 4096;
    p1.x = x;
    p1.y = y;

    return p1;
}


/*------------------ Serial Port Functions for TouchScreen(TS) -----------------*/
static int putcharTS(int c)
{
    // Do nothing while waiting for Transmitter Holding Register bit to change
    while((TouchScreen_LineStatusReg & 0x20) != 0x20);

    TouchScreen_TransmitterFifo = (char)c; 			// Write character to Transmitter FIFO register
	return c;
}


static int getcharTS(void)
{
    // Do nothing while waiting for the Data Ready bit to change
    while((TouchScreen_LineStatusReg & 0x1) != 0x01);

    return (int) TouchScreen_ReceiverFifo;		// Read new character from ReceiverFiFo register
}

static int TS_TestForReceivedData(void)
{
    // Check if any character has been received and return TRUE if so
    if ((TouchScreen_LineStatusReg & 0x1) == 0x1) {
	    return 1;
	} else {
	    return 0;
	}
}

static void TS_Flush(void)
{
    int flushedData;

	// Flush UART receive buffer
	while((TouchScreen_LineStatusReg & 0x01) == 0x01) {
	    flushedData = TouchScreen_ReceiverFifo;
	}
}
