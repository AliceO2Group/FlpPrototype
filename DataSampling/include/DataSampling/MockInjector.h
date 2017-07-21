///
/// \file    MockInjector.h
/// \author  Sylvain Chapeland
///

#ifndef DATA_SAMPLING_MOCK_INJECTOR_H
#define DATA_SAMPLING_MOCK_INJECTOR_H

#include <DataFormat/DataBlockContainer.h>
#include <vector>
#include <DataFormat/DataSet.h>
#include "InjectorInterface.h"

namespace AliceO2 {
namespace DataSampling {

class MockInjector: public InjectorInterface
{

/// \brief Example interface to inject data into data sampling system.
///
/// Function to be called from relevant data producers to inject data to data monitoring system
/// Data provided should not be used any more once function returns (if necessary, should be copied).
///
/// @todo Make it a class, add namespaces, rename.
///
/// \param dataSet A reference to a vector of data blocks. All data blocks in the vector have the same event id.
/// \return 0 on success, an error code otherwise
    int injectSamples(DataSetReference dataSet) override;

};

}
}

#endif //DATA_SAMPLING_MOCK_INJECTOR_H
