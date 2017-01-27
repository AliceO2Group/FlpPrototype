///
/// \file   InjectorDevice.cxx
/// \author Barthelemy von Haller
///

#include "DataSampling/InjectorDevice.h"
#include <FairMQTransportFactoryZMQ.h>
#include "DataSampling/DsInfoLogger.h"

using namespace std;

namespace AliceO2 {
namespace DataSampling {

InjectorDevice::InjectorDevice()
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

InjectorDevice::~InjectorDevice()
{
  ChangeState(STOP);
  ChangeState(RESET_TASK);
  WaitForEndOfState(RESET_TASK);

  ChangeState(RESET_DEVICE);
  WaitForEndOfState(RESET_DEVICE);

  ChangeState(END);
}

void InjectorDevice::sendData(std::string /*std::vector<DataBlockContainer*>*/ data)
{
  mData = new string(data);

  mAvailableData = true;

  DsInfoLogger::getInstance() << "sendData() : " << *mData << infologger::endm;

  Run();
}

void InjectorDevice::Run()
{
  if(!mAvailableData) {
//    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    return;
  }

  FairMQMessagePtr msg(NewMessage(const_cast<char *>(mData->c_str()),
                                  mData->length(),
                                  [](void * /*data*/, void *object) { delete static_cast<string *>(object);},
                                  mData));
  mAvailableData = false;
  DsInfoLogger::getInstance() << "Sending \"" << *mData << "\"" << infologger::endm;

  fChannels.at("data-out").at(0).Send(msg);
}

}
}