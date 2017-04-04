/// \file Iommu.h
/// \brief Definition of functions related to the IOMMU
///
/// \author Pascal Boeschoten (pascal.boeschoten@cern.ch)

#ifndef ALICEO2_FLPPROTOTYPE_COMMON_IOMMU_H_
#define ALICEO2_FLPPROTOTYPE_COMMON_IOMMU_H_

namespace AliceO2
{
namespace Common
{
namespace Iommu
{

/// Checks if the IOMMU is enabled
bool isEnabled();

} // namespace Iommu
} // namespace Common
} // namespace AliceO2

#endif // ALICEO2_FLPPROTOTYPE_COMMON_IOMMU_H_
