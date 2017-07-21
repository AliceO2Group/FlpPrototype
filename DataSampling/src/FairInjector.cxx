///
/// \file    FairInjector.cxx
/// \author  Barthelemy von Haller
///

#include <DataSampling/FairInjector.h>
#include <DataFormat/DataSet.h>

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

//std::map<void *, DataBlockContainerReference> store;

void cleanupCallback(void *data, void *object)
{
//  if (data != nullptr) {
//    store.erase(data);
//  }
  if ((object!=nullptr)&&(data!=nullptr)) {
    DataBlockContainerReference *ptr=(DataBlockContainerReference *)object;
    delete ptr;
  }
}

int FairInjector::injectSamples(DataSetReference dataSetReference)
{
  FairMQParts parts;
  for (DataBlockContainerReference block : *dataSetReference) {

    DataBlockHeaderBase &header = block->getData()->header;
    char *data = block->getData()->data;

    // just to keep reference alive
//    store[data] = block;
    DataBlockContainerReference *ptr = new DataBlockContainerReference(block);

    FairMQMessagePtr msgHeader(NewMessage((void *) &header, (header.headerSize), cleanupCallback, (void *) nullptr));
    FairMQMessagePtr msgBody(NewMessage((void *) data, (header.dataSize), cleanupCallback, (void *) (ptr)));

    parts.AddPart(std::move(msgHeader));
    parts.AddPart(std::move(msgBody));
    // TODO do not use parts
//    Send(msgHeader, "data-out", 0);
//    Send(msgBody, "data-out", 0);
  }
  Send(parts, "data-out", 0);
}

void FairInjector::Run()
{

}

}
}