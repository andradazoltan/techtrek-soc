/*
 *  HikerCam.h
 *
 *  Created on: Feb 4, 2020
 *  Author: Andrada Zoltan
 */

#ifndef HIKERCAM_H_
#define HIKERCAM_H_

#include "SpinGenApi/SpinnakerGenApi.h"
#include "Spinnaker.h"
#include <atomic>
#include <mutex>
#include <vector>

class HikerCam {
public:
  HikerCam();
  ~HikerCam();

  int InitCamera(void);
  int StartAcquisition(void);
  void EndAcquisition(void);
  void GetBoundingBoxData(std::vector<Spinnaker::InferenceBoundingBox> &buf);

private:
  Spinnaker::SystemPtr mSystem;
  Spinnaker::CameraPtr mCamera;
  std::atomic<bool> endAcquistionSignal;

  std::mutex *bufferMutex;
  std::vector<Spinnaker::InferenceBoundingBox> *boundingBoxBuffer;

  int EnableInference(Spinnaker::GenApi::INodeMap &nodeMap);
};

#endif /* HIKERCAM_H_ */
