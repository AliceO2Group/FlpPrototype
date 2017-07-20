///
/// \file    MockInjector.cxx
/// \author  Barthelemy von Haller
///

#include <DataSampling/MockInjector.h>
#include <DataFormat/DataSet.h>

namespace AliceO2 {
namespace DataSampling {

// TODO use MockDataBlockContainer like in DataBlockProducer
int MockInjector::injectSamples(DataSetReference dataBlocks)
{
  unsigned int nBlocks = (int) dataBlocks->size();
  int totalSize = 0;
  for (DataBlockContainerReference blockContainer : *dataBlocks) {
    DataBlockHeaderBase *header = &blockContainer->getData()->header;
    void *payload = blockContainer->getData()->data;
    unsigned int blockSize = header->dataSize;
    printf("%p : %d\n", payload, blockSize);
    totalSize += blockSize;
  }

  printf("DataSampling injection: got %u blocks, total size %u bytes\n", nBlocks, totalSize);
  return 0;
}

}
}