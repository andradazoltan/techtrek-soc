/*
 * main.cpp
 *
 *  Created on: Feb 4, 2020
 *      Author: Andrada Zoltan
 */

#include "PeopleTracker.h"
#include <iostream>
#include <thread>
#include <stdio.h>

using namespace Spinnaker;
using std::cout;
using std::thread;
using std::vector;

int main(void) {
    int err = 0;

    FILE *fp = fopen("/tmp/pipe", "r+");

    PeopleTracker* tracker = new PeopleTracker();

    err = tracker->InitTracker();
    if (err) {
        cout << "Error InitTracker!\n";
        return -1;
    }

    // Create acquisition thread.
    thread acqThread(&PeopleTracker::StartTracking, tracker);

    int peopleCount = 0;
    while (1) {
        int tmp = tracker->GetPeopleCount();
        if (tmp != peopleCount) {
            peopleCount = tmp;
            cout << peopleCount << "\n";
            fprintf(fp, "%d\n", peopleCount);
            fflush(fp);
        }
        usleep(100 * 1000);
    }

    delete tracker;
    fclose(fp);
    return 0;
}
