#include <DataFormat/DataBlockContainer.h>
#include <string>

// base DataBlockContainer class

DataBlockContainer::DataBlockContainer(DataBlock *v_data) : data(v_data) {
}

DataBlockContainer::~DataBlockContainer() {
}

DataBlock * DataBlockContainer::getData() {
  return data;
}


// container for data pages coming fom MemPool class

DataBlockContainerFromMemPool::DataBlockContainerFromMemPool(MemPool *pool,DataBlock *v_data) {
  mp=pool;
  if (mp==NULL) {
    throw std::string("NULL argument");
  }
  data=v_data;
  if (data==NULL) {
    data=(DataBlock*)mp->getPage();
    if (data==NULL) {
      throw std::string("No page available");
    }
  }
}


DataBlockContainerFromMemPool::~DataBlockContainerFromMemPool() {
  if (mp!=NULL) {
    if (data!=NULL) {
      mp->releasePage(data);
    }
  }
}  
