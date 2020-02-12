/*
 * C File for driving UART GPS
 *
 * This currently does not support data-logging functionality. It only
 * parses GGA strings.
 */

#include "gps.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_sentence_type(char *sentence_buf) {
  if (strncmp(sentence_buf, "GPGGA", 5) == 0) {
    return SENTENCE_TYPE_GGA;
  }

  return SENTENCE_TYPE_INVALID;
}

int parse_gga(char *sentence_buf, struct gga *sentence) {
  char *field_start = strtok(sentence_buf, ",");
  char *next_field_start;

  // Time
  field_start = strtok(NULL, ",");
  sentence->gga_utc =
      (field_start[0] - '0') * 36000 + (field_start[1] - '0') * 3600 +
      (field_start[2] - '0') * 600 + (field_start[3] - '0') * 60 +
      strtof(field_start + 4, &next_field_start);

  // Latitude
  field_start = strtok(NULL, ",");
  sentence->gga_lat = (field_start[0] - '0') * 10 + (field_start[1] - '0') +
                      strtof(field_start + 2, &next_field_start) / 60;
  field_start = strtok(NULL, ",");
  if (field_start[0] == 'S') {
    sentence->gga_lat = -sentence->gga_lat;
  } else if (field_start[0] != 'N') {
    return SENTENCE_TYPE_INVALID;
  }

  // Longitude
  field_start = strtok(NULL, ",");
  sentence->gga_lon = (field_start[0] - '0') * 100 +
                      (field_start[1] - '0') * 10 + (field_start[2] - '0') +
                      strtof(field_start + 3, &next_field_start) / 60;
  field_start = strtok(NULL, ",");
  if (field_start[0] == 'W') {
    sentence->gga_lat = -sentence->gga_lat;
  } else if (field_start[0] != 'E') {
    return SENTENCE_TYPE_INVALID;
  }

  // Position fix indicator
  field_start = strtok(NULL, ",");
  sentence->gga_fix = strtod(field_start, &next_field_start);

  // Number of satellites used
  field_start = strtok(NULL, ",");
  sentence->gga_satellites_used = strtod(field_start, &next_field_start);

  // Horizontal dilution of precision
  field_start = strtok(NULL, ",");
  sentence->gga_hdop = strtof(field_start, &next_field_start);

  // Mean-sea-level altitude
  field_start = strtok(NULL, ",");
  sentence->gga_msl_alt = strtof(field_start, &next_field_start);
  strtok(NULL, ","); // get rid of the unit (assume it's meters)

  // Geoidal separation
  field_start = strtok(NULL, ",");
  sentence->gga_separation = strtof(field_start, &next_field_start);

  return 0;
}

// Read the next sentence from the GPS and store it in the provided buffer, and
// return the type of the sentence. Returns SENTENCE_TYPE_INVALID if the
// sentence is longer than 128 characters.
int read_sentence(char *raw_sentence) {
  int length = 0;

  while (*raw_sentence != '$') {
    *raw_sentence = (char)gps_uart_getchar();
  }

  while (raw_sentence[length] != '\r' && length < 128) {
    length++;
    raw_sentence[length] = (char)gps_uart_getchar();
  }

  if (raw_sentence[length] != '\r') {
    raw_sentence[length] = '\0';
    return SENTENCE_TYPE_INVALID;
  }

  raw_sentence[length] = '\0';
  return get_sentence_type(raw_sentence);
}

// Read and parse the next GGA sentence received from the GPS, discarding
// sentences of other types
void read_gga(struct gga *sentence) {
  char raw_sentence[128];

  while (read_sentence(raw_sentence) != SENTENCE_TYPE_GGA) {
  }

  parse_gga(raw_sentence, sentence);
}

void display(long long digits) {
  *HEX0_1 = digits & 0xFFFF;
  *HEX2_3 = (digits >> 8) & 0xFFFF;
  *HEX4_5 = (digits >> 16) & 0xFFFF;
}

