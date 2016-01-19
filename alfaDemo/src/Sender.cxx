///
/// @file    Sender.cxx
/// @author  Barthelemy von Haller
///

#include <memory>
#include "AlfaTest/Sender.h"

#include <iostream>

#include <FairMQMessage.h>
#include <FairMQTransportFactory.h>
#include <TMessage.h>

#include <TH1F.h>

using namespace std;

namespace AliceO2 {
namespace ProjectTemplate {
namespace AlfaTest {

Sender::Sender()
  : mText("hello"), mRun(true)
{
  mHisto = new TH1F("asdf", "asdf", 100, 0, 99);
}

Sender::~Sender()
{
}

void Sender::CustomCleanup(void *data, void *object)
{
  delete (std::string *) object;
}

void Sender::CustomCleanupTMessage(void *data, void *object)
{
  delete (TMessage *) object;
}

void Sender::myStop()
{
  mRun = false;
}

void Sender::go() // attempt not to use the state machines
{
  while (mRun) {
    std::cout << "In Run : string is : " << mText << std::endl;

    std::string *text = new std::string(mText);

    unique_ptr<FairMQMessage> msg(
      fTransportFactory->CreateMessage(const_cast<char *>(text->c_str()), text->length(), CustomCleanup, text));

    std::cout << "Sending \"" << mText << "\"" << std::endl;

    fChannels.at("data-out-string").at(0).Send(msg);

    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
  }
}

void Sender::Run()
{
  // TODO : clean up memory

  while (CheckCurrentState(RUNNING)) {
    std::cout << "In Run : string is : " << mText << std::endl;

    std::string *text = new std::string(mText);

    unique_ptr<FairMQMessage> msg(
      fTransportFactory->CreateMessage(const_cast<char *>(text->c_str()), text->length(), CustomCleanup, text));

    std::cout << "Sending \"" << mText << "\"" << std::endl;

    fChannels.at("data-out").at(0).Send(msg);

    cout << "and now sending a histo" << endl;

    TMessage* message = new TMessage(kMESS_OBJECT);
    message->WriteObject(mHisto);

    unique_ptr<FairMQMessage> msg2(fTransportFactory->CreateMessage (message->Buffer(), message->BufferSize(), CustomCleanupTMessage, message));

    std::cout << "Sending \"" << mHisto->GetName() << "\"" << std::endl;
    mHisto->FillRandom("gaus", 10);

    fChannels.at("data-out").at(1).Send(msg2);

    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
  }
}

} // namespace AlfaTest
} // namespace ProjectTemplate
} // namespace AliceO2