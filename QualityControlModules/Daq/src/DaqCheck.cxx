///
/// \file   DaqCheck.cxx
/// \author Barthelemy von Haller
///

#include "Daq/DaqCheck.h"

// ROOT
#include <TH1.h>

using namespace std;

ClassImp(AliceO2::QualityControlModules::Daq::DaqCheck)

namespace AliceO2 {
namespace QualityControlModules {
namespace Daq {

DaqCheck::DaqCheck()
{
}

DaqCheck::~DaqCheck()
{
}

void DaqCheck::configure(std::string name)
{
  // First call the parent !
  AliceO2::QualityControl::Checker::CheckInterface::configure(name);
}

Quality DaqCheck::check(const MonitorObject *mo)
{
  Quality result = Quality::Null;

  return result;
}

std::string DaqCheck::getAcceptedType()
{
  return "TH1";
}

void DaqCheck::beautify(MonitorObject *mo, Quality checkResult)
{
// NOOP
}

} // namespace Daq
} // namespace QualityControl
} // namespace AliceO2

