///
/// \file    MockSampler.cxx
/// \author  Barthelemy von Haller
///

#include <DataFormat/DataBlock.h>
#include "DataSampling/MockSampler.h"

using namespace std;

namespace AliceO2 {
namespace DataSampling {

MockSampler::MockSampler() : mCurrentBlock(nullptr)
{
  producer = new DataBlockProducer(true, 1024 * 1024, false);
}

MockSampler::~MockSampler()
{
  delete producer;
}

std::vector<std::shared_ptr<DataBlockContainer>> *MockSampler::getData(int timeout)
{
  if (mCurrentBlock) {
    delete mCurrentBlock;
    mCurrentBlock = nullptr;
  }
  auto *blocks = new std::vector<std::shared_ptr<DataBlockContainer>>();

  producer->regenerate();
  DataBlock *block = producer->get();
  std::shared_ptr<DataBlockContainer> containerPtr = std::make_shared<DataBlockContainer>(block);
  blocks->push_back(containerPtr);

  producer->regenerate();
  DataBlock *block2 = producer->get();
  std::shared_ptr<DataBlockContainer> containerPtr2 = std::make_shared<DataBlockContainer>(block2);
  blocks->push_back(containerPtr2);

  mCurrentBlock = blocks;
  return blocks;
}

void MockSampler::releaseData()
{
  if (mCurrentBlock) {
    for (std::shared_ptr<DataBlockContainer> block : *mCurrentBlock) {
      if (block->getData()->data) {
        delete[] block->getData()->data;
      }
      delete block->getData(); // REALLY ??
    }
    delete mCurrentBlock;
    mCurrentBlock = nullptr;
  }
}

}
}
