/*
 *  WiFi.ch
 *
 *  Created on: Jan 30, 2020
 *  Author: Connor Fong
 */

#ifndef WIFI_H_
#define WIFI_H_

// Function Definitions
void InitWIFI(void);
void lua_dofile(void);
void lua_checkwifi(void);
int putcharWIFI(int c);
int getcharWIFI(void);
int WIFITestForReceivedData(void);
void WIFI_Flush(void);

#endif /* WIFI_H_ */
