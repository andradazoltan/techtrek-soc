/*
 *  WiFi.c
 *  Drives UART using UART
 *
 *  Created on: Jan 30, 2020
 *  Author: Connor Fong & Ash Tan
 */

#include "Wifi.h"
#include "RegisterDefs.h"
#include <stdio.h>
#include <string.h>

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
    int counter = 0;
    char cmd[] = "dofile(\"server.lua\")\r\n";

    while (cmd[counter] != '\0') {          //Put message onto buffer
            putcharWIFI(cmd[counter]) ;
            counter++;
    }

    for(int i = 0; i < 100000 /* Timeout */; i++) {
        if (WIFITestForReceivedData()) {
            (void)getcharWIFI();
			i = 0;
        }
    }

}

void lua_postGPS(int latitude, int longitude) {
    int counter = 0;
    char cmd[50];

    snprintf(cmd, sizeof(cmd), "post_GPS(%.6d, %.6d)\r\n", latitude, longitude);

    while (cmd[counter] != '\0') {          //Put message onto buffer
            putcharWIFI(cmd[counter]) ;
            counter++;
    }
}

void lua_getWeather(char *response) {
    int counter = 0;
    char cmd[] = "get_weather()\r\n";

    while (cmd[counter] != '\0') {          //Put message onto buffer for wifi to receive
            putcharWIFI(cmd[counter]) ;
            counter++;
    }

    for(int i = 0; i < 100000 /* Timeout */; i++) { //Get response 
        if (WIFITestForReceivedData()) {
            char data = (char)getcharWIFI();
            strcat(response, &data);        //Add character to response string
			i = 0;
        }
    }
}


void lua_checkwifi(void)
{
    char cmd[] = "check_wifi()\r\n";

    for (int i = 0; i < 14; i++) {
        putcharWIFI(cmd[i]);
    } 

	for(int i = 0; i < 100000 /* Timeout */; i++) {
        if (WIFITestForReceivedData()) {
			(void)getcharWIFI();
			i = 0;
        }
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
