#ifndef DATAFORMAT_DATABLOCKCONTAINER
#define DATAFORMAT_DATABLOCKCONTAINER

#include <DataFormat/MemPool.h>
#include <DataFormat/DataBlock.h>

#include <stdlib.h>
#include <memory>

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
  DataBlockContainerFromMemPool(std::shared_ptr<MemPool> pool, DataBlock *v_data=NULL);
  ~DataBlockContainerFromMemPool();

  private:
  std::shared_ptr<MemPool> mp;
};


/**
 * DataBlockContainer that takes ownership of the payload and deletes it when needed.
 */
class SelfReleasingBlockContainer : public DataBlockContainer
{

  public:
    SelfReleasingBlockContainer()
    {
      data = new DataBlock();
      data->data = nullptr;
    }

    ~SelfReleasingBlockContainer()
    {
      if (data->data != nullptr) {
        delete[] data->data;
      }
      delete data;
    }
};

#endif
