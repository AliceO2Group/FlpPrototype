/// \file testDataFormat.c
/// \brief Example of C usage of data format used in DataBlock.h.
///
/// \author Sylvain Chapeland, CERN

#include "DataFormat/DataBlock.h"
#include <stdlib.h>
#include <stdio.h>

int main() {

  int i,j;
  
  // construct a data block with 1 level of sub-blocks
  int payloadSize=100;
  int numberOfSubBlocks=10;
  int subBlocksSize=numberOfSubBlocks*(sizeof(DataBlockHeaderBase)+payloadSize);

  DataBlockHeaderBase *topHeader=(DataBlockHeaderBase *) malloc(sizeof(DataBlockHeaderBase)+subBlocksSize);
  
  topHeader->blockType=H_BASE;
  topHeader->headerSize=sizeof(DataBlockHeaderBase);
  topHeader->dataSize=subBlocksSize;

  for (i=0; i<numberOfSubBlocks; i++) {
    DataBlockHeaderBase *subHeader=(DataBlockHeaderBase *)&((char *)topHeader)[sizeof(DataBlockHeaderBase)+(sizeof(DataBlockHeaderBase)+payloadSize)*i];
    subHeader->blockType=H_BASE;
    subHeader->headerSize=sizeof(DataBlockHeaderBase);
    subHeader->dataSize=payloadSize;
    char *payload=&((char *)subHeader)[subHeader->headerSize];
    int j;
    for (j=0;j<payloadSize;j++) {
      payload[j]=(char) (i*10 + j % 10);
    }
  }

  // browse a data block with 1 level of sub-blocks (all of base type)
  DataBlockHeaderBase *mainBlockPtr=topHeader;
  printf("Main block info: 0x%p type=%X headerSize=%d payloadSize=%d\n",mainBlockPtr,(int)mainBlockPtr->blockType,mainBlockPtr->headerSize,mainBlockPtr->dataSize);

  // compute end of data block
  DataBlockHeaderBase *endPtr=(DataBlockHeaderBase *)&((char *)mainBlockPtr)[mainBlockPtr->headerSize+mainBlockPtr->dataSize];

  // iterate on sub-blocks
  DataBlockHeaderBase *subBlockPtr=(DataBlockHeaderBase *)&((char *)mainBlockPtr)[mainBlockPtr->headerSize];
  for (i=1; ; i++) {
    if (subBlockPtr>=endPtr) break;
    printf("  sub-block %d @ 0x%p\n",i,subBlockPtr);
    printf("    type=%X headerSize=%d payloadSize=%d\n",(int)subBlockPtr->blockType,subBlockPtr->headerSize,subBlockPtr->dataSize);    
    char *data;
    data=&((char *)subBlockPtr)[subBlockPtr->headerSize];
    for (j=0;j<subBlockPtr->dataSize;j++) {
      printf("%d  ",(int)data[j]);
    }
    printf("\n");
    subBlockPtr=(DataBlockHeaderBase *)&((char *)subBlockPtr)[sizeof(DataBlockHeaderBase)+payloadSize];
  }

  free(topHeader);
  return 0;
}
