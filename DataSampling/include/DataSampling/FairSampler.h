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

namespace AliceO2 {
namespace DataSampling {

class FairSampler : public SamplerInterface, public FairMQDevice
{
  public:
    /// Standard Constructor
    FairSampler();

    /// Destructor
    virtual ~FairSampler();

    std::vector<std::shared_ptr<DataBlockContainer>> * getData(int timeout = 0);

    void releaseData();

  protected:
    bool HandleData(FairMQParts& parts, int /*index*/);
    virtual void Run() override;
    virtual void PreRun() override {
      std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Needed for some obscure reason
    };

  private:
    std::vector<std::shared_ptr<DataBlockContainer>>* mBlock;
    std::timed_mutex mBlockMutex;

};

}
}

#endif //PROJECT_FAIRSAMPLER_H
