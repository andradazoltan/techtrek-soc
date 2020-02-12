
#ifndef __gps_h
#define __gps_h

#define GPS_ReceiverFifo (*(volatile unsigned char *)(0xFF210210))
#define GPS_TransmitterFifo (*(volatile unsigned char *)(0xFF210210))
#define GPS_InterruptEnableReg (*(volatile unsigned char *)(0xFF210212))
#define GPS_InterruptIdentificationReg (*(volatile unsigned char *)(0xFF210214))
#define GPS_FifoControlReg (*(volatile unsigned char *)(0xFF210214))
#define GPS_LineControlReg (*(volatile unsigned char *)(0xFF210216))
#define GPS_ModemControlReg (*(volatile unsigned char *)(0xFF210218))
#define GPS_LineStatusReg (*(volatile unsigned char *)(0xFF21021A))
#define GPS_ModemStatusReg (*(volatile unsigned char *)(0xFF21021C))
#define GPS_ScratchReg (*(volatile unsigned char *)(0xFF21021E))
#define GPS_DivisorLatchLSB (*(volatile unsigned char *)(0xFF210210))
#define GPS_DivisorLatchMSB (*(volatile unsigned char *)(0xFF210212))

#define HEX0_1 (volatile unsigned int *)(0xFF200030)
#define HEX2_3 (volatile unsigned int *)(0xFF200040)
#define HEX4_5 (volatile unsigned int *)(0xFF200050)
#define SW (volatile unsigned int *)(0xFF200000)
#define LEDS (volatile unsigned int *)(0xFF200020)

#define TRUE 1
#define FALSE 0

#define SENTENCE_TYPE_GGA 1
#define SENTENCE_TYPE_INVALID -1

/*
 * This struct holds the data extracted from a GGA sentence during parsing.
 */
struct gga {
  float gga_utc; // in seconds
  float gga_lat; // positive is north
  float gga_lon; // positive is east
  int gga_fix;
  int gga_satellites_used;
  float gga_hdop;
  float gga_msl_alt;    // in meters
  float gga_separation; // in meters
};

void gps_uart_init(void);
int gps_uart_putchar(int c);
int gps_uart_getchar(void);
int gps_uart_data_available(void);
void gps_uart_flush(void);

/*
 * Given a char array containing a valid GGA sentence, parses the sentence and
 * stores its contents in the struct gga *sentence.
 */
int parse_gga(char *raw_sentence, struct gga *sentence);

// The display functions here are not strictly GPS-related (they were useful in
// the demo for the exercise), and may be better moved to their own library or
// deleted.

// Display the six most significant digits of the float num on the 7-segment
// displays (without a decimal point), and turn on LED10 if num is negative
void displayf(float num);

// Display the six most significant digits of int num on the 7-segment displays
// and turn on LED10 if num is negative
void displayd(int num);

// Write the six most significant digits of the hexadecimal value digits to the
// 7-segment displays
void display(long long digits);

// These functions are not really used - they are useful for the data-logging
// functionality, but nothing outside of that.
int swap_endian(char *s);
char *float_to_lat(int x);
char *float_to_lon(int x);

#endif