void displayf(float num) {
  char buf[100];
  long long digits = 0;

  sprintf(buf, "%f", fabs(num));

  if (num < 0) {
    *LEDS |= 0x1 << 9;
  } else {
    *LEDS = 0;
  }

  int offset = 0;
  for (int i = 0; i < 6; i++) {
    if (buf[i] == '.') {
      offset = 1;
    }
    if (buf[i] == '\0') {
      break;
    }

    digits <<= 4;
    digits += buf[i + offset] - '0';
  }

  display(digits);
}

void displayd(int num) {
  char buf[100];
  long long digits = 0;

  sprintf(buf, "%d", abs(num));

  if (num < 0) {
    *LEDS = 0x1 << 9;
  } else {
    *LEDS = 0;
  }

  for (int i = 0; i < 6; i++) {
    if (buf[i] == '\0') {
      break;
    }

    digits <<= 4;
    digits += buf[i] - '0';
  }

  display(digits);
}

void gps_uart_init(void) {
  // Program serial port to communicate with GPS

  GPS_LineControlReg |= 0x80; // set bit 7 of Line Control Register to 1, to
                              // gain access to the baud rate registers
  GPS_DivisorLatchLSB = 0x45; // set Divisor latch (LSB and MSB) with correct
                              // value for required baud rate
  GPS_DivisorLatchMSB = 0x01;
  GPS_LineControlReg = 0x03; // program other bits in that reg for 8 bit data, 1
                             // stop bit, no parity etc
  GPS_FifoControlReg =
      GPS_FifoControlReg |
      0x06; // Reset the Fifo’s in the FiFo Control Reg by setting bits 1 & 2
  GPS_FifoControlReg = GPS_FifoControlReg ^
                       0x06; // Now Clear all bits in the FiFo control registers
}

int gps_uart_putchar(int c) {
  while ((GPS_LineStatusReg & 0x20) !=
         0x20) { // wait for Transmitter Holding Register bit (5) of line status
                 // register to be '1'
                 // Do nothing. Wait for bit to'z change. Will be stuck in loop
  }
  printf("sending char: %x\n", (char)c);
  GPS_TransmitterFifo = (char)c; // write character to Transmitter fifo register
  return c;                      // return the character we printed
}

int gps_uart_getchar(void) {
  while (
      (GPS_LineStatusReg & 0x1) !=
      0x01) { // wait for Data Ready bit (0) of line status register to be '1'
    // Do nothing. Wait for bit to change. Will be stuck in loop
  }
  // printf("receiving char... \n");
  return (int)GPS_ReceiverFifo; // read new character from ReceiverFiFo
                                // register, return new character
}

// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read from the FIFO
int gps_uart_data_available(void) {
  if ((GPS_LineStatusReg & 0x1) == 0x1) {
    return TRUE;
  } else {
    return FALSE;
  }
  // if RS232_LineStatusReg bit 0 is set to 1
  // return TRUE, otherwise return FALSE
}

//
// Remove/flush the UART receiver buffer by removing any unread characters
//
void gps_uart_flush(void) {
  int flushedData;
  while ((GPS_LineStatusReg & 0x01) == 0x01) {
    flushedData = GPS_ReceiverFifo;
  }

  (void)flushedData; // Get rid of unused variable warning
  return;
  // while bit 0 of Line Status Register == ‘1’
  // read unwanted char out of fifo receiver buffer
  // return; // no more characters so return
}

int swap_endian(char *s) {
  register int val;
  val = strtoul(s, NULL, 16); // convert to 4 byte int form in base 16
  val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
  val = (val << 16) | ((val >> 16) & 0xFFFF);
  return val;
}

// thesetwo functions takea 4 byte IEEE-754 format float
// (passed as a 4 byte int representing latitude and longitude values)
// in big endian formatand converts it to an ASCII decimal string
// which it returns with decimal point in the string.
char *float_to_lat(int x) // output format is xx.yyyy
{
  static char buff[100];
  float *ptr = (float *)(&x); // cast int to float
  float f = *ptr;             // get the float
  sprintf(buff, "%2.4f", f);  // write in string to an arrayreturnbuff ;
  return buff;
}

char *float_to_lon(int x) // output format is (-)xxx.yyyy
{
  static char buff[100];
  float *ptr = (float *)(&x);
  float f = *ptr;
  sprintf(buff, "%3.4f", f);
  return buff;
}
