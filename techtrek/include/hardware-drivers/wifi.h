/*
 *  wifi.h
 *
 *  Created on: Jan 30, 2020
 *  Author: Connor Fong & Ash Tan
 */

#ifndef WIFI_H_
#define WIFI_H_

// Function Definitions
void wifi_uart_init(void);
int WIFITestForReceivedData(void);
void wifi_uart_flush(void);

// Server Functions
void lua_doServerFile(void);
void lua_postGPS(float latitude, float longitude);
void lua_getWeather(char responseBody[]);
void lua_getPopulation(char responseBody[]);
void lua_postPopulation(int amount);
void lua_getWarnings(char responseBody[]);
void lua_postHelp(char helpMessage[]);
void lua_getRating(char responseBody[]);
void lua_postRating(int score);

#endif /* WIFI_H_ */
