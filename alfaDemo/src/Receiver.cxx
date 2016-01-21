///
/// @file    Receiver.cxx
/// @author  Barthelemy von Haller
///

#include "AlfaTest/Receiver.h"

#include <iostream>
#include <TH1.h>
#include <TCanvas.h>

namespace AliceO2 {
namespace ProjectTemplate {
namespace AlfaTest {

using namespace std;

Receiver::Receiver()
{
}



void Receiver::Run()
{
  // TODO : clean up memory

  while (CheckCurrentState(RUNNING)) {
    unique_ptr<FairMQMessage> msg(fTransportFactory->CreateMessage());

    if (fChannels.at("data-in").at(0).Receive(msg) > 0) {
      cout << "Received message: \""
      << string(static_cast<char *>(msg->GetData()), msg->GetSize())
      << "\"" << endl;
    }
    cout << "loop" << endl;

    unique_ptr<FairMQMessage> msg2(fTransportFactory->CreateMessage());


    if (fChannels.at("data-in").at(1).Receive(msg2) > 0) {
      cout << "Receiving an histo: \"" << endl;
      TestTMessage tm(msg2->GetData(), msg2->GetSize());
      TH1* histo = static_cast<TH1*>(tm.ReadObject(tm.GetClass()));
      TCanvas c;
      cout << "   name : " << histo->GetName() << endl;
      histo->Draw();
      c.SaveAs("test.png");
    }
  }
}

Receiver::~Receiver()
{
}

} // namespace AlfaTest
} // namespace ProjectTemplate
} // namespace AliceO2