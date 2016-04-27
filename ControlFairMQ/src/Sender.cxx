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

void Sender::CustomCleanup(void *data, void *object)
{
    delete (std::string*)object;
}

void Sender::Run()
{
  while (CheckCurrentState(RUNNING)) {
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

	  getLogger() << "In Run : string is : " << mText << InfoLogger::endm;

    std::string *text = new std::string(mText);
    std::unique_ptr<FairMQMessage> msg(NewMessage(const_cast<char*>(text->c_str()), text->length(), CustomCleanup, text));

    getLogger() << "Sending \"" << mText << "\"" << InfoLogger::endm;

    Send(msg, "data");
  }
}

InfoLogger& Sender::getLogger()
{
  return mLogger;
}

} // namespace Core
} // namespace ControlFairMQ
} // namespace AliceO2
