#include "runFairMQDevice.h"
#include "3-FairOnData.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& /*options*/)
{
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/)
{
  return new FairOnData();
}
