/*
 *  PeopleTracker.h
 *
 *  Created on: Feb 6, 2020
 *  Author: Andrada Zoltan
 */

#ifndef PEOPLE_TRACKER_H_
#define PEOPLE_TRACKER_H_

#include "Centroid.h"
#include "HikerCam.h"
#include "Spinnaker.h"
#include <atomic>
#include <vector>

class PeopleTracker {
public:
  PeopleTracker();
  ~PeopleTracker();

  int InitTracker();
  void StartTracking();
  void StopTracking();
  int GetPeopleCount();

private:
  std::atomic<int> peopleCount;
  HikerCam *mCam;
  std::atomic<bool> endTrackingSignal;
  std::vector<Centroid *> *centroidTracker;
};

#endif /* PEOPLE_TRACKER_H_ */
