//=============================================================================
// Copyright © 2017 FLIR Integrated Imaging Solutions, Inc. All Rights Reserved.
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

#ifndef SPINNAKER_GENAPI_H
#define SPINNAKER_GENAPI_H

#include "BooleanNode.h"
#include "CategoryNode.h"
#include "ChunkAdapter.h"
#include "ChunkAdapterDcam.h"
#include "ChunkAdapterGEV.h"
#include "ChunkAdapterGeneric.h"
#include "ChunkAdapterU3V.h"
#include "ChunkPort.h"
#include "CommandNode.h"
#include "EnumClasses.h"
#include "EnumEntryNode.h"
#include "EnumNode.h"
#include "EnumNodeT.h"
#include "EventAdapter.h"
#include "EventAdapter1394.h"
#include "EventAdapterGEV.h"
#include "EventAdapterGeneric.h"
#include "EventAdapterU3V.h"
#include "EventPort.h"
#include "FloatNode.h"
#include "FloatRegNode.h"
#include "GCString.h"
#include "GCStringVector.h"
#include "GCTypes.h"
#include "IntRegNode.h"
#include "IntegerNode.h"
#include "Node.h"
#include "NodeCallback.h"
#include "NodeMap.h"
#include "Persistence.h"
#include "Pointer.h"
#include "PortNode.h"
#include "PortWriteList.h"
#include "RegisterNode.h"
#include "StringNode.h"
#include "StringRegNode.h"
#include "Synch.h"
#include "Types.h"
#include "ValueNode.h"

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4244) // conversion from 'std::streamsize' to 'size_t'
#include "Filestream.h"
#pragma warning(pop)
#else
#include "Filestream.h"
#endif

#include "IDeviceInfo.h"
#include "IPort.h"

#ifdef _WIN32
// GenApi Warnings
#pragma warning(disable : 4068) // unknown pragma; refers to BullsEyeCoverage
#pragma warning(disable : 4251) // XXX needs to have dll-interface to be used by
                                // clients of class YYY
#pragma warning(disable : 4702) // unreachable code in <list>,...
#pragma warning(disable : 4275) // non dll-interface structXXX used as base
#pragma warning(disable : 4312) // 'type cast' : conversion from 'uintptr_t' to
                                // 'void *' of greater size
#endif

#endif // SPINNAKER_GENAPI_H
