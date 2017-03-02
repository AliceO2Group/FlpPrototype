///
/// \file   FairSampler.cxx
/// \author Barthelemy von Haller
///

#include "DataSampling/FairSampler.h"
#include "DataSampling/DsInfoLogger.h"

using namespace std;

namespace AliceO2 {
namespace DataSampling {

/// Standard Constructor
FairSampler::FairSampler()
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

  WaitForEndOfState(RUN);
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

DataBlock *FairSampler::getData(int timeout)
{
  return nullptr;

}

void FairSampler::Run()
{

  while (CheckCurrentState(RUNNING)) {
    unique_ptr<FairMQMessage> message(fTransportFactory->CreateMessage());

    while (fChannels.at("data-in").at(0).ReceiveAsync(message) > 0) {

DsInfoLogger::getInstance() << "data !" << infologger::endm;
//      TestTMessage tm(message->GetData(), message->GetSize());
//      TObject *tobj = tm.ReadObject(tm.GetClass());
//      if (tobj) {
//        // TODO once the bug in ROOt that removes spaces in strings passed in signal slot is fixed we can use the normal name.
//        string objectName = tobj->GetName();
//        boost::erase_all(objectName, " ");
//        if (mCache.count(objectName) && mCache[objectName]) {
//          delete mCache[objectName];
//        }
//        mCache[objectName] = tobj;
//        mFrame->updateList(objectName);
//      } else {
//        cerr << "not a tobject !" << endl;
//      }
    }
//    this_thread::sleep_for(chrono::milliseconds(1000));
  }



}

void FairSampler::releaseData()
{

}

bool FairSampler::HandleData(FairMQMessagePtr & msg, int)
{
  LOG(INFO) << "Received: \"" << *static_cast<string*>(msg->GetData()) << "\"";

  // return true if want to be called again (otherwise go to IDLE state)
  return true;
}

}
}