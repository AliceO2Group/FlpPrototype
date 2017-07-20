///
/// \file    MockInjector.cxx
/// \author  Barthelemy von Haller
///

#include <DataSampling/MockInjector.h>
#include <DataFormat/DataSet.h>

namespace AliceO2 {
namespace DataSampling {

int MockInjector::injectSamples(DataSetReference dataSet)
{
  // TODO we don't want to slow down the caller, thus we do nothing...
  // TODO This being said we might want in certain cases to print the stuff below.
  //
//  unsigned int nBlocks = (int) dataSet->size();
//  int totalSize = 0;
//  for (DataBlockContainerReference blockContainer : *dataSet) {
//    DataBlockHeaderBase *header = &blockContainer->getData()->header;
//    void *payload = blockContainer->getData()->data;
//    unsigned int blockSize = header->dataSize;
//    printf("%p : %d\n", payload, blockSize);
//    totalSize += blockSize;
//  }
//
//  printf("DataSampling injection: got %u blocks, total size %u bytes\n", nBlocks, totalSize);
  return 0;
}

}
}