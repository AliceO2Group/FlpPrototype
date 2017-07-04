///
/// \file    FairInjector.cxx
/// \author  Barthelemy von Haller
///

#include <DataSampling/FairInjector.h>

using namespace std;

namespace AliceO2 {
namespace DataSampling {

FairInjector::FairInjector()
{
  FairMQChannel histoChannel;
  histoChannel.UpdateType("pub");
  histoChannel.UpdateMethod("bind");
  histoChannel.UpdateAddress("tcp://*:5558");
  histoChannel.UpdateSndBufSize(10000);
  histoChannel.UpdateRcvBufSize(10000);
  histoChannel.UpdateRateLogging(0);
  fChannels["data-out"].push_back(histoChannel);

  SetTransport("zeromq"); // or "nanomsg", etc

  ChangeState(INIT_DEVICE);
  WaitForEndOfState(INIT_DEVICE);

  ChangeState(INIT_TASK);
  WaitForEndOfState(INIT_TASK);

  ChangeState(FairMQStateMachine::Event::RUN);
}

FairInjector::~FairInjector()
{
  ChangeState(STOP);
  ChangeState(RESET_TASK);
  WaitForEndOfState(RESET_TASK);

  ChangeState(RESET_DEVICE);
  WaitForEndOfState(RESET_DEVICE);

  ChangeState(END);
}


int FairInjector::injectSamples(std::vector<std::shared_ptr<DataBlockContainer>> &dataBlocks)
{
  mDataBlocks = dataBlocks;
  mAvailableData = true;
  sendSamples();
  return 0;
}

void FairInjector::sendSamples()
{
  if (!mAvailableData) {
    return;
  }
  mAvailableData = false;

  FairMQParts parts;
  for (std::shared_ptr<DataBlockContainer> block : mDataBlocks) {
    DataBlockHeaderBase &header = block->getData()->header;
    char *data = block->getData()->data;

    FairMQMessagePtr msgHeader =
      NewMessage((void *) &header, (header.headerSize),
                 [](void * /*data*/, void *object) { /*todo*/ }/*, (void *) nullptr*/);
    FairMQMessagePtr msgBody(
      NewMessage((void *) data, (header.dataSize),
                 [](void * /*data*/, void *object) { /*todo*/ }/*, (void *) (bCopy)*/));

//    DsInfoLogger::getInstance() << "Sending block with id \"" << header.id << "\"" << infologger::endm;

    parts.AddPart(std::move(msgHeader));
    parts.AddPart(std::move(msgBody));
  }
  Send(parts, "data-out", 0);
}

void FairInjector::Run()
{

}

}
}