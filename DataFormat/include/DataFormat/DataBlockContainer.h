#ifndef DATAFORMAT_DATABLOCKCONTAINER
#define DATAFORMAT_DATABLOCKCONTAINER

#include <DataFormat/MemPool.h>
#include <DataFormat/DataBlock.h>

#include <stdlib.h>

// A container class for data blocks.
// In particular, allows to take care of the block release after use.

class DataBlockContainer {

  public:
  DataBlockContainer(DataBlock *v_data=NULL);
  virtual ~DataBlockContainer();
  DataBlock *getData();

  protected:
  DataBlock *data;
};




class DataBlockContainerFromMemPool : public DataBlockContainer {

  public:
  DataBlockContainerFromMemPool(MemPool *pool,DataBlock *v_data=NULL);
  ~DataBlockContainerFromMemPool();

  private:
  MemPool *mp;
};




#endif
