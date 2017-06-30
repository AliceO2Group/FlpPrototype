///
/// \file    MockSampler.cxx
/// \author  Barthelemy von Haller
///

#include "DataSampling/MockSampler.h"

namespace AliceO2 {
namespace DataSampling {

MockSampler::MockSampler() : currentBlock(nullptr)
{
  producer = new DataBlockProducer(true);
}

MockSampler::~MockSampler()
{
  delete producer;
}

std::vector<std::shared_ptr<DataBlockContainer>> *MockSampler::getData(int timeout)
{
  if (currentBlock) {
    delete currentBlock;
    currentBlock = nullptr;
  }
  unsigned int i = 0;
  auto *blocks = new std::vector<std::shared_ptr<DataBlockContainer>>();
  DataBlock *block = producer->get();
  std::shared_ptr<DataBlockContainer> containerPtr = std::make_shared<DataBlockContainer>(block);
  blocks->push_back(containerPtr);
  currentBlock = blocks;
  return blocks;
}

void MockSampler::releaseData()
{
  delete currentBlock;
  currentBlock = nullptr;
  producer->regenerate();
}

}
}
