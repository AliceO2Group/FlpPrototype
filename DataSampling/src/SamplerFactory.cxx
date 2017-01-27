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

namespace AliceO2 {
namespace DataSampling {

SamplerFactory::SamplerFactory()
{

}

SamplerFactory::~SamplerFactory()
{

}

SamplerInterface *SamplerFactory::create(std::string name)
{
  if (name == "MockSampler") {
    return new MockSampler();
  } else   if (name == "FileSampler") {
    return new FileSampler();
  } else   if (name == "FairSampler") {
    return new FairSampler();
  } else {
    BOOST_THROW_EXCEPTION(FatalException() << errinfo_details("No sampler named " + name));
  }
  return nullptr;
}

}
}

