/// \file    Sender.cxx
/// \brief   Implementation of the Sender class.
///
/// \author  Vasco Barroso, CERN

#include "ControlFairMQ/Sender.h"

#include <boost/thread.hpp>
#include <memory>

#include <FairMQMessage.h>
#include <FairMQTransportFactory.h>
//#include <iostream>

namespace AliceO2 {
namespace ControlFairMQ {
namespace Core {

using AliceO2::InfoLogger::InfoLogger;

Sender::Sender()
  : mText("hello"), mRun(true)
{
}

Sender::~Sender()
{
}

bool Sender::ConditionalRun()
{
  std::this_thread::sleep_for(std::chrono::seconds(1));

  mLogger << "In Run : string is : " << mText << InfoLogger::endm;

  // create a copy of the data with new(), that will be deleted after the transfer is complete
  std::string* text = new std::string(mText);

  // create message object with a pointer to the data buffer,
  // its size,
  // custom deletion function (called when transfer is done),
  // and pointer to the object managing the data buffer
  FairMQMessagePtr msg(NewMessage(const_cast<char*>(text->c_str()),
                                  text->length(),
                                  [](void* /*data*/, void* object) { delete static_cast<std::string*>(object); },
                                  text));

  mLogger << "Sending \"" << mText << "\"" << InfoLogger::endm;

  // in case of error or transfer interruption, return false to go to IDLE state
  // successfull transfer will return number of bytes transfered (can be 0 if sending an empty message).
  if (Send(msg, "data") < 0)
  {
    return false;
  }

  return true;
}

} // namespace Core
} // namespace ControlFairMQ
} // namespace AliceO2
