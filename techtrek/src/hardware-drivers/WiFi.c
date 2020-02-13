/*
 *  WiFi.c
 *  Drives UART using UART
 *
 *  Created on: Jan 30, 2020
 *  Author: Connor Fong & Ash Tan
 */

#include "Wifi.h"
#include "RegisterDefs.h"
#include "Screens.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**************************************************************************
** Subroutine to initialise the WIFI Port by writing some data
** to the internal registers.
** Call this function at the start of the program before you attempt
** to read or write to data via the WIFI port
**
** Refer to UART data sheet for details of registers and programming
***************************************************************************/
void InitWIFI(void)
{
    WIFI_LineControlReg |= 0x80;                      // Enable access to the baud rate registers
    WIFI_DivisorLatchLSB = 0x1B;                      // Set Divisor latch (LSB and MSB) to get required baud rate
    WIFI_DivisorLatchMSB = 0x00;
    WIFI_LineControlReg = 0x03;                       // Configure other settings: 8 bit data, 1 stop bit, no parity, etc.
    WIFI_FifoControlReg = WIFI_FifoControlReg | 0x06; // Reset the Fifo's
    WIFI_FifoControlReg = WIFI_FifoControlReg ^ 0x06; // Clear all bits in the FiFo control registers
}

/************ TECHTREK FUNCTIONS ******************/
void lua_doServerFile() {
    printf("In Lua do file\n");
    int counter = 0;
    int i;
    char* cmd = "dofile(\"server.lua\")\r\n";

    printf("Putting Message into Buffer\n");
    sendCommand(cmd);

    printf("Done putting message\n");
    for(i = 0; i < 100000 /* Timeout */; i++) {
        if (WIFITestForReceivedData()) {
            (void)getcharWIFI();
            i = 0;
        }
    }
}

void lua_postGPS(double latitude, double longitude) {
    int i = 0;
    char cmd[50];

    printf("HELLLLLO %d", (int)latitude*1000000);

    snprintf(cmd, sizeof(cmd), "post_gps(%d, %d)\r\n", (int)(latitude*1000000), (int)(longitude*1000000));

    printf(cmd);
    sendCommand(cmd);

    for(i = 0; i < 100000 /* Timeout */; i++) {
        if (WIFITestForReceivedData()) {
            char curr = (char) getcharWIFI();
            i = 0;
        }
    }
}

void lua_getWeather(char *response) {
    int counter = 0;
    int i;
    char cmd[] = "get_weather()\r\n";

    sendCommand(cmd);

    printf("In get weather\n");

    for(i = 0; i < 400000 /* Timeout */; i++) { //Get response
        if (WIFITestForReceivedData()) {
            strcat(response, (char*)getcharWIFI());        //Add character to response string
            i = 0;
        }
    }
    printf(response);
}

void lua_postHelp(void) {
    char cmd[] = "post_help()\r\n";

    printf(cmd);
    sendCommand(cmd);

    for(int i = 0; i < 100000 /* Timeout */; i++) {
        if (WIFITestForReceivedData()) {
            char curr = (char) getcharWIFI();
            i = 0;
        }
    }

    // Blink help message a few times
    for (int i = 0; i < 3; i++) {
        OutGraphicsCharFont5(65, 400, WHITE, RED, "HELP IS ON THE WAY!", 0);
        usleep(500 * 1000);
        OutGraphicsCharFont5(65, 400, WHITE, RED, "HELP IS ON THE WAY!", 1);
        usleep(500 * 1000);
    }
    OutGraphicsCharFont5(65, 400, WHITE, RED, "HELP IS ON THE WAY!", 0);

    // Wait a few seconds before going back to main screen
    sleep(3);

    // Transition back to main screen
    drawMainScreen();
}

void sendCommand(char *command)
{
    int i;
    for (i = 0; i < strlen(command); i++)
    {
        putcharWIFI(command[i]);
    }
}

/*------------------ Serial Port Functions for WIFI -----------------*/
int putcharWIFI(int c)
{
    // Do nothing while waiting for Transmitter Holding Register bit to change
    while((WIFI_LineStatusReg & 0x20) != 0x20);

	WIFI_TransmitterFifo = (char)c;         // Write character to Transmitter FIFO register
	return c;
}

int getcharWIFI(void)
{
    // Do nothing while waiting for the Data Ready bit to change
    while((WIFI_LineStatusReg & 0x1) != 0x01);

	return (int) WIFI_ReceiverFifo;     // Read new character from ReceiverFiFo register
}

int WIFITestForReceivedData(void)
{
    // Check if any character has been received and return TRUE if so
    if ((WIFI_LineStatusReg & 0x1) == 0x1) {
	    return 1;
	} else {
	    return 0;
	}
}

void WIFI_Flush()
{
    int flushedData;

    // Flush UART receive buffer
    while((WIFI_LineStatusReg & 0x01) == 0x01) {
	    flushedData = WIFI_ReceiverFifo;
	}
}
