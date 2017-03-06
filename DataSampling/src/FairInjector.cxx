///
/// \file    FairInjector.cxx
/// \author  Barthelemy von Haller
///

#include <DataSampling/FairInjector.h>
//#include "DataSampling/InjectorDevice.h"
#include <FairMQTransportFactoryZMQ.h>
#include "DataSampling/DsInfoLogger.h"

namespace AliceO2 {
namespace DataSampling {

FairInjector::FairInjector() {
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

FairInjector::~FairInjector() {
//  delete mDevice;
  ChangeState(STOP);
  ChangeState(RESET_TASK);
  WaitForEndOfState(RESET_TASK);

  ChangeState(RESET_DEVICE);
  WaitForEndOfState(RESET_DEVICE);

  ChangeState(END);
}


int FairInjector::injectSamples(std::vector<std::shared_ptr<DataBlockContainer>> &dataBlocks)
{
  // copy the data and send it via FairMQ
//  mDevice->sendData(dataBlocks);
//  sendData(dataBlocks);

//void InjectorDevice::sendData(/*std::string*/ std::vector<std::shared_ptr<DataBlockContainer>> data)
//void FairInjector::sendData(std::vector<std::shared_ptr<DataBlockContainer>> data)
//{
  // todo add a mutex somewhere here to protect the data in datablocks --> is that a problem performance wise ?
  // todo should we have 2 members so that we can always write quickly if the other one is being read ?
  // todo actually it might not be necessary because we call Run() ourselvees. Is it ever called by fairroot ?
//  mData = new string(data);
  mDataBlocks = dataBlocks; // tODO verify with Sylvain whether we should copy the content

  mAvailableData = true;

  DsInfoLogger::getInstance() << "sendData() : \n" <<
                              " id: " << mDataBlocks[0]->getData()->header.id <<
                              infologger::endm;

  Run();

  return 0;

}

void FairInjector::Run()
{
  if (!mAvailableData) {
    return;
  }

  for (std::shared_ptr<DataBlockContainer> block : mDataBlocks) {
    DataBlockHeaderBase &header = block->getData()->header;
    char *data = block->getData()->data;
    FairMQParts parts;

    FairMQMessagePtr msgHeader=
      NewMessage((void *) &header,  (header.headerSize),
                 [](void * /*data*/, void *object) { /*todo*/ }/*, (void *) nullptr*/);
    FairMQMessagePtr msgBody(
      NewMessage((void *) data,  (header.dataSize),
                 [](void * /*data*/, void *object) { /*todo*/ }/*, (void *) (bCopy)*/));

    DsInfoLogger::getInstance() << "Sending block with id \"" << header.id << "\"" << infologger::endm;

    parts.AddPart(std::move(msgHeader));
    parts.AddPart(std::move(msgBody));
    Send(parts, "data-out", 0);
  }
}

}
}