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
int putcharWIFI(int c);
int getcharWIFI(void);
int WIFITestForReceivedData(void);
void WIFI_Flush(void);

void lua_doServerFile();
void lua_postGPS(int latitude, int longitude);
void lua_getWeather(char *response);

void lua_checkwifi(void);

#endif /* WIFI_H_ */
