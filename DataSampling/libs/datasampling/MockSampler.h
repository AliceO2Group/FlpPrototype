///
/// \file    MockSampler.h
/// \author  Barthelemy von Haller
///

#ifndef DATA_SAMPLING_MOCKSAMPLER_H
#define DATA_SAMPLING_MOCKSAMPLER_H

#include <DataBlock.h>
#include "SamplerInterface.h"
#include "DataBlockProducer.h"

namespace AliceO2 {
namespace DataSampling {

/// \brief Sampler that returns blocks of data with a payload 10 bytes long.
/// \author Barthelemy von Haller
class MockSampler : public SamplerInterface
{
  public:
    /// Standard Constructor
    MockSampler();
    /// Destructor
    virtual ~MockSampler();

    virtual DataBlock* getData(int timeout = 0);
    virtual void releaseData();

  private:
    DataBlockProducer *producer;
    DataBlock *currentBlock;

};

}
}

#endif //DATA_SAMPLING_MOCKSAMPLER_H
