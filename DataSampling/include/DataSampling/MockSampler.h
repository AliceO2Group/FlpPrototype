///
/// \file    MockSampler.h
/// \author  Barthelemy von Haller
///

#ifndef DATA_SAMPLING_MOCKSAMPLER_H
#define DATA_SAMPLING_MOCKSAMPLER_H

#include <DataFormat/DataBlock.h>
#include <memory>
#include "SamplerInterface.h"
#include "DataBlockProducer.h"

namespace AliceO2 {
namespace DataSampling {

/// \brief Sampler that returns blocks of data with a random payload.
///
/// This data sampler returns blocks of data with a random payload.
/// The size of the payload follows a standard distribution using the default parameters of DataBlockProducer.
/// The blocks that are returned are a ptr to a vector of 2 elements each being a shared_ptr<DataBlockContainer>.
///
/// \author Barthelemy von Haller
class MockSampler : public SamplerInterface
{
  public:
    /// Standard Constructor
    MockSampler();
    /// Destructor
    ~MockSampler() override;

    /**
     * Get the next data block available.
     * @param timeout
     * @return A ptr to a vector of 2 shared_ptr of DataBlockContainer.
     */
    std::vector<std::shared_ptr<DataBlockContainer>>* getData(int timeout = 0) override;
    void releaseData() override;

  private:
    DataBlockProducer *producer;
    std::vector<std::shared_ptr<DataBlockContainer>>* mCurrentBlock;

};

}
}

#endif //DATA_SAMPLING_MOCKSAMPLER_H
