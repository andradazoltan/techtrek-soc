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

/* Auto-generated file. Do not modify. */

#ifndef FLIR_SPINNAKER_TRANSPORTLAYERSYSTEM_H
#define FLIR_SPINNAKER_TRANSPORTLAYERSYSTEM_H

#include "SpinGenApi/SpinnakerGenApi.h"
#include "SpinnakerPlatform.h"
#include "TransportLayerDefs.h"
#include <string>

namespace Spinnaker {
/**
 * @defgroup SpinnakerQuickSpinClasses Spinnaker QuickSpin Classes
 */
/*@{*/

/**
 * @defgroup TransportLayerSystem_h TransportLayerSystem Class
 */
/*@{*/

/**
 *@brief Part of the QuickSpin API to provide access to camera information
 *without having to first initialize the camera.
 */

class SPINNAKER_API TransportLayerSystem {
public:
  TransportLayerSystem(GenApi::INodeMap *nodeMapTLDevice);
  ~TransportLayerSystem();

protected:
  TransportLayerSystem();

private:
  TransportLayerSystem(const TransportLayerSystem &);
  TransportLayerSystem &operator=(const TransportLayerSystem &);
  TransportLayerSystem &operator=(GenApi::INodeMap &);

public:
  /**
   * Description: Enables or disables enumeration of GEV Interfaces.
   * Visibility: Expert
   */
  GenApi::IBoolean &EnumerateGEVInterfaces;

  /**
   * Description: Automatically forces any cameras on the system to an IP
   * Address on the same subnet as the interfaces they are connected to.
   * Visibility: Expert
   */
  GenApi::ICommand &AutoForceIP;

protected:
  friend class System;
  friend class ISystem;
  friend class SystemPtrInternal;
};
/*@}*/

/*@}*/

} // namespace Spinnaker
#endif // FLIR_SPINNAKER_TRANSPORTLAYERSYSTEM_H