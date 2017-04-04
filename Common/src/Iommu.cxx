/// \file Iommu.cxx
/// \brief Implementation of functions related to the IOMMU
///
/// \author Pascal Boeschoten (pascal.boeschoten@cern.ch)

#include "Common/Iommu.h"
#include <boost/filesystem/operations.hpp>

namespace AliceO2
{
namespace Common
{
namespace Iommu
{

bool isEnabled()
{
  // This should do the trick...
  return boost::filesystem::exists("/sys/kernel/iommu_groups/0");
}

} // namespace Util
} // namespace Common
} // namespace AliceO2
