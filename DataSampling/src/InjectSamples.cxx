#include <DataSampling/InjectSamples.h>
#include <stdio.h>

//int injectSamples(std::vector<std::shared_ptr<DataBlockContainer>> &dataBlocks) {

int injectSamples(const DataSetReference sample) {
  

  unsigned int nBlocks=(int)sample->size();  
  int totalSize=0;
  for (unsigned int i=0;i<nBlocks;i++) {
    DataBlockContainerReference blockContainer=sample->at(i);
        
    DataBlockHeaderBase *header=&blockContainer->getData()->header;   
    char *payload=blockContainer->getData()->data;
    
    unsigned int blockSize=header->dataSize;
    //printf("%p : %d\n",payload,blockSize);
    totalSize+=blockSize;
  }
  printf("DataSampling injection: got %u blocks, total size %u bytes\n",nBlocks,totalSize);
  
  return 0;
}
