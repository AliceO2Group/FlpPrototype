#include "3-FairOnData.h"

#include <iostream>
#include "FairMQLogger.h"
#include "FairMQProgOptions.h"

FairOnData::FairOnData()
{
    // register a handler for data arriving on "data" channel
    OnData("notification", &FairOnData::HandleNotification);
}

// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool FairOnData::HandleNotification(FairMQMessagePtr& msg, int index)
{
    LOG(INFO) << "Notification received: \"" << std::string(static_cast<char*>(msg->GetData()), msg->GetSize()) << "\"";

    // return true if want to be called again (otherwise go to IDLE state)
    return true;
}

FairOnData::~FairOnData()
{
}

int main(int argc, char** argv)
{
    try
    {
        FairMQProgOptions config;
        config.ParseAll(argc, argv);

        FairOnData notifier;
        notifier.CatchSignals();
        notifier.SetConfig(config);

        notifier.ChangeState("INIT_DEVICE");
        notifier.WaitForEndOfState("INIT_DEVICE");

        notifier.ChangeState("INIT_TASK");
        notifier.WaitForEndOfState("INIT_TASK");

        notifier.ChangeState("RUN");
        notifier.InteractiveStateLoop();
    }
    catch (std::exception& e)
    {
        LOG(ERROR) << "Unhandled Exception reached the top of main: "
                   << e.what() << ", application will now exit";
        return 1;
    }

    return -1;
}
