///
/// \file   FairSampler.h
/// \author Barthelemy von Haller
///

#ifndef PROJECT_FAIRSAMPLER_H
#define PROJECT_FAIRSAMPLER_H

#include <FairMQDevice.h>
#include "SamplerInterface.h"

namespace AliceO2 {
namespace DataSampling {

class FairSampler : public SamplerInterface, public FairMQDevice
{
  public:
    /// Standard Constructor
    FairSampler();

    /// Destructor
    virtual ~FairSampler();

    DataBlock *getData(int timeout = 0);

    void releaseData();

  protected:
    bool HandleData(FairMQMessagePtr &, int);
    virtual void Run() override;

};

}
}

#endif //PROJECT_FAIRSAMPLER_H
