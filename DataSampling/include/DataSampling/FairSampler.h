///
/// \file   FairSampler.h
/// \author Barthelemy von Haller
///

#ifndef PROJECT_FAIRSAMPLER_H
#define PROJECT_FAIRSAMPLER_H

#include <FairMQDevice.h>
#include <DataFormat/DataBlockContainer.h>
#include "SamplerInterface.h"
#include <mutex>
#include <thread>
//#include <DataFormat/DataSet.h>

namespace AliceO2 {
namespace DataSampling {

class FairSampler : public SamplerInterface, public FairMQDevice
{
  public:
    /// Standard Constructor
    FairSampler();

    /// Destructor
    ~FairSampler() override;

    DataSetReference getData(int timeout = 0) override;

    void releaseData() override;

  protected:
    void Run() override;
    void PreRun() override {
      std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Needed for some obscure reason
    };

  private:
    DataSetReference mDataSet;
    std::timed_mutex mBlockMutex;
    bool mReceivingDataSet, mReceivingDataBlock;
};

}
}

#endif //PROJECT_FAIRSAMPLER_H
