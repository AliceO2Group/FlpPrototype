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
  // register a handler for data arriving on "data" channel
  OnData("data", &Receiver::HandleData);
}

Receiver::~Receiver()
{
}

// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool Receiver::HandleData(FairMQMessagePtr &msg, int /*index*/)
{
  mLogger << "Received message: \""
              << std::string(static_cast<char *>(msg->GetData()), msg->GetSize())
              << "\"" << InfoLogger::endm;
}

} // namespace Core
} // namespace ControlFairMQ
} // namespace AliceO2
