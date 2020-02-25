/*
 *  WiFi.cpp
 *  Drives UART using UART
 *
 *  Created on: Jan 30, 2020
 *  Author: Connor Fong & Ash Tan
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
void InitWIFI(void);
int putcharWIFI(int c);
int getcharWIFI( void );
int WIFITestForReceivedData(void);
void WIFI_Flush(void);
void lua_dofile(void);

//Server Functions
void lua_doServerFile(void);
char* lua_postGPS(float latitude, float longitude);
char* lua_getWeather(void);
char* lua_postHelp(void);
char* lua_getPopulation(void);
char* lua_getWarnings(void);
char* lua_postRating(int score);
char* lua_getRating(void);

//Helper Functions
char* WIFI_SaveFlush(void);
void sendCommand(char *command);
void WIFI_wait(int cycles);




//Test GPS Coordinates
    // Mt Baker 48.777342,-121.813200   float lat = 48.777342; float longitude = -121.813200;
    // Whistler 50.116322,-122.957359
    // La Foret 48.777342,-122.995400 
    // Dubai    25.276987, 55.296249    float lat = 25.276987; float longitude = 55.296249;

/* ----------- Main Function -----------------*/
int main (void) {

    char* getWeatherResponse;
    float lat = 25.276987;
    float longitude = 55.296249;

    printf("Starting wifi!\n");
    InitWIFI();				// Initialize the port
    WIFI_Flush();

    printf("Doing Server File noooooow!\n");
    lua_doServerFile();

    WIFI_Flush();


    // printf("\n\nPosting Help\n");
    // getWeatherResponse = lua_postHelp();
    // printf(getWeatherResponse);

    // printf("\n\nPosting GPS\n");
    // lua_postGPS(lat, longitude); 
	// getWeatherResponse = WIFI_SaveFlush();
    // printf(getWeatherResponse);


    printf("\n\nGet Weather\n");
    getWeatherResponse = lua_getWeather();
    printf(getWeatherResponse);

    printf("\n\nGet population\n");
    getWeatherResponse = lua_getPopulation();
    printf(getWeatherResponse);

    printf("\n\nGet warnings\n");
    getWeatherResponse = lua_getWarnings();
    printf(getWeatherResponse);

    printf("\n\nPost Rating\n");
    getWeatherResponse = lua_postRating(1);
    printf(getWeatherResponse);

    printf("\n\nGet Rating\n");
    getWeatherResponse = lua_getRating();
    printf(getWeatherResponse);


    printf("End of Server Function Tests\n");

    while(1) {
        char data;
        if (WIFITestForReceivedData()) {
            data = (char) getcharWIFI();
            printf("%c", data);
        }
    }

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
    int i = 0;
    char cmd[50];
    char *flushbuf = ""; 
    char *responseHeaders = "";
    
    //Multiply by 1000000 to avoid passing a float 
    snprintf(cmd, sizeof(cmd), "post_gps(%d, %d)\r\n", (int)(latitude*1000000), (int)(longitude*1000000));

    printf(cmd);
    sendCommand(cmd);

    flushbuf = WIFI_SaveFlush(); // This saves the response from the WIFI chip (Important!)
    // responseHeaders = strtok( flushbuf , "[");
    return flushbuf;
}



char* lua_getWeather(void) {
    char cmd[] = "get_weather()\r\n";
    char *flushbuf = ""; 
    char *responseBody = "";
    int i = 0;
    
    sendCommand(cmd);
    flushbuf = WIFI_SaveFlush(); // This saves the response from the WIFI chip (Important!)

    /* walk through tokens when splitting string*/
    responseBody = strtok( flushbuf , "[");
    responseBody = strtok(NULL, "[");
    return responseBody;
}

char* lua_getPopulation() {
    char cmd[] = "get_population()\r\n";
    char *flushbuf = "";
    char *responseBody = ""; 
    
    sendCommand(cmd);
    flushbuf = WIFI_SaveFlush(); // This saves the response from the WIFI chip (Important!)

    /* walk through tokens when splitting string*/
    responseBody = strtok( flushbuf , "[");
    responseBody = strtok(NULL, "[");
    return responseBody;
}

