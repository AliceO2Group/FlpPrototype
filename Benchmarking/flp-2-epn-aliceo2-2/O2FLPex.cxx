/// \file O2FLPex.cxx
/// \brief FLP device using FairMQ and data format specified in DataBlock.h
///
/// \author D. Klein, A. Rybalchenko, M.Al-Turany
/// \modifed by Adam Wegrzynek (21.08.2015)

#include <vector>
#include <time.h>       /* time */
#include <algorithm>
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
    char *current = payload;
    for (int i = 0; i < blocksNumber; i++) {
        /// FairMQ delets sent data; to prevent it function void FairMQMessageZMQ::CleanUp in file FairRoot/fairmq/zeromq/FairMQMessageZMQ.cxx was changed
        FairMQMessage* msg = fTransportFactory->CreateMessage((void*) current, fEventSize, NULL, NULL);
        fChannels["data-out"].at(0).Send(msg);
        delete msg;
        current += fEventSize;
    }
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
  blocksNumber = (int) memorySize / fEventSize;
  payload = (char*)malloc(sizeof(char) * memorySize);
  char *current = payload;
  switch (key) {
  case EventSize:   
    for (int i = 0; i < blocksNumber; i++) {
      //std::fill(current, current + fEventSize, '*');
      std::generate_n(current, fEventSize, std::rand);   
      current += fEventSize;
      
    }
    LOG(INFO) << "Memory init finished";
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
