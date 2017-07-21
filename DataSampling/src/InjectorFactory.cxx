///
/// \file   TaskFactory.cxx
/// \author Barthelemy von Haller
///

// O2
#include "Common/Exceptions.h"
// DataSampling
#include "DataSampling/InjectorFactory.h"
#include "DataSampling/MockInjector.h"
#ifdef WITH_FAIRROOT
#include "DataSampling/FairInjector.h"
#endif

using namespace AliceO2::Common;

namespace AliceO2 {
namespace DataSampling {

std::unique_ptr<InjectorInterface> InjectorFactory::create(std::string name)
{
  if (name == "MockInjector") {
    return std::make_unique<MockInjector>();
#ifdef WITH_FAIRROOT
  } else if (name == "FairInjector") {
    return std::make_unique<FairInjector>();
#endif
  } else {
    BOOST_THROW_EXCEPTION(FatalException() << errinfo_details("No injector named " + name));
  }
}

}
}

