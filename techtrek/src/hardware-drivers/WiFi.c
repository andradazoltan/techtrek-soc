/*
 *  WiFi.cpp
 *  Drives UART using UART
 *
 *  Created on: Jan 30, 2020
 *  Author: Connor Fong
 */

#define WIFI_ReceiverFifo (*(volatile unsigned char *)(0xFF210200))
#define WIFI_TransmitterFifo (*(volatile unsigned char *)(0xFF210200))
#define WIFI_InterruptEnableReg (*(volatile unsigned char *)(0xFF210202))
#define WIFI_InterruptIdentificationReg (*(volatile unsigned char *)(0xFF210204))
#define WIFI_FifoControlReg (*(volatile unsigned char *)(0xFF210204))
#define WIFI_LineControlReg (*(volatile unsigned char *)(0xFF210206))
#define WIFI_ModemControlReg (*(volatile unsigned char *)(0xFF210208))
#define WIFI_LineStatusReg (*(volatile unsigned char *)(0xFF21020A))
#define WIFI_ModemStatusReg (*(volatile unsigned char *)(0xFF21020C))
#define WIFI_ScratchReg (*(volatile unsigned char *)(0xFF21020E))
#define WIFI_DivisorLatchLSB (*(volatile unsigned char *)(0xFF210200))
#define WIFI_DivisorLatchMSB (*(volatile unsigned char *)(0xFF210202))



#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <string.h>

/* -----------Function Definitions ------------*/ 
void Init_WIFI(void);
int putcharWIFI(int c);
int getcharWIFI( void );
int WIFITestForReceivedData(void);
void WIFI_Flush(void);
void lua_dofile(void);

void lua_doServerFile();
void lua_postGPS(int latitude, int longitude);
char[] lua_getWeather();

void lua_checkwifi();

//Test GPS Coordinates
    // Mt Baker 48.777342,-121.813200
    // Whistler 50.116322,-122.957359
    // La Foret 48.777342,-122.995400

/* ----------- Main Function -----------------*/
int main (void) {
    Init_WIFI();				// Initialize the port
	WIFI_Flush();
    // lua_dofile();
    // lua_checkwifi();

    lua_doServerFile();
    
    lua_postGPS(50.116322,-122.957359); 
    lua_get_weather();

    while(1) {
        char data;
        if (WIFITestForReceivedData()) {
            data = (char) getcharWIFI();
            printf("%c", data);
        }
    }
}




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
    char cmd[50] = snprintf(cmd, sizeof(cmd), "post_GPS(%.6f, %.6f)\r\n", latitude, longitude);

    while (cmd[counter] != '\0') {          //Put message onto buffer
            putcharWIFI(cmd[counter]) ;
            counter++;
    }
}

char[] lua_getWeather() {
    int counter = 0;
    char cmd[] = "get_weather()\r\n";
    char response[100] = "";

    while (cmd[counter] != '\0') {          //Put message onto buffer for wifi to receive
            putcharWIFI(cmd[counter]) ;
            counter++;
    }

    for(int i = 0; i < 100000 /* Timeout */; i++) { //Get response 
        if (WIFITestForReceivedData()) {
            strcat(response, getcharWIFI());        //Add character to response string
			i = 0;
        }
    }

    return response;
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
	    return true;
	} else {
	    return false;
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
