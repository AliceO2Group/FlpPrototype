///
/// \file   FairSampler.cxx
/// \author Barthelemy von Haller
///

#include <DataFormat/DataBlock.h>
#include "DataSampling/FairSampler.h"
#include "DataSampling/DsInfoLogger.h"

using namespace std;

namespace AliceO2 {
namespace DataSampling {

/// Standard Constructor
FairSampler::FairSampler() : mBlock(nullptr)
{
  FairMQChannel histoChannel;
  histoChannel.UpdateType("sub");
  histoChannel.UpdateMethod("connect");
  histoChannel.UpdateAddress("tcp://localhost:5558");
  histoChannel.UpdateSndBufSize(10000);
  histoChannel.UpdateRcvBufSize(10000);
  histoChannel.UpdateRateLogging(0);
  fChannels["data-in"].push_back(histoChannel);

  SetTransport("zeromq"); // or "nanomsg", etc

  ChangeState(INIT_DEVICE);
  WaitForEndOfState(INIT_DEVICE);
  ChangeState(INIT_TASK);
  WaitForEndOfState(INIT_TASK);
  ChangeState(RUN);

  // register a handler for data arriving on "data" channel
  OnData("data-in", &FairSampler::HandleData);
}

/// Destructor
FairSampler::~FairSampler()
{
  ChangeState(STOP);
  ChangeState(RESET_TASK);
  WaitForEndOfState(RESET_TASK);

  ChangeState(RESET_DEVICE);
  WaitForEndOfState(RESET_DEVICE);

  ChangeState(END);
}

vector<shared_ptr<DataBlockContainer>> *FairSampler::getData(int timeout)
{
  bool gotLock = mBlockMutex.try_lock_for(chrono::milliseconds(timeout));
  if (gotLock) {
    if (mBlock == nullptr) {
      mBlockMutex.unlock();
      return nullptr;
    }
    return mBlock;
  }
  return nullptr;
}

void FairSampler::Run()
{
}

void FairSampler::releaseData()
{
  if (mBlock) {
    // TODO delete elements?
    delete mBlock;
    mBlock = nullptr;
  }

  mBlockMutex.unlock();
}

bool FairSampler::HandleData(FairMQParts &parts, int /*index*/)
{
//  cout << "HandleData()" << endl;
//  cout << "   number of parts : " << parts.Size() << endl;

  // store the data
  if (!mBlockMutex.try_lock()) {
    return true;
  }

  // TODO delete elements?
  if (mBlock) {
    delete mBlock;
  }
  mBlock = new std::vector<std::shared_ptr<DataBlockContainer>>();

  DataBlock *block = new DataBlock();
  // TODO loop over parts
  block->header = *static_cast<DataBlockHeaderBase *>(parts.At(0)->GetData());
  block->data = static_cast<char *>(parts.At(1)->GetData()); // TODO should I copy ?
  std::shared_ptr<DataBlockContainer> containerPtr = std::make_shared<DataBlockContainer>(block);
  mBlock->push_back(containerPtr);
  mBlockMutex.unlock();
  // TODO we continuously receive data and trash it. This is stupid.
  // tODO we should rather get one sample, store it, wait only get another one when getData() is called.
  // TODO we should also have an expiration and update the data sample if it has been waiting for too long.

  // return true if want to be called again (otherwise go to IDLE state)
  return true;
}

}
}