/************************** WARNING ********************/
char* lua_getWarnings() {
    char cmd[] = "get_warnings()\r\n";
    char *flushbuf = ""; 
    char *responseBody = "";
    
    sendCommand(cmd);
    flushbuf = WIFI_SaveFlush(); // This saves the response from the WIFI chip (Important!)

    /* walk through tokens when splitting string*/
    responseBody = strtok( flushbuf , "[");
    responseBody = strtok(NULL, "[");
    return responseBody;
}


/************************** HELP ********************/
char* lua_postHelp() {
    int i = 0;
    char cmd[] = "post_help()\r\n";
    char *flushbuf = ""; 

    printf(cmd);
    sendCommand(cmd);

    flushbuf = WIFI_SaveFlush(); // This saves the response from the WIFI chip (Important!)

    return flushbuf;
}

/************************** RATING ********************/
char* lua_getRating() {
    char cmd[] = "get_rating()\r\n";
    char *flushbuf = ""; 
    char *responseBody = "";
    
    sendCommand(cmd);
    flushbuf = WIFI_SaveFlush(); // This saves the response from the WIFI chip (Important!)

    /* walk through tokens when splitting string*/
    responseBody = strtok( flushbuf , "[");
    responseBody = strtok(NULL, "[");
    return responseBody;
}

char* lua_postRating(int score){
    int i = 0;
    char cmd[50];
    char *flushbuf = ""; 
    
    snprintf(cmd, sizeof(cmd), "post_rating(%d)\r\n", score);

    printf(cmd);
    sendCommand(cmd);

    flushbuf = WIFI_SaveFlush(); // This saves the response from the WIFI chip (Important!)

    return flushbuf;
}





/************************** NOT IMPLEMENTED YET ********************/





/************ HELPER FUNCTIONS ******************/

void sendCommand(char *command)
{
	int i,j;
	for (i = 0; i < strlen(command); i++)
	{
        for(j = 0; j < 1000; j++) {;} //Give time to send signal
		putcharWIFI(command[i]);
	}
}

void WIFI_wait(int cycles)
{
    int i=0;
    while(!WIFITestForReceivedData() && i < cycles) {
        i++;
    }
}


//********** SAVE CHARACTER STREAM FROM WIFI CHIP *********//
char* WIFI_SaveFlush()
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

int putcharWIFI(int c)
{
	while((WIFI_LineStatusReg & 0x20) != 0x20) {   	// wait for Transmitter Holding Register bit (5) of line status register to be '1'
		//Do nothing. Wait for bit to'z change. Will be stuck in loop
	} 
	//printf("sending char: %x\n",(char)c);
	WIFI_TransmitterFifo = (char)c; 			// write character to Transmitter fifo register
	return c;											// return the character we printed

}


int getcharWIFI(void)
{
	while((WIFI_LineStatusReg & 0x1) != 0x01) {    	// wait for Data Ready bit (0) of line status register to be '1'		
	//Do nothing. Wait for bit to change. Will be stuck in loop
	} 
	//printf("receiving char... \n");
	return (int) WIFI_ReceiverFifo;					// read new character from ReceiverFiFo register, return new character
 
}


// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read from the FIFO
int WIFITestForReceivedData(void)
{
	if ((WIFI_LineStatusReg & 0x1) == 0x1) {
		return TRUE;
	} else {
		return FALSE;
	}
 // if WIFI_LineStatusReg bit 0 is set to 1
 //return TRUE, otherwise return FALSE
}


void WIFI_Flush()
{
    int flushedData;
    int i;

    // Flush UART receive buffer
    // while((WIFI_LineStatusReg & 0x01) == 0x01) {
	//     flushedData = WIFI_ReceiverFifo;
	// }

    for(i = 0; i < 10000000 /* Timeout */; i++) {
        char data;
        if (WIFITestForReceivedData()) {
            data = (char) getcharWIFI();
            printf("%c", data);
        }
    }
}



