///
/// \file    FairInjector.h
/// \author  Barthelemy von Haller
///

#ifndef DATA_SAMPLING_FAIRINJECTOR_H
#define DATA_SAMPLING_FAIRINJECTOR_H

#include <Common/DataBlockContainer.h>
#include <vector>
#include "DataSampling/InjectorInterface.h"
#include <FairMQDevice.h>
#include <vector>
#include <Common/DataBlockContainer.h>
#include <Common/DataSet.h>

namespace AliceO2 {
namespace DataSampling {

class FairInjector : public InjectorInterface, public FairMQDevice
{
  public:

    FairInjector();
    ~FairInjector() override;

    /// \brief Example interface to inject data into data sampling system.
    ///
    /// Function to be called from relevant data producers to inject data to data monitoring system
    /// Data provided should not be used any more once function returns (if necessary, should be copied).
    ///
    /// \param dataSetReference A reference to a vector of data blocks. All data blocks in the vector have the same event id.
    /// \return 0 on success, an error code otherwise
    int injectSamples(DataSetReference dataSetReference) override;

  protected:
    void Run() override;
};

}
}

#endif //DATA_SAMPLING_FAIRINJECTOR_H
