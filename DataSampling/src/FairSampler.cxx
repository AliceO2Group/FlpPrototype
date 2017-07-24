///
/// \file   FairSampler.cxx
/// \author Barthelemy von Haller
///

#include <DataFormat/DataBlock.h>
#include "DataSampling/FairSampler.h"

using namespace std;

namespace AliceO2 {
namespace DataSampling {

/// Standard Constructor
FairSampler::FairSampler() : mDataSet(nullptr)
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

DataSetReference FairSampler::getData(int timeout)
{
  bool gotLock = mBlockMutex.try_lock_for(chrono::milliseconds(timeout));
  if (gotLock) {
    if (mDataSet == nullptr) {
      mBlockMutex.unlock();
      return nullptr;
    }
    return mDataSet;
  }
  return nullptr;
}

void FairSampler::Run()
{
}

void FairSampler::releaseData()
{
  if (mDataSet) {
    mDataSet.reset();
  }

  mBlockMutex.unlock();
}

bool FairSampler::HandleData(FairMQParts &parts, int /*index*/)
//bool FairSampler::HandleData(FairMQMessagePtr &msg, int /*index*/)
{
  if (!mBlockMutex.try_lock()) {
    return true;
  }

  mDataSet = make_shared<DataSet>();
  for (int subblock = 0; subblock < parts.Size(); subblock = subblock + 2) {
    auto block = make_shared<SelfReleasingBlockContainer>();

    block->getData()->header = *static_cast<DataBlockHeaderBase *>(parts.At(subblock)->GetData());
    block->getData()->data = new char[parts.At(subblock + 1)->GetSize()];
    memcpy(block->getData()->data, static_cast<char *>(parts.At(subblock + 1)->GetData()), parts.At(subblock + 1)->GetSize());
//    block->getData()->data = static_cast<char *>(parts.At(subblock + 1)->GetData());
    mDataSet->push_back(block);
  }
  mBlockMutex.unlock();
  // TODO we continuously receive data and trash it. This is stupid.
  // tODO we should rather get one sample, store it, wait only get another one when getData() is called.
  // TODO we should also have an expiration and update the data sample if it has been waiting for too long.

  // return true if want to be called again (otherwise go to IDLE state)
  return true;
}

}
}