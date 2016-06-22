///
/// \file   NonEmpty.cxx
/// \author Barthelemy von Haller
///

#include "Common/NonEmpty.h"

// std
#include <assert.h>
#include <iostream>
// ROOT
#include <TH1.h>

using namespace std;

ClassImp(AliceO2::QualityControlModules::Common::NonEmpty)

namespace AliceO2 {
namespace QualityControlModules {
namespace Common {

NonEmpty::NonEmpty()
{
}

NonEmpty::~NonEmpty()
{
}

void NonEmpty::configure(std::string name)
{
  // First call the parent !
  CheckInterface::configure(name);
}

Quality NonEmpty::check(const MonitorObject *mo)
{
  Quality result = Quality::Null;

  // The framework guarantees that the encapsulated object is of the accepted type.
  TH1 *histo = dynamic_cast<TH1*>(mo->getObject());

  // assert(histo != nullptr);
  if (histo != nullptr) {
    if (histo->GetEntries() > 0) {
      result = Quality::Good;
    } else {
      result = Quality::Bad;
    }
  }

  return result;
}

std::string NonEmpty::getAcceptedType()
{
  return "TH1";
}

void NonEmpty::beautify(MonitorObject *mo, Quality checkResult)
{
// NOOP
}

}  // namespace Checker
}  // namespace QualityControl
} // namespace AliceO2 

