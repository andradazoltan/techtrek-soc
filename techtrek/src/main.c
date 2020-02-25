﻿/*
 *  main.c
 *
 *  Main TechTrek Program File
 *
 *  Created on: Feb 8, 2020
 *  Author: Andrada Zoltan
 */

#include "WiFi.h"
#include "TouchScreen.h"
#include "Graphics.h"
#include "Screens.h"
#include "RegisterDefs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

// Private function prototypes
void getPeopleCount(int fd);

// Global Variables
void *virtual_base = NULL;
int people_count = 0;

int main (void) {
    int fd, fd_fifo;

    // Open memory as if it were a device for read and write access
    if ((fd = open( "/dev/mem", ( O_RDWR | O_SYNC ))) == -1) {
        printf("ERROR: could not open \"/dev/mem\"...\n");
        return 1;
    }

    // Map 2Mbyte of memory starting at 0xFF200000 to user space
    virtual_base = mmap(NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ),MAP_SHARED, fd, HW_REGS_BASE);
    if (virtual_base == MAP_FAILED) {
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return 1;
    }

    // Initialize a named pipe between this program and the hikercam program
   /* mkfifo("/tmp/pipe", S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if ((fd_fifo = open("/tmp/pipe", O_RDONLY)) == -1 ) {
        printf("ERROR: could not open \"/tmp/pipe\"...\n");
        return 1;
    }*/

    // Initialize all of the hardware
    InitWIFI();
    WIFI_Flush();
    InitTouch();

    // Initialize the WiFi connection to the server
    lua_doServerFile();
    WIFI_Flush();

    // Initialize the main GUI screen
    initColours();
    drawMainScreen();

    // Kick off threads
    pthread_t touch_screen_thread;
    pthread_t people_count_thread;
    pthread_create(&touch_screen_thread, NULL, (void *)&ReadTouchScreen, NULL);
    //pthread_create(&people_count_thread, NULL, (void *)&getPeopleCount, fd_fifo);

    pthread_join(touch_screen_thread, NULL);

    // When finished, unmap the virtual space and close the memory "device"
    if (munmap( virtual_base, HW_REGS_SPAN ) != 0) {
        printf("ERROR: munmap() failed...\n");
        close(fd);
        close(fd_fifo);
        return 1;
    }

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
void getPeopleCount(int fd) {
    while (1) {
        // Assume count will never get larger than 8 digits long
        char buf[8] = {0};

        // Note that read will block until the FIFO is not empty
        if (read(fd, buf, 8) > 0) {
            printf("%s\n", buf);
            people_count = atoi(buf);
        }

        usleep(1000 * 1000);
    }
}
