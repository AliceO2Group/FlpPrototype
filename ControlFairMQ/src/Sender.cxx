/// \file    Sender.cxx
/// \brief   Implementation of the Sender class.
///
/// \author  Vasco Barroso, CERN

#include "ControlFairMQ/Sender.h"
#include <FairMQMessage.h>
#include <FairMQTransportFactory.h>
#include <iostream>
#include <memory>

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

void Sender::Run()
{
  while (CheckCurrentState(RUNNING)) {
    getLogger() << "In Run : string is : " << mText << InfoLogger::endm;

    std::string *text = new std::string(mText);

    std::unique_ptr<FairMQMessage> msg(
      fTransportFactory->CreateMessage(const_cast<char *>(text->c_str()), text->length(), NULL, text));

    getLogger() << "Sending \"" << mText << "\"" << InfoLogger::endm;

    fChannels.at("data-out").at(0).Send(msg);

    //boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
  }
}

InfoLogger& Sender::getLogger()
{
  return mLogger;
}

} // namespace AlfaTest
} // namespace ProjectTemplate
} // namespace AliceO2
