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

#ifndef FLIR_SPINNAKER_IIMAGE_H
#define FLIR_SPINNAKER_IIMAGE_H

#include "CameraDefs.h"
#include "ChunkData.h"
#include "SpinGenApi/GCString.h"
#include "SpinGenApi/GCTypes.h"
#include "SpinnakerDefs.h"
#include "SpinnakerPlatform.h"

namespace Spinnaker {
class ImageStatistics;
class ImagePtr;
enum PixelFormatEnums;

/**
 *  @defgroup SpinnakerClasses Spinnaker Classes
 */
/*@{*/

/**
 *  @defgroup IImage_h IImage Class
 */
/*@{*/

/**
 * @brief The interface file for Image.
 */

class SPINNAKER_API IImage {
public:
  virtual ~IImage(){};

  virtual ColorProcessingAlgorithm GetColorProcessing() const = 0;
  virtual ImagePtr
  Convert(Spinnaker::PixelFormatEnums format,
          ColorProcessingAlgorithm colorAlgorithm = DEFAULT) const = 0;
  virtual void
  Convert(ImagePtr destinationImage, Spinnaker::PixelFormatEnums format,
          ColorProcessingAlgorithm colorAlgorithm = DEFAULT) const = 0;
  DEPRECATED_FUNC("This function has been deprecated. Please use more specific "
                  "functions defined in the ImageUtilityPolarization class.",
                  virtual ImagePtr ExtractPolarization(
                      const PolarizationAlgorithm polarizationAlogrithm,
                      const PolarizationResolution resolution) const = 0;);
  virtual void ResetImage(size_t width, size_t height, size_t offsetX,
                          size_t offsetY,
                          Spinnaker::PixelFormatEnums pixelFormat) = 0;
  virtual void ResetImage(size_t width, size_t height, size_t offsetX,
                          size_t offsetY,
                          Spinnaker::PixelFormatEnums pixelFormat,
                          void *pData) = 0;
  virtual void Release() = 0;
  virtual uint64_t GetID() const = 0;
  virtual void *GetData() const = 0;
  virtual void *GetPrivateData() const = 0;
  virtual double GetDataAbsoluteMax() const = 0;
  virtual double GetDataAbsoluteMin() const = 0;
  virtual size_t GetBufferSize() const = 0;
  virtual void DeepCopy(const ImagePtr pSrcImage) = 0;
  virtual size_t GetWidth() const = 0;
  virtual size_t GetHeight() const = 0;
  virtual size_t GetStride() const = 0;
  virtual size_t GetBitsPerPixel() const = 0;
  virtual size_t GetNumChannels() const = 0;
  virtual size_t GetXOffset() const = 0;
  virtual size_t GetYOffset() const = 0;
  virtual size_t GetXPadding() const = 0;
  virtual size_t GetYPadding() const = 0;
  virtual uint64_t GetFrameID() const = 0;
  virtual size_t GetPayloadType() const = 0;
  virtual PayloadTypeInfoIDs GetTLPayloadType() const = 0;
  virtual uint64_t GetTLPixelFormat() const = 0;
  virtual PixelFormatNamespaceID GetTLPixelFormatNamespace() const = 0;
  virtual GenICam::gcstring GetPixelFormatName() const = 0;
  virtual Spinnaker::PixelFormatEnums GetPixelFormat() const = 0;
  virtual Spinnaker::PixelFormatIntType GetPixelFormatIntType() const = 0;
  virtual bool IsIncomplete() const = 0;
  virtual size_t GetValidPayloadSize() const = 0;
  virtual uint64_t GetChunkLayoutId() const = 0;
  virtual uint64_t GetTimeStamp() const = 0;
  virtual void Save(const char *pFilename,
                    ImageFileFormat format = FROM_FILE_EXT) = 0;
  virtual void Save(const char *pFilename, PNGOption &pOption) = 0;
  virtual void Save(const char *pFilename, PPMOption &pOption) = 0;
  virtual void Save(const char *pFilename, PGMOption &pOption) = 0;
  virtual void Save(const char *pFilename, TIFFOption &pOption) = 0;
  virtual void Save(const char *pFilename, JPEGOption &pOption) = 0;
  virtual void Save(const char *pFilename, JPG2Option &pOption) = 0;
  virtual void Save(const char *pFilename, BMPOption &pOption) = 0;
  virtual const ChunkData &GetChunkData() const = 0;
  virtual void CalculateStatistics(ImageStatistics &pStatistics) = 0;
  virtual bool HasCRC() const = 0;
  virtual bool CheckCRC() const = 0;
  virtual size_t GetImageSize() const = 0;
  virtual bool IsInUse() = 0;
  virtual ImageStatus GetImageStatus() const = 0;
  DEPRECATED_FUNC(
      "This function has been deprecated. Polarization images created through "
      "the ImageUtilityPolarization class now use an appropriate pixel format "
      "to hold the raw polarization values.",
      virtual float *GetPolarizationValues() const = 0;);
  DEPRECATED_FUNC("This function has been deprecated. Polarization algorithms "
                  "are applied through specific functions defined in the "
                  "ImageUtilityPolarization class.",
                  virtual PolarizationAlgorithm GetPolarizationAlgorithm()
                      const = 0;);

protected:
  IImage(){};
};

/*@}*/

/*@}*/
} // namespace Spinnaker

#endif // FLIR_SPINNAKER_IIMAGE_H