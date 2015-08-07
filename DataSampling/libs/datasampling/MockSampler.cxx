///
/// \file    MockSampler.cxx
/// \author  Barthelemy von Haller
///

#include "MockSampler.h"

namespace AliceO2 {
namespace DataSampling {

MockSampler::MockSampler()
{
  producer = new DataBlockProducer(10);
}

MockSampler::~MockSampler()
{
  delete producer;
}

DataBlock *MockSampler::getData(int timeout)
{
  // we keep a copy because here we have const and we want to return a non-const
  return producer->get();
}

void MockSampler::releaseData()
{
  producer->regenerate();
}

}
}