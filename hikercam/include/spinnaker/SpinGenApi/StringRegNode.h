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

#ifndef SPINNAKER_GENAPI_STRINGREGNODE_H
#define SPINNAKER_GENAPI_STRINGREGNODE_H

#include "Base.h"
#include "GCString.h"
#include "INode.h"
#include "ISelector.h"
#include "RegisterNode.h"
#include "SpinnakerPlatform.h"
#include "StringNode.h"
#include "Types.h"
#include "ValueNode.h"

//#include <assert.h>

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4250) // C4250 - 'class1' : inherits 'class2::member'
                                // via dominance
#pragma warning(disable : 4251) // XXX needs to have dll-interface to be used by
                                // clients of class YYY
#pragma warning(disable : 4275) // non dll-interface structXXX used as base
#endif

namespace Spinnaker {
namespace GenApi {
/**
 *  @defgroup SpinnakerGenApiClasses Spinnaker GenApi Classes
 */
/*@{*/

/**
 *  @defgroup StringRegNode_h StringRegNode Class
 */
/*@{*/

/**
 * @brief Interface for string properties
 */
class SPINNAKER_API StringRegNode : virtual public StringNode,
                                    virtual public RegisterNode {
public:
  struct NodeImpl;
  StringRegNode();

  StringRegNode(std::shared_ptr<Node::NodeImpl> pString);

  virtual ~StringRegNode();

  /**
   * overload SetReference for Value
   */
  virtual void SetReference(INode *pBase);
};

/*@}*/
/*@}*/
} // namespace GenApi
} // namespace Spinnaker

#ifdef _WIN32
#pragma warning(pop)
#endif

#endif // SPINNAKER_GENAPI_STRINGREGNODE_H