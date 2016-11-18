///
/// \file 2-RequestNotification.cxx
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#include <iostream>
#include "GUI/RequestNotifier.h"

// Example callback funtion, received message is passed as ptree (user needs to be aware of message structure)
void exampleCallback(std::unique_ptr<boost::property_tree::ptree> message) {
  std::cout << "Name: " << message->get<std::string>("name") << "; Value: " << message->get<std::string>("value") << std::endl;
}

// Example that shows how to recieve requests and get different callbacks executed based on message names
int main() {
    // request socket url
    std::string url = "tcp://127.0.0.1:3001";

    // create request notifier object
    AliceO2::GUI::RequestNotifier *rep = new AliceO2::GUI::RequestNotifier(url);
    
    // bind message name; provided function will be called whenever message with given name arrives
    rep->bind(std::string("hello"), &exampleCallback);

    // do something else, and call processRequests periodically to obtain new messages from queue
    for(;;) {
      rep->processRequests();
      std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }   
}

