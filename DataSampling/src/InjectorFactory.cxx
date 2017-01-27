///
/// \file   TaskFactory.cxx
/// \author Barthelemy von Haller
///

// O2
#include "Common/Exceptions.h"
// DataSampling
#include "DataSampling/InjectorFactory.h"
#include "DataSampling/MockInjector.h"
#include "DataSampling/FairInjector.h"

using namespace AliceO2::Common;

namespace AliceO2 {
namespace DataSampling {

InjectorInterface *InjectorFactory::create(std::string name)
{
  if (name == "MockInjector") {
    return new MockInjector();
  } else if (name == "FairInjector") {
    return new FairInjector();
  } else {
    BOOST_THROW_EXCEPTION(FatalException() << errinfo_details("No injector named " + name));
  }
  return nullptr;
}

}
}

