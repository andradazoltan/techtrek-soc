/*
 *  WiFi.h
 *
 *  Created on: Jan 30, 2020
 *  Author: Connor Fong & Ash Tan
 */

#ifndef WIFI_H_
#define WIFI_H_

// Function Definitions
void InitWIFI(void);
int WIFITestForReceivedData(void);
void WIFI_Flush(void);

//Server Functions
void lua_doServerFile(void);
char* lua_postGPS(float latitude, float longitude);
char* lua_getWeather(void);
char* lua_postHelp(void);
char* lua_getPopulation(void);
char* lua_getWarnings(void);

#endif /* WIFI_H_ */
