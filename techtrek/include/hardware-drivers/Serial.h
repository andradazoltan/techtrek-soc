/*
 *  Serial.h
 *
 *  Created on: Jan 23, 2020
 *  Author: Ash Tan
 */

#ifndef SERIAL_H_
#define SERIAL_H_

// Function Definitions
void InitRS232(void);
int putcharRS232(int c);
int getcharRS232(void);
int RS232TestForReceivedData(void);
void RS232Flush(void);

#endif /* SERIAL_H_ */
