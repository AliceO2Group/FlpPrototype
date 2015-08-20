/**
 * O2FLPex.cxx
 *
 * @since 2013-04-23
 * @author D. Klein, A. Rybalchenko, M.Al-Turany
 */

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

void O2FLPex::Run()
{
	LOG(DEBUG) << "Using DataBlock.h";
	char *payload = (char*)malloc(sizeof(char) * fEventSize);
	std::fill(payload, payload + fEventSize, '*');
	LOG(INFO) << "Header size: " << sizeof(DataBlockHeaderBase) << "; Payload size: " << fEventSize;
        DataBlockHeaderBase header;
        header.blockType = 1;
        header.headerSize = 30;
        header.dataSize = fEventSize;
	DataBlock *block = new DataBlock();
	block->header = header;
	block->data = payload;
  while (GetCurrentState() == RUNNING) {
	FairMQMessage* msg = fTransportFactory->CreateMessage(header.headerSize + fEventSize);
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
