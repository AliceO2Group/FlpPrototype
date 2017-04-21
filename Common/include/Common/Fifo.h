///
/// \file    Fifo.h
/// \brief   Class to implement a lock-free 1-to-1 FIFO
/// \author  Sylvain Chapeland
///

#ifndef COMMON_FIFO_H
#define COMMON_FIFO_H

#include <vector>
#include <atomic>
#include <stdlib.h>

namespace AliceO2 {
namespace Common {

/// \brief   Class to implement a lock-free 1-to-1 FIFO (1 writer, 1 reader)
/// Two threads can safely work concurrently on the two sides of the FIFO (one using push(), the other using pop()).
/// External protection is needed for other types of concurrent access, functions are not re-entrant. (e.g. 2 threads calling push(), or 2 threads calling pop())
/// \author   Sylvain Chapeland
template <class T>
class Fifo {
  public:

    /// Constructor
    /// \param[in]  size   Size of the FIFO (number of elements it can hold).
    Fifo(int size);
    
    /// Destructor
    ~Fifo();

    /// Push an element in FIFO.
    /// \param[in]  data   Element to be added to FIFO.
    /// \return   0 on success
    int push(const T &data);    // push an element in FIFO. Returns 0 on success.
    
    /// Retrieve first element of FIFO.
    /// \param[in,out]  data   Element read from FIFO (by reference).
    /// \return   0 on success   
    int pop(T &data);

    /// Retrieve first element from FIFO, without removing it from FIFO.
    /// \param[in,out]  data   Element read from FIFO (by reference).
    /// \return   0 on success
    int front(T &data);

    /// Check if Fifo is full.
    /// \return   non-zero if FIFO full
    int isFull();

    /// Check if Fifo is empty.
    /// \return   non-zero if FIFO empty
    int isEmpty();
    
    /// Retrieve space available in FIFO
    /// \return   number of free slots in FIFO
    int getNumberOfFreeSlots();
    
    /// Retrieve space used in FIFO
    /// \return   number of pending items in FIFO
    int getNumberOfUsedSlots();
    
    /// clears FIFO content
    void clear();
    

    /// reset FIFO statistics
    void resetStats();

    /// \return   number of items written to FIFO
    unsigned long long getNumberIn();
    /// \return   number of items read from FIFO
    unsigned long long getNumberOut();
    
  private:

    int size; // size of FIFO (number of elements it can store)
    std::atomic<int> indexStart; // index of latest element poped
    std::atomic<int> indexEnd; // index of latest element pushed
    std::vector<T> data;  // array storing FIFO elements (circular buffer - has one more item than max number of elements stored)
    
    unsigned long long nIn; // statistics - number of elements pushed to FIFO
    unsigned long long nOut; // statistics - number of elements retrieved from FIFO
};



template <class T>
Fifo<T>::Fifo(int s) {
  indexStart=0;
  indexEnd=0;
  size=s;
  data.resize(size+1); // keep one extra slot to mark separation between begin/end of circular buffer
  resetStats();
}

template <class T>
Fifo<T>::~Fifo() {
  /*
  if (nIn!=nOut) {
    printf("FIFO stats - warning: %llu in, %llu out\n",nIn,nOut);
  } else {
    printf("FIFO stats : %llu in, %llu out\n",nIn,nOut);
  }
  */
}

template <class T>
int Fifo<T>::push(const T &item) {
  int indexEndNew;

//  printf("push : start=%d end=%d\n",(int)indexStart,(int)indexEnd);

  indexEndNew=this->indexEnd+1;
  if (indexEndNew>this->size) {
    indexEndNew=0;
  }

  // append new item only if some space left
  if (indexEndNew==this->indexStart) {
    return -1;
  }
  
  //printf("push \ %d = %p\n",indexEndNew,item);  
  data[indexEndNew]=item;
  this->indexEnd=indexEndNew;
  nIn++;
  return 0;
}

template <class T>
int Fifo<T>::pop(T &item) {

  //printf("pop : start=%d end=%d\n",(int)indexStart,(int)indexEnd);

  // check if FIFO empty
  if (isEmpty()) {
    return -1;
  }

  int new_indexStart;
  new_indexStart=this->indexStart+1;
  if (new_indexStart>this->size) {
    new_indexStart=0;
  }
  this->indexStart=new_indexStart;
  
  item=data[this->indexStart];
  data[this->indexStart]=0; // reset value, in case it is a shared_ptr
  //printf("pop \ %d = %p\n",new_indexStart,item);  
  nOut++;
  return 0;
}

template <class T>
int Fifo<T>::front(T &item) {

  //printf("front : start=%d end=%d\n",(int)indexStart,(int)indexEnd);

  // check if FIFO empty
  if (isEmpty()) {
    return -1;
  }

  item=data[(this->indexStart+1)%(this->size+1)];
  return 0;
}



template <class T>
int Fifo<T>::isEmpty() {
  if (indexEnd==indexStart) {
    return 1;
  }
  return 0;
}

template <class T>
int Fifo<T>::isFull() {
  if ( ((indexEnd+1)==indexStart) || ((indexEnd+1>size)&&(indexStart==0)) ) {
    return 1;
  }
  return 0;
}

template <class T>
int Fifo<T>::getNumberOfFreeSlots() {
  int i1=indexEnd;
  int i2=indexStart;
  if (i1>=i2) {
    return size-i1+i2;
  }
  return i2-i1-1;
}

template <class T>
int Fifo<T>::getNumberOfUsedSlots() {
  return size-getNumberOfFreeSlots();
}

template <class T>
void Fifo<T>::clear() {
  data.clear();
  indexStart=0;
  indexEnd=0;
  return;
}

template <class T> void Fifo<T>::resetStats() {
  nIn=0;
  nOut=0;
}

template <class T> unsigned long long Fifo<T>::getNumberIn() {return nIn;}

template <class T> unsigned long long Fifo<T>::getNumberOut()  {return nOut;}
 



} // namespace Common
} // namespace AliceO2

#endif // COMMON_FIFO_H
