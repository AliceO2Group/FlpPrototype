///
/// \file    InjectSamples.h
/// \author  Sylvain Chapeland
///

#ifndef DATA_SAMPLING_INJECTSAMPLES_H
#define DATA_SAMPLING_INJECTSAMPLES_H

#include <DataFormat/DataBlockContainer.h>
#include <vector>


// Example interface to inject data into data sampling system.
//
// Function to be called from relevant data producers to inject data to data monitoring system
// Data provided should not be used any more once function returns (if necessary, should be copied).
// Argument is a reference to a vector of data blocks. All data blocks in the vector have the same event id.
// Returns 0 on success, an error code otherwise

int injectSamples(std::vector<DataBlockContainer *> &dataBlocks);


#endif //DATA_SAMPLING_DATABLOCKPRODUCER_H
