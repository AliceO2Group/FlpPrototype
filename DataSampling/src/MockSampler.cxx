///
/// \file    MockSampler.cxx
/// \author  Barthelemy von Haller
///

#include "DataSampling/MockSampler.h"

namespace AliceO2 {
namespace DataSampling {

MockSampler::MockSampler()
{
  producer = new DataBlockProducer(true);
}

MockSampler::~MockSampler()
{
  delete producer;
}

std::vector<std::shared_ptr<DataBlockContainer>> * MockSampler::getData(int timeout)
{
  // we keep a copy because here we have const and we want to return a non-const
//  return producer->get(); TODO make it return a DataBlockContainer
  std::vector<std::shared_ptr<DataBlockContainer>> *v = nullptr;
  return v; // TODO make it return a DataBlockContainer
}

void MockSampler::releaseData()
{
  producer->regenerate();
}

}
}
