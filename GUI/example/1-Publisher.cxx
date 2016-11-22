///
/// \file 1-Publisher.cxx
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#include "GUI/Publisher.h"

#include <iostream>
#include <memory>
#include <thread>

/// Example that shows how to publish message
int main() {
  /// publisher socket url
  std::string url = "tcp://127.0.0.1:3000";

  // create GUI publisher object
  std::unique_ptr<AliceO2::GUI::Publisher> publisher = std::make_unique<AliceO2::GUI::Publisher>(url);
  
  // create a message (JSON-encoded)
  std::string message = "{\"name\" : \"test\"}";
  
  // publish values
  for (;;) {
    publisher->publish(message);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  }
}
