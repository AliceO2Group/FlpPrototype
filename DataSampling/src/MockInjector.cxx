///
/// \file    MockInjector.cxx
/// \author  Barthelemy von Haller
///

#include <DataSampling/MockInjector.h>

namespace AliceO2 {
namespace DataSampling {

/// @todo rename the file MockInjectSamples
int MockInjector::injectSamples(std::vector<std::shared_ptr<DataBlockContainer>> &dataBlocks)
{
  unsigned int nBlocks = (int) dataBlocks.size();
  int totalSize = 0;
  for (unsigned int i = 0; i < nBlocks; i++) {
    std::shared_ptr<DataBlockContainer> blockContainer = dataBlocks.at(i);
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