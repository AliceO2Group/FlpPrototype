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
//  return producer->get();

  unsigned int i = 0;
//  DataBlockProducer producer(false, 1024);
//  while (keepRunning) {
    auto *blocks = new std::vector<std::shared_ptr<DataBlockContainer>>();
    DataBlock *block = producer->get();
    std::shared_ptr<DataBlockContainer> containerPtr = std::make_shared<DataBlockContainer>(block);
    blocks->push_back(containerPtr);
//    producer.regenerate();
//  }
//  std::vector<std::shared_ptr<DataBlockContainer>> *v = nullptr;
  return blocks;
}

void MockSampler::releaseData()
{
  producer->regenerate();
}

}
}
