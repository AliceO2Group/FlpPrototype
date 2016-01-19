///
/// @file    main.cxx
/// @author  Barthelemy von Haller
///

#include "AlfaTest/Sender.h"
#include <FairMQProgOptions.h>
#include <FairMQParser.h>
#include <FairMQTransportFactoryZMQ.h>

using namespace std;

AliceO2::ProjectTemplate::AlfaTest::Sender sender;

void handler_interruption(int sig)
{
  std::cout << "interruption" << std::endl;
  sender.stop();
}

int main(int argc, char *argv[])
{
  FairMQProgOptions config;

  try {

    // Load configuration for sender
    config.UserParser<FairMQParser::JSON>("alfa.json", "sender");

    // Set the channels based on the configuration
    sender.fChannels = config.GetFairMQMap();

    // Get the proper transport factory
#ifdef NANOMSG
    FairMQTransportFactory* transportFactory = new FairMQTransportFactoryNN();
#else
    FairMQTransportFactory *transportFactory = new FairMQTransportFactoryZMQ();
#endif
    sender.SetTransport(transportFactory);

    //  sender.go();  // poller is null if we don't use the state machine of the device

    sender.ChangeState("INIT_DEVICE");
    sender.WaitForEndOfState("INIT_DEVICE");

    sender.ChangeState("INIT_TASK");
    sender.WaitForEndOfState("INIT_TASK");

    sender.ChangeState("RUN");
    sender.InteractiveStateLoop();
  }
  catch (std::exception &e) {
    cout << e.what();
    cout << "Command line options are the following: ";
    config.PrintHelp();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
