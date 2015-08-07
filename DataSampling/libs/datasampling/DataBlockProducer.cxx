///
/// \file    DataBlockProducer.cxx
/// \author  Barthelemy von Haller
///

#include <DataBlock.h>
#include "DataBlockProducer.h"
#include <fstream>

namespace AliceO2 {
namespace DataSampling {

DataBlockProducer::DataBlockProducer(uint32_t payloadSize) : mCurrent(0), mPayloadSize(payloadSize)
{
  regenerate();
}

DataBlockProducer::~DataBlockProducer()
{
  delete mCurrent->data;
  delete mCurrent;
}

DataBlock DataBlockProducer::get() const
{
  return *mCurrent;
}

/// Note : the file is opened and closed each time.
void DataBlockProducer::saveToFile(std::string pathToFile, bool append) const
{
  std::ios_base::openmode extra_mode = append ? std::ios::app : std::ios::trunc;
  std::fstream file(pathToFile, std::ios::out | std::ios::binary | extra_mode);
  file.write((char *) &mCurrent->header, sizeof(DataBlockHeaderBase));
  file.write(mCurrent->data, mPayloadSize);
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
  mCurrent = new DataBlock();
  mCurrent->header.blockType = 0xba; // whatever
  mCurrent->header.headerSize = 0x60; // just the header base -> 96 bits
  mCurrent->header.dataSize = mPayloadSize * 8;
  char *buffer = new char[mPayloadSize];
  for(int i = 0 ; i < mPayloadSize ; i++ ) {
    buffer[i] = (i%26)+'a';
  }
  mCurrent->data = buffer;
}

}
}