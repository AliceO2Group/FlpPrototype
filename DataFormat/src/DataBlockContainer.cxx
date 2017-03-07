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

DataBlockContainerFromMemPool::DataBlockContainerFromMemPool(std::shared_ptr<MemPool> pool, DataBlock *v_data) {
  mp=pool;
  if (mp==nullptr) {
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
  if (mp!=nullptr) {
    if (data!=nullptr) {
      mp->releasePage(data);
    }
  }
}  
