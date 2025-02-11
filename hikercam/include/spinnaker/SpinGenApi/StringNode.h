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

#ifndef SPINNAKER_GENAPI_STRINGNODE_H
#define SPINNAKER_GENAPI_STRINGNODE_H

#include "Base.h"
#include "GCString.h"
#include "INode.h"
#include "ISelector.h"
#include "IString.h"
#include "SpinnakerPlatform.h"
#include "Types.h"
#include "ValueNode.h"

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
 *  @defgroup StringNode_h StringNode Class
 */
/*@{*/

/**
 * @brief Interface for string properties
 */
class SPINNAKER_API StringNode : virtual public IString,
                                 virtual public ValueNode {
public:
  struct NodeImpl;
  StringNode();

  StringNode(std::shared_ptr<Node::NodeImpl> pString);

  virtual ~StringNode();

  /**
   * Set node value
   *
   * @param Value The value to set
   * @param Verify Enables AccessMode and Range verification (default = true)
   */
  virtual void SetValue(const GenICam::gcstring &Value, bool Verify = true);

  /**
   * Set node value
   */
  virtual IString &operator=(const GenICam::gcstring &Value);

  /**
   * Get node value
   *
   * @param Verify Enables Range verification (default = false). The AccessMode
   * is always checked
   * @param IgnoreCache If true the value is read ignoring any caches (default =
   * false)
   * @return The value read
   */
  virtual GenICam::gcstring GetValue(bool Verify = false,
                                     bool IgnoreCache = false);

  /**
   * Get node value
   */
  virtual GenICam::gcstring operator()();

  /**
   * Get node value
   */
  virtual GenICam::gcstring operator*();

  /**
   * Retrieves the maximum length of the string in bytes
   */
  virtual int64_t GetMaxLength();

  /**
   * overload SetReference for Value
   */
  virtual void SetReference(INode *pBase);

private:
  std::shared_ptr<Node::NodeImpl> m_pString;
};

typedef StringNode CStringRef;

/*@}*/
/*@}*/
} // namespace GenApi
} // namespace Spinnaker

#ifdef _WIN32
#pragma warning(pop)
#endif

#endif // SPINNAKER_GENAPI_STRINGNODE_H