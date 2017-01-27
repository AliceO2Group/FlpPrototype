#include <DataSampling/FairInjector.h>
#include <stdio.h>

namespace AliceO2 {
namespace DataSampling {

FairInjector::FairInjector() {
  mDevice = new InjectorDevice();
}

FairInjector::~FairInjector() {
  delete mDevice;
}

int FairInjector::injectSamples(std::vector<DataBlockContainer *> &dataBlocks)
{
  // copy the data and send it via FairMQ
  mDevice->sendData("asdf");
  return 0;
}

}
}