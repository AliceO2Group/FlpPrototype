///
/// \file    FairInjector.h
/// \author  Barthelemy von Haller
///

#ifndef DATA_SAMPLING_FAIRINJECTOR_H
#define DATA_SAMPLING_FAIRINJECTOR_H

#include <DataFormat/DataBlockContainer.h>
#include <vector>
#include "InjectorInterface.h"
#include "InjectorDevice.h"

namespace AliceO2 {
namespace DataSampling {

class FairInjector : public InjectorInterface
{
  public:

    FairInjector();
    virtual ~FairInjector();

    /// \brief Example interface to inject data into data sampling system.
    ///
    /// Function to be called from relevant data producers to inject data to data monitoring system
    /// Data provided should not be used any more once function returns (if necessary, should be copied).
    ///
    /// \param dataBlocks A reference to a vector of data blocks. All data blocks in the vector have the same event id.
    /// \return 0 on success, an error code otherwise
    int injectSamples(std::vector<std::shared_ptr<DataBlockContainer>> &dataBlocks);

  private:
    InjectorDevice* mDevice;
};

}
}

#endif //DATA_SAMPLING_DATABLOCKPRODUCER_H
