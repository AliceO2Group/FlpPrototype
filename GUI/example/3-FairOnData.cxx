#include "3-FairOnData.h"

#include <iostream>
#include <string>
#include "FairMQLogger.h"

/// register a handler for data arriving on "notification" channel
FairOnData::FairOnData()
{
    OnData("notification", &FairOnData::HandleNotification);
}

/// handler is called whenever a message arrives on "notification", with a reference to the message and a sub-channel index (here 0)
bool FairOnData::HandleNotification(FairMQMessagePtr& msg, int index)
{
    LOG(INFO) << "Notification received: \"" << std::string(static_cast<char*>(msg->GetData()), msg->GetSize()) << "\"";

    // create response
    std::string *response = new std::string("{ \"executed\": 1 }");

    // create message object with a pointer to the data buffer
    // custom deletion function (called when transfer is done)
    // and pointer to the object managing the data buffer
    FairMQMessagePtr responseMessage(NewMessage(const_cast<char*>(response->c_str()),
                                     response->length(),
                                     [](void* /*data*/, void* object) { delete static_cast<std::string*>(object); },
                                     response));

    if (Send(responseMessage, "notification") < 0)
    {
        return false;
    }
    // return true if want to be called again (otherwise go to IDLE state)
    return true;
}

FairOnData::~FairOnData()
{
}

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& /*options*/)
{
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/)
{
  return new FairOnData();
}

//int main(int argc, char** argv)
//{
//    try
//    {
//        FairMQProgOptions config;
//        config.ParseAll(argc, argv);
//
//        FairOnData notifier;
//        notifier.CatchSignals();
//        notifier.SetConfig(config);
//
//        notifier.ChangeState("INIT_DEVICE");
//        notifier.WaitForEndOfState("INIT_DEVICE");
//
//        notifier.ChangeState("INIT_TASK");
//        notifier.WaitForEndOfState("INIT_TASK");
//
//        notifier.ChangeState("RUN");
//        notifier.InteractiveStateLoop();
//    }
//    catch (std::exception& e)
//    {
//        LOG(ERROR) << "Unhandled Exception reached the top of main: "
//                   << e.what() << ", application will now exit";
//        return 1;
//    }
//
//    return -1;
//}
