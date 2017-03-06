///
/// \file    FairInjector.h
/// \author  Barthelemy von Haller
///

#ifndef DATA_SAMPLING_FAIRINJECTOR_H
#define DATA_SAMPLING_FAIRINJECTOR_H

#include <DataFormat/DataBlockContainer.h>
#include <vector>
#include "InjectorInterface.h"
//#include "InjectorDevice.h"
#include <FairMQDevice.h>
#include <vector>
#include <DataFormat/DataBlockContainer.h>

namespace AliceO2 {
namespace DataSampling {

class FairInjector : public InjectorInterface, public FairMQDevice
{
  public:

    FairInjector();
    virtual ~FairInjector();

    void sendData(std::vector<std::shared_ptr<DataBlockContainer>> data);

    /// \brief Example interface to inject data into data sampling system.
    ///
    /// Function to be called from relevant data producers to inject data to data monitoring system
    /// Data provided should not be used any more once function returns (if necessary, should be copied).
    ///
    /// \param dataBlocks A reference to a vector of data blocks. All data blocks in the vector have the same event id.
    /// \return 0 on success, an error code otherwise
    int injectSamples(std::vector<std::shared_ptr<DataBlockContainer>> &dataBlocks);

  protected:
    virtual void Run();

    std::vector<std::shared_ptr<DataBlockContainer>> mDataBlocks;
    bool mAvailableData; // to make sure that we don't try to send something that is not there

//  private:
//    InjectorDevice* mDevice;
};

}
}

#endif //DATA_SAMPLING_FAIRINJECTOR_H
