#include <DataFormat/MemPool.h>
#ifndef __APPLE__
#include <malloc.h>
#endif
#include <boost/format.hpp>
#include <sstream>
#include <iostream>

void MemPool::deletePages() {
  int nPagesUsed=0;
  
  if (pageTable!=NULL) {
    for (int i=0;i<numberOfPages;i++) {
      if (pageTable[i]!=NULL) {
        if(pageIsUsed[i].test_and_set()) {
          nPagesUsed++;
          pageIsUsed[i].clear();
        }
        free(pageTable[i]);
      }
    }
    delete[] pageTable;
    pageTable=NULL;
  }
  if (pageIsUsed!=NULL) {
    delete[] pageIsUsed;
    pageIsUsed=NULL;
  }
  if (nPagesUsed) {
    std::stringstream err;
    err << boost::format("Warning: still %d pages in use") % nPagesUsed;    
    std::cerr << err.str() << std::endl;
  }
}

MemPool::MemPool(int v_numberOfPages, int v_pageSize, int align) {

  int errorIx=-1;
  
  numberOfPages=v_numberOfPages;
  pageSize=v_pageSize;
  pageTable=NULL;
  pageIsUsed=NULL;
  
  pageTable=new void*[numberOfPages];
  pageIsUsed=new std::atomic_flag[numberOfPages];
  
  for (int i=0;i<numberOfPages;i++) {
    pageTable[i]=NULL;
    pageIsUsed[i].clear();
  }
  for (int i=0;i<numberOfPages;i++) {
#ifdef __APPLE__
    void *newPage=malloc(pageSize);
#else
    void *newPage=memalign(align,pageSize);
#endif
    if (newPage==NULL) {
      errorIx=i;
      break;
    }
    pageTable[i]=newPage;
  }
  if (errorIx>=0) {
    deletePages();
    std::stringstream err;
    err << boost::format("Failed to allocate page %d / %d sized %d") % errorIx % numberOfPages % pageSize;
    throw err.str();
  }
  
  lastPageIndexGet=-1;
  lastPageIndexRelease=-1;
}

MemPool::~MemPool() {
  deletePages();
}


void *MemPool::getPage() {
  int j=0;
  for (int i=0;i<numberOfPages;i++) {
    int newIx=(lastPageIndexGet + 1 + i) % numberOfPages;
    if (!pageIsUsed[newIx].test_and_set()) {
      lastPageIndexGet=newIx;
      //printf("getPage() scan => %d\n",j);
      return pageTable[newIx];
    }
    j++;
  }
  return NULL;
}

// todo: use HASH for fast ptr->index access


void MemPool::releasePage(void *pagePtr) {
  int j=0;
  for (int i=0;i<numberOfPages;i++) {
    int pageIx=(lastPageIndexRelease+i+1) % numberOfPages;
    if (pagePtr==pageTable[pageIx]) {
      pageIsUsed[pageIx].clear();
      lastPageIndexRelease=pageIx;
      //printf("realeasePage() scan => %d\n",j);
      return;
    }
    j++;
  }
}

int MemPool::getPageSize () {
  return pageSize;
}
  
