﻿/*
 *  main.c
 *
 *  Main TechTrek Program File
 *
 *  Created on: Feb 8, 2020
 *  Author: Andrada Zoltan
 */

#include "Graphics.h"
#include "RegisterDefs.h"
#include "Screens.h"
#include "TouchScreen.h"
#include "gps.h"
#include "images.h"
#include "wifi.h"
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Private function prototypes
void get_people_count(int fd);

// Global Variables
void *virtual_base = NULL;
BMP techtrek;
BMP parkmap;

int main(void) {
  int fd, fd_fifo;

  // Open memory as if it were a device for read and write access
  if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
    printf("ERROR: could not open \"/dev/mem\"...\n");
    return 1;
  }

  // Map 2Mbyte of memory starting at 0xFF200000 to user space
  virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED,
                      fd, HW_REGS_BASE);
  if (virtual_base == MAP_FAILED) {
    printf("ERROR: mmap() failed...\n");
    close(fd);
    return 1;
  }

  // Initialize a named pipe between this program and the hikercam program
  mkfifo("/tmp/pipe",
         S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  if ((fd_fifo = open("/tmp/pipe", O_RDONLY | O_NONBLOCK)) == -1) {
    printf("ERROR: could not open \"/tmp/pipe\"...\n");
    return 1;
  }

  // Initialize all of the hardware
  wifi_uart_init();
  wifi_uart_flush();
  touch_uart_init();
  gps_uart_init();

  // Initialize the WiFi connection to the server
  lua_doServerFile();
  wifi_uart_flush();

  // Initialize the images
  initImage("/home/ubuntu/Pictures/techtrek.bmp", &techtrek);
  initImage("/home/ubuntu/Pictures/PacificSpiritParkMap.bmp", &parkmap);

  // Initialize the main GUI screen
  initColours();
  drawMainScreen();

  // Get GPS coordinates
  struct gga sentence;
  do {
    read_gga(&sentence);
  } while (!gga_fix_is_valid(sentence));

  printf("Location: %f %f\n", sentence.gga_lat, sentence.gga_lon);
  lua_postGPS(sentence.gga_lat, sentence.gga_lon);

  // Kick off threads
  pthread_t touch_screen_thread;
  pthread_t people_count_thread;
  pthread_create(&touch_screen_thread, NULL, (void *)&ReadTouchScreen, NULL);
  pthread_create(&people_count_thread, NULL, (void *)&get_people_count,
                 (void *)fd_fifo);

  pthread_join(touch_screen_thread, NULL);
  pthread_join(people_count_thread, NULL);

  // When finished, unmap the virtual space and close the memory "device"
  if (munmap(virtual_base, HW_REGS_SPAN) != 0) {
    printf("ERROR: munmap() failed...\n");
    freeImage(&techtrek);
    freeImage(&parkmap);
    close(fd);
    close(fd_fifo);
    return 1;
  }

  // free images
  freeImage(&techtrek);
  freeImage(&parkmap);

  close(fd);
  close(fd_fifo);
  return 0;
}

/*
 * Read the people count from the shared pipe and update the global
 * people count every second.
 *
 * @param fd: file pointer to pipe file
 */
void get_people_count(int fd) {
  while (1) {
    // Assume count will never get larger than 8 digits long
    char buf[8] = {0};

    // Note that read will block until the FIFO is not empty
    if (read(fd, buf, 8) > 0) {
      people_count = atoi(buf);

      // If on the info screen, update the displayed people count
      if (currScreen == INFO_SCREEN) {
        FillRect(335, 190, 755, 300, DARK_SLATE_BLUE);
        OutGraphicsCharFont3(345, 200, GREEN, DARK_SLATE_BLUE,
                             "TRAIL POPULATION:", 0);
        char people[20];
        sprintf(people, "%d people", people_count);
        OutGraphicsCharFont4(450, 240, BLACK, DARK_SLATE_BLUE, people, 0);
      }
      lua_postPopulation(people_count);
    }

    usleep(1000 * 1000);
  }
}
