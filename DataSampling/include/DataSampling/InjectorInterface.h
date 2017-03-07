///
/// \file    InjectorInterface.h
/// \author  Barthelemy von Haller
///

#ifndef DATA_SAMPLING_INJECTOR_INTERFACE_H
#define DATA_SAMPLING_INJECTOR_INTERFACE_H

#include <DataFormat/DataBlockContainer.h>
#include <vector>
#include <memory>

namespace AliceO2 {
namespace DataSampling {

class InjectorInterface
{

  public:
    virtual ~InjectorInterface(){};

/// \brief Example interface to inject data into data sampling system.
///
/// Function to be called from relevant data producers to inject data to data monitoring system
/// Data provided should not be used any more once function returns (if necessary, should be copied).
///
/// @todo Make it a class, add namespaces, rename.
///
/// \param dataBlocks A reference to a vector of data blocks. All data blocks in the vector have the same event id.
/// \return 0 on success, an error code otherwise
    virtual int injectSamples(std::vector<std::shared_ptr<DataBlockContainer>> &dataBlocks) = 0;

};
}
}
#endif //DATA_SAMPLING_INJECTOR_INTERFACE_H
