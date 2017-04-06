///
/// \file   TaskFactory.cxx
/// \author Barthelemy von Haller
///

// O2
#include "Common/Exceptions.h"
// DataSampling
#include "DataSampling/SamplerFactory.h"
#include "DataSampling/MockSampler.h"
#include "DataSampling/FileSampler.h"
#include "DataSampling/FairSampler.h"

using namespace AliceO2::Common;
using namespace std;

namespace AliceO2 {
namespace DataSampling {

std::unique_ptr<SamplerInterface> SamplerFactory::create(std::string name)
{
  if (name == "MockSampler") {
    return make_unique<MockSampler>();
  } else   if (name == "FileSampler") {
    return make_unique<FileSampler>();
  } else   if (name == "FairSampler") {
    return make_unique<FairSampler>();
  } else {
    BOOST_THROW_EXCEPTION(FatalException() << errinfo_details("No sampler named " + name));
  }
  return nullptr;
}

}
}

