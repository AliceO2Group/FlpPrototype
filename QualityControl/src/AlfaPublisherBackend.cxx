///
/// \file   AlfaPublisherBackend.cxx
/// \author Barthelemy von Haller
///

#include "QualityControl/AlfaPublisherBackend.h"
#include "FairMQTransportFactoryZMQ.h"
#include "TMessage.h"
#include "TH1F.h"
#include "FairMQProgOptions.h"
#include "FairMQParser.h"

using namespace std;

namespace AliceO2 {
namespace QualityControl {
namespace Core {

AlfaPublisherBackend::AlfaPublisherBackend()
  : mText("asdf")
{

  FairMQProgOptions config;

  // load communication layout and properties
  std::string filename = "alfa.json";
  std::string id = "sender";
  config.UserParser<FairMQParser::JSON>(filename, id);
  fChannels = config.GetFairMQMap();

  // Get the transport layer
#ifdef NANOMSG
  FairMQTransportFactory* transportFactory = new FairMQTransportFactoryNN();
#else
  FairMQTransportFactory *transportFactory = new FairMQTransportFactoryZMQ();
#endif
  SetTransport(transportFactory);

}

AlfaPublisherBackend::~AlfaPublisherBackend()
{
}

// helper function to clean up the object holding the data after it is transported.
void AlfaPublisherBackend::CustomCleanup(void *data, void *object)
{
  delete static_cast<TMessage*>(object); // Does it delete the TH1F ?
}

void AlfaPublisherBackend::publish(MonitorObject *mo)
{
  // from Mohammad
  TH1F * th1 = new TH1F("test", "test", 100, 0, 99);
  TMessage *message = new TMessage(kMESS_OBJECT);
  message->WriteObject(th1);
  FairMQMessage *msg = fTransportFactory->CreateMessage(message->Buffer(), message->BufferSize(), CustomCleanup,
                                                        message);
  fChannels.at("data-out").at(0).Send(msg);
}

void AlfaPublisherBackend::Init()
{
  cout << "Init()" << endl;
}

void AlfaPublisherBackend::Run()
{
  cout << "Run()" << endl;
}

} // namespace Core
} // namespace QualityControl
} // namespace AliceO2

