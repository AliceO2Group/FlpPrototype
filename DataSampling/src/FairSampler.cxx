///
/// \file   FairSampler.cxx
/// \author Barthelemy von Haller
///

#include "DataSampling/FairSampler.h"
#include <DataFormat/DataBlock.h>

using namespace std;

namespace AliceO2 {
namespace DataSampling {

/// Standard Constructor
FairSampler::FairSampler() : mDataSet(nullptr), mReceivingDataSet(false), mReceivingDataBlock(false)
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
  while (CheckCurrentState(RUNNING)) {

    if (!mBlockMutex.try_lock()) {
      continue;
    }
    // if we already have a dataset, let's not get another one.
    if (mDataSet) {
      mBlockMutex.unlock();
      continue;
    }

    do {
      std::unique_ptr<FairMQMessage> msg(NewMessage());
      if (Receive(msg, "data-in") > 0) {

        // what follows is a little state machine to know where we are in the stream of data coming in.
        // We expect : header (type BB) -> data -> ... <repeat> ... -> header (type FF ie. End Of Message)

        // start receiving a new data set
        if (!mReceivingDataSet) {
          mDataSet = make_shared<DataSet>();
          auto block = make_shared<SelfReleasingBlockContainer>();
          block->getData()->header = *static_cast<DataBlockHeaderBase *>(msg->GetData());
          mReceivingDataSet = true;
          mReceivingDataBlock = true;
          mDataSet->push_back(block);

        // in the middle of receiving a data set
        } else {

          // waiting for next block -> we expect a header
          if (!mReceivingDataBlock) {
            // receive a new header
            DataBlockHeaderBase header = *static_cast<DataBlockHeaderBase *>(msg->GetData());
            // determine whether it is an End Of Message
            if (header.blockType == H_EOM) {
              mReceivingDataSet = false;
              mReceivingDataBlock = false;
            } else if (header.blockType == H_BASE) {
              auto block = make_shared<SelfReleasingBlockContainer>();
              block->getData()->header = *static_cast<DataBlockHeaderBase *>(msg->GetData());
              mReceivingDataBlock = true;
              mDataSet->push_back(block);
            }

          // in the middle of receiving a block (we expect a data msg)
          } else {
            auto block = mDataSet->at(mDataSet->size() - 1);
            block->getData()->data = new char[msg->GetSize()];
            memcpy(block->getData()->data, static_cast<char *>(msg->GetData()),
                   msg->GetSize());
            mReceivingDataBlock = false;
          }
        }
      }
    }
    while (mReceivingDataSet);

    mBlockMutex.unlock();
  }
}

void FairSampler::releaseData()
{
  if (mDataSet) {
    mDataSet.reset();
  }

  mBlockMutex.unlock();
}

}
}