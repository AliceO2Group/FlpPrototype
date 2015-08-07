///
/// \file    DataBlockProducer.cxx
/// \author  Barthelemy von Haller
///

#include <DataBlock.h>
#include "DataBlockProducer.h"
#include <fstream>
#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */

namespace AliceO2 {
namespace DataSampling {

DataBlockProducer::DataBlockProducer(bool random, uint32_t payloadSize) : mCurrent(0),
                                                                          mCurrentPayloadSize(payloadSize),
                                                                          mRandom(random)
{
  srand(time(NULL));
  regenerate();
}

DataBlockProducer::~DataBlockProducer()
{
  delete mCurrent->data;
  delete mCurrent;
}

/// Ownership remains with DataBlockProducer.
DataBlock *DataBlockProducer::get() const
{
  return mCurrent;
}

/// Note : the file is opened and closed each time.
void DataBlockProducer::saveToFile(std::string pathToFile, bool append) const
{
  std::ios_base::openmode extra_mode = append ? std::ios::app : std::ios::trunc;
  std::fstream file(pathToFile, std::ios::out | std::ios::binary | extra_mode);
  file.write((char *) &mCurrent->header, sizeof(DataBlockHeaderBase));
  file.write(mCurrent->data, mCurrentPayloadSize);
  file.close();
}

void DataBlockProducer::regenerate()
{
  if (mCurrent) {
    if (mCurrent->data) {
      delete mCurrent->data;
    }
    delete mCurrent;
  }
  if (mRandom) {
    mCurrentPayloadSize = rand() % 100 + 1; // between 1 and 100
  }
  mCurrent = new DataBlock();
  mCurrent->header.blockType = 0xba; // whatever
  mCurrent->header.headerSize = 0x60; // just the header base -> 96 bits
  mCurrent->header.dataSize = mCurrentPayloadSize * 8;
  char *buffer = new char[mCurrentPayloadSize];
  for (int i = 0; i < mCurrentPayloadSize; i++) {
    if (mRandom) {
      buffer[i] = (rand() % 26) + 'a';
    } else {
      buffer[i] = (i % 26) + 'a';
    }
  }
  mCurrent->data = buffer;
}

}
}