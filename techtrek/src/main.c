/*
 *  main.c
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
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>

void *virtual_base = NULL;

// File descriptor
int fd;

int main (void) {
    int fd;

    // Open memory as if it were a device for read and write access
    if ((fd = open( "/dev/mem", ( O_RDWR | O_SYNC ))) == -1) {
        printf("ERROR: could not open \"/dev/mem\"...\n");
        return(1);
    }

    // Map 2Mbyte of memory starting at 0xFF200000 to user space
    virtual_base = mmap(NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ),MAP_SHARED, fd, HW_REGS_BASE);
    if (virtual_base == MAP_FAILED) {
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return(1);
    }

    // Initialize all of the hardware
    InitWIFI();
    InitTouch();

    // Initialize the main screen
    initColours();
    drawMainScreen();

    // Kick off threads
    pthread_t touch_screen_thread;
    pthread_create(&touch_screen_thread, NULL, (void *)&ReadTouchScreen, NULL);

    pthread_join(touch_screen_thread, NULL);

    // When finished, unmap the virtual space and close the memory "device"
    if (munmap( virtual_base, HW_REGS_SPAN ) != 0) {
        printf("ERROR: munmap() failed...\n");
        close(fd);
        return(1);
    }
    close(fd);
    return 0;
}
