/*
 *  WiFi.cpp
 *  Drives UART using UART
 *
 *  Created on: Jan 30, 2020
 *  Author: Connor Fong
 */

#include "WiFi.h"
#include "RegisterDefs.h"

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

void lua_dofile(void)
{
    char cmd[] = "dofile(\"send_text_message.lua\")\r\n";

    for (int i = 0; i < 32; i++) {
        putcharWIFI(cmd[i]);
    }

	for(int i = 0; i < 100000 /* Timeout */; i++) {
        if (WIFITestForReceivedData()) {
            (void)getcharWIFI();
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
