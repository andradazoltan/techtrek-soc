/*
 *  main.cpp
 *
 *  Created on: Feb 8, 2020
 *  Author: Andrada Zoltan
 */

#include "WiFi.h"
#include "TouchScreen.h"
#include "Graphics.h"

int main (void) {
    // Initialize all of the hardware
    InitWIFI();
    InitTouch();
}
