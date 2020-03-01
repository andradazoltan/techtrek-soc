/*
 * gps.c
 *
 * C File for driving UART GPS
 *
 * This currently does not support data-logging functionality. It only
 * parses GGA strings.
 *
 * Created on: Feb 23, 2020
 * Author: Jacob Grossbard
 */

#include "gps.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Given a char array containing a valid GGA sentence, parses the sentence and
 * stores its contents in the struct gga *sentence.
 */
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
    sentence->gga_lat = -sentence->gga_lon;
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

/*
 * Read the next sentence of the specified type from the GPS and store it in the
 * provided buffer Returns -1 on invalid input
 */
int read_sentence(char *raw_sentence, int sentence_type) {
  int length = 0;
  char *start_sequence;

  switch (sentence_type) {
  case SENTENCE_TYPE_GGA:
    start_sequence = "$GPGGA";
    break;
  default:
    return -1;
  }

  gps_uart_flush();

  char checksum;
  char received_checksum;
  do {

    while (*raw_sentence != '$') {
      *raw_sentence = (char)gps_uart_getchar();
    }

    length = 1;
    checksum = 0;

    while (length < 125 && raw_sentence[length - 1] != '*') {
      raw_sentence[length] = (char)gps_uart_getchar();
      if (raw_sentence[length] != '*') {
        checksum ^= raw_sentence[length];
      }
      length++;
    }

    raw_sentence[length] = (char)gps_uart_getchar();
    raw_sentence[length + 1] = (char)gps_uart_getchar();
    raw_sentence[length + 2] = '\0';

    sscanf(raw_sentence + length, "%hhX", &received_checksum);

  } while (checksum != received_checksum ||
           strncmp(raw_sentence, start_sequence, 6) != 0);

  return 0;
}

/*
 * Read and parse the next GGA sentence received from the GPS, discarding
 * sentences of other types
 */
void read_gga(struct gga *sentence) {
  char raw_sentence[128];

  read_sentence(raw_sentence, SENTENCE_TYPE_GGA);
  printf("%s\n", raw_sentence);
  parse_gga(raw_sentence, sentence);
}

/*
 * Determine whether a given struct represents a valid fix or not. Returns 1 if
 * the fix is valid and 0 otherwise
 */
int gga_fix_is_valid(struct gga sentence) {
  if (sentence.gga_fix > 0 && sentence.gga_fix < 6) {
    return 1;
  }

  return 0;
}

/*
 * Initialize the UART
 */
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

/*
 * Write a single character the the UART
 */
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

/*
 * Read and return a single character from the UART. If none are available,
 * block until one becomes available
 */
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

/*
 * Poll the UART to determine if any character is available in the buffer
 */
int gps_uart_data_available(void) {
  if ((GPS_LineStatusReg & 0x1) == 0x1) {
    return 1;
  } else {
    return 0;
  }
}

/*
 * Flush the UART receiver buffer by removing any unread characters
 */
void gps_uart_flush(void) {
  int flushedData;
  while ((GPS_LineStatusReg & 0x01) == 0x01) {
    flushedData = GPS_ReceiverFifo;
  }

  (void)flushedData; // Get rid of unused variable warning
  return;
}
