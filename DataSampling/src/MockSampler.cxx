///
/// \file    MockSampler.cxx
/// \author  Barthelemy von Haller
///

#include "DataSampling/MockSampler.h"

using namespace std;

namespace AliceO2 {
namespace DataSampling {

MockSampler::MockSampler() : mCurrentBlock(nullptr)
{
  mProducer = make_unique<DataBlockProducer>(true /*random*/, 1024 * 1024);
}

MockSampler::~MockSampler()
{
}

DataSetReference MockSampler::getData(int timeout)
{
  mCurrentBlock.reset();
  DataSetReference dataSetReference = mProducer->getDataSet(2 /*blocks*/ );
  mCurrentBlock = dataSetReference;
  return dataSetReference;
}

void MockSampler::releaseData()
{
  mCurrentBlock.reset();
}

}
}
