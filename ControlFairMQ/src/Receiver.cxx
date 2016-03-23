/// \file    Receiver.cxx
/// \brief   Implementation of the Receiver class.
///
/// \author  Vasco Barroso, CERN

#include "ControlFairMQ/Receiver.h"
#include <iostream>

namespace AliceO2 {
namespace ControlFairMQ {
namespace Core {

using AliceO2::InfoLogger::InfoLogger;

Receiver::Receiver()
{
}

Receiver::~Receiver()
{
}

void Receiver::Run()
{
  while (CheckCurrentState(RUNNING)) {
	std::unique_ptr<FairMQMessage> msg(NewMessage());

	if (Receive(msg, "data") >= 0) {
      getLogger() << "Received message: \""
    	<< std::string(static_cast<char*>(msg->GetData()), msg->GetSize())
        << "\"" << InfoLogger::endm;
    }

    getLogger() << "loop" << InfoLogger::endm;
  }
}

InfoLogger& Receiver::getLogger()
{
  return mLogger;
}

} // namespace Core
} // namespace ControlFairMQ
} // namespace AliceO2
