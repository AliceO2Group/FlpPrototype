/// \file O2FLPex.cxx
/// \brief FLP device using FairMQ and data format specified in DataBlock.h
///
/// \author D. Klein, A. Rybalchenko, M.Al-Turany
/// \modifed by Adam Wegrzynek (21.08.2015)

#include <vector>
#include <time.h>       /* time */

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "FairMQLogger.h"
#include "O2FLPex.h"

using namespace std;

O2FLPex::O2FLPex() :
  fEventSize(10000)
{
}

O2FLPex::~O2FLPex()
{

}

void O2FLPex::Init()
{
  FairMQDevice::Init();
}
/// Structure definied in DataBlock.h is used instead of Content from O2FLPex.h
/// Infinite loop: 
///  - allocates contiguous memory for DataBlock 
///  - sets the headers in DataBlockHeaderBase
///  - memcopies the data
///  - creates new FairMQ message (passes DataBlock pointer in the constructor)
///  - sends message over the "dat-out channel"
void O2FLPex::Run()
{
  while (GetCurrentState() == RUNNING) {
    //calculates needed memory size
    int memorySize = sizeof(DataBlockHeaderBase) + sizeof(char) * fEventSize;
    //contiguous memory allocation for DataBlock
    uint32_t *buffer = (uint32_t*)malloc(memorySize);
  
    DataBlock *frame = new DataBlock();
    //assigns header pointer
    frame->header = (DataBlockHeaderBase*) buffer;
    frame->header->blockType = 1;
    frame->header->headerSize = sizeof(DataBlockHeaderBase);
    frame->header->dataSize = fEventSize;
    //assigns data pointer
    frame->data = (char*)(buffer + 3);
    //copies the payload (= copying the data from physmem to Linux memory)
    memcpy(frame->data, payload, sizeof(char) * fEventSize);

  /* LOG(INFO) << "block type: " << frame->header->blockType << "; header size: "
     << frame->header->headerSize << "; data size: " << frame->header->dataSize << "; data: "
     << frame->data << endl; */

    //create a new message, passes pointer to the buffer and its size
    FairMQMessage* msg = fTransportFactory->CreateMessage((void*) buffer, memorySize, NULL, NULL);
    fChannels["data-out"].at(0).Send(msg);
    delete msg;
  }
}
void O2FLPex::SetProperty(const int key, const string& value)
{
  switch (key) {
  default:
    FairMQDevice::SetProperty(key, value);
    break;
  }
}

string O2FLPex::GetProperty(const int key, const string& default_/*= ""*/)
{
  switch (key) {
  default:
    return FairMQDevice::GetProperty(key, default_);
  }
}
/// Prepares test message that sizes equals to fEventSize (parameter passed when runing program) - it simulates data provided by RORC
void O2FLPex::SetProperty(const int key, const int value)
{
  switch (key) {
  case EventSize:
    fEventSize = value;
    LOG(INFO) << "Initialized static payload: " << fEventSize << " bytes";;
    payload = (char*)malloc(sizeof(char) * fEventSize);
    fill(payload, payload + fEventSize, '*');
    break;
  default:
    FairMQDevice::SetProperty(key, value);
    break;
  }
}

int O2FLPex::GetProperty(const int key, const int default_/*= 0*/)
{
  switch (key) {
  case EventSize:
    return fEventSize;
  default:
    return FairMQDevice::GetProperty(key, default_);
  }
}
