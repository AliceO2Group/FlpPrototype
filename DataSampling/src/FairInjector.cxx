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

void cleanupCallback(void *data, void *object)
{
  if ((object!=nullptr)&&(data!=nullptr)) {
    DataBlockContainerReference *ptr=(DataBlockContainerReference *)object;
    delete ptr;
  }
}

void cleanupHeader(void *data, void *object)
{
  if(data) {
    delete data;
  }
}

int FairInjector::injectSamples(DataSetReference dataSetReference)
{
  FairMQParts parts;
  for (DataBlockContainerReference block : *dataSetReference) {

    DataBlockHeaderBase &header = block->getData()->header;
    char *data = block->getData()->data;

    // just to keep reference alive
    DataBlockContainerReference *ptr = new DataBlockContainerReference(block);

    FairMQMessagePtr msgHeader(NewMessage((void *) &header, (header.headerSize), cleanupCallback, (void *) nullptr));
    FairMQMessagePtr msgBody(NewMessage((void *) data, (header.dataSize), cleanupCallback, (void *) (ptr)));

    // TODO do not use parts
    int ret1 = Send(msgHeader, "data-out", 0);
    int ret2 = Send(msgBody, "data-out", 0);
    if(ret1 < 0 || ret2 < 0) {
      cerr << "error sending data to DataSampling (header return : " << ret1 << " ; body return : " << ret2 << ")" << endl;
      break;
    }
  }
  DataBlockHeaderBase *endOfMessage = new DataBlockHeaderBase();
  endOfMessage->blockType = H_EOM;
  endOfMessage->headerSize = 0x60; // just the header eom -> 96 bits
  FairMQMessagePtr msgEom(NewMessage((void *) endOfMessage, (endOfMessage->headerSize), cleanupHeader));
  int ret = Send(msgEom, "data-out", 0);
  if(ret < 0) {
    cerr << "error sending data to DataSampling (return : " << ret << ")" << endl;
  }
}

void FairInjector::Run()
{

}

}
}