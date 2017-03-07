///
/// \file    InjectSamples.h
/// \author  Sylvain Chapeland
///

#ifndef DATA_SAMPLING_INJECTSAMPLES_H
#define DATA_SAMPLING_INJECTSAMPLES_H

#include <DataFormat/DataSet.h>

#include <vector>
#include <memory>

// Example interface to inject data into data sampling system.
//
// Function to be called from relevant data producers to inject data to data monitoring system.
// Sample can be kept (but not modified) after function returns if necessary. Will be deleted automatically when not used (referenced) any more.
// Returns 0 on success, an error code otherwise

int injectSamples(const DataSetReference sample);


#endif //DATA_SAMPLING_DATABLOCKPRODUCER_H
