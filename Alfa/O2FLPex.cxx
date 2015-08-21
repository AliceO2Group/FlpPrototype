/// \file O2EPNex.cxx
/// \brief FLP device using FairMQ and data format specified in DataBlock.h (instead of Content structure)
///
/// \author D. Klein, A. Rybalchenko, M.Al-Turany
/// \author modifed by Adam Wegrzynek (21.08.2015)

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
/// This function was competly rewritten by Adam, it uses structures from DataBlock.h instead of standard Content structure from O2FLPex.h
/// The function runs in seperate thread. At first:
///  - prepares test message that sizes equals to fEventSize (parameter passed when runing program)
///  - creates DataBlockHeaderBase and DataBlock
/// Then in infinite loop: 
///  - creates empty FairMQ message
///  - formats and copies to FairMQ buffer prepared DataBlock (uses snprintf instead of memcpy)
///  - sends message over the "dat-out channel"
void O2FLPex::Run()
{
  LOG(DEBUG) << "Using DataBlock.h";
  //allocating memory for test string
  char *payload = (char*)malloc(sizeof(char) * fEventSize);
  //filling the paylod with '*'
  std::fill(payload, payload + fEventSize, '*');
  LOG(INFO) << "Header size: " << sizeof(DataBlockHeaderBase) << "; Payload size: " << fEventSize;
  //creating header
  DataBlockHeaderBase header;
  header.blockType = 1;
  //this is maximum size of header after converting to char (smarter way needed, maybe covering to byte representation?)
  header.headerSize = 30;
  header.dataSize = fEventSize;
  DataBlock *block = new DataBlock();
  block->header = header;
  block->data = payload;

  while (GetCurrentState() == RUNNING) {
    //creating a message, size of header and data
    FairMQMessage* msg = fTransportFactory->CreateMessage(header.headerSize + fEventSize);
    //converting all data to char and copy to provided pointer
    snprintf ((char *) msg->GetData(), header.headerSize + fEventSize, "%010d%010d%010d%s", block->header.blockType, block->header.headerSize, block->header.dataSize, block->data);
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

void O2FLPex::SetProperty(const int key, const int value)
{
  switch (key) {
  case EventSize:
    fEventSize = value;
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
