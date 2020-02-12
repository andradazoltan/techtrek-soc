/*
 *  Serial.c
 *  Drives UART RS232
 *
 *  Created on: Jan 23, 2020
 *  Author: Ash Tan
 */

#include "Serial.h"
#include "RegisterDefs.h"

/**************************************************************************
** Subroutine to initialize the RS232 Port by writing some data
** to the internal registers.
** Call this function at the start of the program before you attempt
** to read or write to data via the RS232 port
**
** Refer to UART data sheet for details of registers and programming
***************************************************************************/
void InitRS232(void)
{
	RS232_LineControlReg |= 0x80;                       // Enable access to the baud rate registers
	RS232_DivisorLatchLSB = 0x45;                       // Set Divisor latch (LSB and MSB) to get required baud rate
	RS232_DivisorLatchMSB = 0x01;
	RS232_LineControlReg = 0x03;                        // Configure other settings: 8 bit data, 1 stop bit, no parity, etc.
	RS232_FifoControlReg = RS232_FifoControlReg | 0x06; // Reset the Fifo's
	RS232_FifoControlReg = RS232_FifoControlReg ^ 0x06; // Clear all bits in the FiFo control registers
}

int putcharRS232(int c)
{
    // Do nothing while waiting for Transmitter Holding Register bit to change
	while((RS232_LineStatusReg & 0x20) != 0x20);

	RS232_TransmitterFifo = (char)c;            // Write character to Transmitter FIFO register
	return c;
}

int getcharRS232(void)
{
    // Do nothing while waiting for the Data Ready bit to change
	while((RS232_LineStatusReg & 0x1) != 0x01);
	return (int) RS232_ReceiverFifo;        // Read new character from ReceiverFiFo register
}

int RS232TestForReceivedData(void)
{
    // Check if any character has been received and return TRUE if so
	if ((RS232_LineStatusReg & 0x1) == 0x1) {
		return 1;
	} else {
		return 0;
	}
}

void RS232Flush(void)
{
	int flushedData;

	// Flush UART receive buffer
	while((RS232_LineStatusReg & 0x01) == 0x01) {
		flushedData = RS232_ReceiverFifo;
	}
	return;
}
