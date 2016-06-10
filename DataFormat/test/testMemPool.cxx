/// \file testDataFormat.c
/// \brief Example of C usage of data format used in DataBlock.h.
///
/// \author Sylvain Chapeland, CERN

#include "DataFormat/MemPool.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <boost/format.hpp>

int main() {

  int nErr=0;
  MemPool *mp;
  int nPages=100;
  int pageSize=10*1024*1024;
  std::vector<void *> pageUsed;
  
    
  printf("Creating pool %d pages x %d bytes\n",nPages,pageSize);
  mp=new MemPool(nPages,pageSize);
  
  printf("Get %d pages from pool\n",nPages);
  nErr=0;
  for (int i=0;i<nPages;i++) {
    void *newPage=mp->getPage();
    if (newPage!=NULL) {
      pageUsed.push_back(newPage);
    } else {
      nErr++;
    }
  }
  if (nErr) {
    printf("%d errors\n",nErr);
  }
  
  printf("Release %d pages\n",nPages/2);
  nErr=0;
  for (int i=0;i<nPages/2;i++) {
    void *newPage=pageUsed.back();
    pageUsed.pop_back();
    if (newPage!=NULL) {
      mp->releasePage(newPage);
    } else {
      nErr++;
    }
  }
  if (nErr) {
    printf("%d errors\n",nErr);
  }

  printf("Get %d pages from pool\n",nPages);
  nErr=0;
  for (int i=0;i<nPages;i++) {
    void *newPage=mp->getPage();
    if (newPage!=NULL) {
      pageUsed.push_back(newPage);
    } else {
      nErr++;
    }
  }
  if (nErr) {
    printf("%d errors\n",nErr);
  }

  printf("Release all pages\n");
  nErr=0;
  while (!pageUsed.empty()) {
    void *newPage=pageUsed.back();
    pageUsed.pop_back();
    if (newPage!=NULL) {
      mp->releasePage(newPage);
    } else {
      nErr++;
    }
  }
  if (nErr) {
    printf("%d errors\n",nErr);
  }


  printf("Delete pool\n");
  delete mp;
  return 0;
}
