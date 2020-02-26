/*
 *  main.c
 *
 *  Main TechTrek Program File
 *
 *  Created on: Feb 8, 2020
 *  Author: Andrada Zoltan
 */

#include "WiFi.h"
#include "gps.h"
#include "TouchScreen.h"
#include "Graphics.h"
#include "Screens.h"
#include "RegisterDefs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

// Private function prototypes
void getPeopleCount(int fd);
void getWarnings(void);

// Global Variables
void *virtual_base = NULL;

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
    mkfifo("/tmp/pipe", S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if ((fd_fifo = open("/tmp/pipe", O_RDONLY | O_NONBLOCK)) == -1) {
        printf("ERROR: could not open \"/tmp/pipe\"...\n");
        return 1;
    }

    // Initialize all of the hardware
    InitWIFI();
    WIFI_Flush();
    InitTouch();
    gps_uart_init();

    // Initialize the WiFi connection to the server
    lua_doServerFile();
    WIFI_Flush();

    // Initialize the images
    BMP techtrek;
    initImage("techtrek.bmp", &techtrek);

    // Initialize the main GUI screen
    initColours();
    drawMainScreen();

    // Kick off threads
    pthread_t touch_screen_thread;
    pthread_t people_count_thread;
    pthread_t get_warnings_thread;
    pthread_create(&touch_screen_thread, NULL, (void *)&ReadTouchScreen, NULL);
    pthread_create(&people_count_thread, NULL, (void *)&getPeopleCount, fd_fifo);
    pthread_create(&get_warnings_thread, NULL, (void *)&getWarnings, NULL);

    struct gga sentence;
    read_gga(&sentence);
    printf("Location: %f %f\n", sentence.gga_lat, sentence.gga_lon);
    lua_postGPS(sentence.gga_lat, sentence.gga_lon);

    pthread_join(touch_screen_thread, NULL);

    // When finished, unmap the virtual space and close the memory "device"
    if (munmap( virtual_base, HW_REGS_SPAN ) != 0) {
        printf("ERROR: munmap() failed...\n");
        close(fd);
        close(fd_fifo);
        return 1;
    }

    // free images
    freeImage(&techtrek);
    
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
    // Post the initial population count, this should be 0
    lua_postPopulation(people_count);

    while (1) {
        // Assume count will never get larger than 8 digits long
        char buf[8] = {0};

        // Note that read will block until the FIFO is not empty
        if (read(fd, buf, 8) > 0) {
            people_count = atoi(buf);
            lua_postPopulation(people_count);
        }

        usleep(1000 * 1000);
    }
}

/*
 * Get the latest warning the from server, and update the list of
 * warnings to hold the latest 5 warnings.
 *
 * If on the main screen currently, print an alert with the total
 * number of unviewed warnings.
 */
void getWarnings(void) {
    int newWarnings = 0;
    char ids[5][30];

    while (1) {
        // Get the latest response
        char response[100];
        lua_getWarnings(response);

        // Index of previous warning
        int prevWarning = (currWarning == 0) ? 4 : currWarning - 1;

        // Pull out the ID of the warning and check that we haven't seen this one yet
        char *tok = strtok(response, "-");
        if (tok != NULL && (strcmp(ids[prevWarning], tok) == 0))
        {
            // Copy the id
            strcpy(ids[currWarning], tok);

            // Copy the actual message
            tok = strtok(NULL, "");
            strcpy(warnings[currWarning], tok);

            // Post an alert if on the main screen
            if (currScreen == MAIN_SCREEN) {
                newWarnings++;

                char alert[30];
                sprintf(alert, "There are %d new warnings!", newWarnings);

                // Alert on main screen
                FillRect(70, 250, 400, 300, RED);
                OutGraphicsCharFont3(80, 260, WHITE, RED, alert, 0);
            }
            else {
                newWarnings = 0;
            }
        }

        // Update the currWarning index
        if (currWarning == 4)
            currWarning = 0;
        else
            currWarning++;

        // Sleep for 5 seconds before trying again
        usleep(5000 * 1000);
    }
}
