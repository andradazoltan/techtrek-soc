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
void lua_postGPS(float latitude, float longitude);
void lua_getWeather(char responseBody[]);
void lua_getPopulation(char responseBody[]);
void lua_getWarnings(char responseBody[]);
void lua_postHelp(void);
void lua_getRating(char responseBody[]);
void lua_postRating(int score);

#endif /* WIFI_H_ */
