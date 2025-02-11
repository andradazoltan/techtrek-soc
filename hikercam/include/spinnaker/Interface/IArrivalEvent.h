//=============================================================================
// Copyright (c) 2001-2018 FLIR Systems, Inc. All Rights Reserved.
//
// This software is the confidential and proprietary information of FLIR
// Integrated Imaging Solutions, Inc. ("Confidential Information"). You
// shall not disclose such Confidential Information and shall use it only in
// accordance with the terms of the license agreement you entered into
// with FLIR Integrated Imaging Solutions, Inc. (FLIR).
//
// FLIR MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE, OR NON-INFRINGEMENT. FLIR SHALL NOT BE LIABLE FOR ANY DAMAGES
// SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
// THIS SOFTWARE OR ITS DERIVATIVES.
//=============================================================================

#ifndef FLIR_SPINNAKER_IARRIVALEVENT_H
#define FLIR_SPINNAKER_IARRIVALEVENT_H

#include "Event.h"
#include "SpinnakerPlatform.h"

namespace Spinnaker {
class IArrivalEvent : public virtual Event {
public:
  virtual ~IArrivalEvent(){};
  virtual void OnDeviceArrival(uint64_t serialNumber) = 0;

protected:
  IArrivalEvent(){};
  IArrivalEvent(const IArrivalEvent &){};
  IArrivalEvent &operator=(const IArrivalEvent &);
};
} // namespace Spinnaker

#endif /* FLIR_SPINNAKER_IARRIVALEVENT_H */