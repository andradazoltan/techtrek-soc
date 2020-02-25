/*
 *  WiFi.c
 *  Drives WiFi using UART
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

static int putcharWIFI(int c);
static int getcharWIFI(void);
static char* WIFI_SaveFlush(void);
static void sendCommand(char *command);
static void WIFI_wait(int cycles);

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
    int i;
    char* cmd = "dofile(\"server.lua\")\r\n";

    sendCommand(cmd);

    for(i = 0; i < 400000 /* Timeout */; i++) {
        if (WIFITestForReceivedData()) {
            (void)getcharWIFI();
            i = 0;
        }
    }
}

char* lua_postGPS(float latitude, float longitude) {
    char cmd[50];
    char *flushbuf = "";

    //Multiply by 1000000 to avoid passing a float
    snprintf(cmd, sizeof(cmd), "post_gps(%d, %d)\r\n", (int)(latitude*1000000), (int)(longitude*1000000));

    printf(cmd);
    sendCommand(cmd);

    flushbuf = WIFI_SaveFlush(); // This saves the response from the WIFI chip (Important!)

    return flushbuf;
}


char* lua_getWeather() {
    char cmd[] = "get_weather()\r\n";
    char *flushbuf = "";

    sendCommand(cmd);
    flushbuf = WIFI_SaveFlush(); // This saves the response from the WIFI chip (Important!)

    return flushbuf;

}

char* lua_getPopulation() {
    char cmd[] = "get_population()\r\n";
    char *flushbuf = "";

    sendCommand(cmd);
    flushbuf = WIFI_SaveFlush(); // This saves the response from the WIFI chip (Important!)

    return flushbuf;
}

char* lua_getWarnings() {
    char cmd[] = "get_warnings()\r\n";
    char *flushbuf = "";

    sendCommand(cmd);
    flushbuf = WIFI_SaveFlush(); // This saves the response from the WIFI chip (Important!)

    return flushbuf;
}

char* lua_postHelp(void) {
    char cmd[] = "post_help()\r\n";
    char *flushbuf = "";

    printf(cmd);
    sendCommand(cmd);

    flushbuf = WIFI_SaveFlush(); // This saves the response from the WIFI chip (Important!)

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

    return flushbuf;
}

/************************** NOT IMPLEMENTED YET ********************/
char* lua_postWarning(float latitude, float longitude) {
    char cmd[50];
    char *flushbuf = "";

    //Multiply by 1000000 to avoid passing a float
    snprintf(cmd, sizeof(cmd), "post_gps(%d, %d)\r\n", (int)(latitude*1000000), (int)(longitude*1000000));

    printf(cmd);
    sendCommand(cmd);

    flushbuf = WIFI_SaveFlush(); // This saves the response from the WIFI chip (Important!)

    return flushbuf;
}

/************ HELPER FUNCTIONS ******************/
static void sendCommand(char *command)
{
    int i,j;
    for (i = 0; i < strlen(command); i++)
    {
        for(j = 0; j < 1000; j++) {;} //Give time to send signal
        putcharWIFI(command[i]);
    }
}

static void WIFI_wait(int cycles)
{
    for (int i = 0; !WIFITestForReceivedData() && i < cycles; i++);
}

/*----------- SAVE CHARACTER STREAM FROM WIFI CHIP ------------------*/
static char* WIFI_SaveFlush()
{
    int i;
    int bytes_received = 0;
    char buf[1024] = "";  // Increase size of buffer if you need to save more!!!

    for(i = 0; i < 10000000 ; i++) { /* Timeout */
        if (WIFITestForReceivedData() == 1) {
            buf[bytes_received++] = (char) getcharWIFI();
            i = 0;
        }
    }
    printf("Got %u bytes \n", bytes_received);
    buf[bytes_received] = '\0';
    return buf;
}

/*------------------ Serial Port Functions for WIFI -----------------*/
static int putcharWIFI(int c)
{
    // Do nothing while waiting for Transmitter Holding Register bit to change
    while((WIFI_LineStatusReg & 0x20) != 0x20);

	WIFI_TransmitterFifo = (char)c;         // Write character to Transmitter FIFO register
	return c;
}

static int getcharWIFI(void)
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
