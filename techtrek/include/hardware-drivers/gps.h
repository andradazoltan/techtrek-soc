/*
 *  gps.h
 *
 *  Created on: Feb 24, 2020
 *  Author: Jacob Grossbard
 */

#ifndef GPS_H_
#define GPS_H_

#include "RegisterDefs.h"

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

/*
 * Block until the GPS sends a GGA sentence, then parse that sentence and store
 * the result in the struct pointed to by sentence
 */
void read_gga(struct gga *sentence);

/*
 * Given a char array containing a valid GGA sentence, parses the sentence and
 * stores its contents in the struct gga *sentence.
 */
int parse_gga(char *raw_sentence, struct gga *sentence);

/*
 * Returns true if the given struct gga represents a valid fix and false
 * otherwise
 */
int gga_fix_is_valid(struct gga sentence);

/*
 * These functions handle serial IO with the GPS device, and usually should not
 * be used outside of the library (though they are made available if needed)
 */
void gps_uart_init(void);
int gps_uart_putchar(int c);
int gps_uart_getchar(void);
int gps_uart_data_available(void);
void gps_uart_flush(void);

#endif /* GPS_H_ */
