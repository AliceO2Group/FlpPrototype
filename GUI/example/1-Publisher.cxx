///
/// \file 1-Publisher.cxx
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#include <iostream>
#include <memory>
#include "GUI/Publisher.h"

/// Example that shows how to publish message
int main() {
  /// publisher socket url
  std::string url = "tcp://127.0.0.1:3000";

  // create GUI publisher object
  std::unique_ptr<AliceO2::GUI::Publisher> publisher = std::make_unique<AliceO2::GUI::Publisher>(url);
  
  // create a message (JSON-encoded)
  std::string message = "{\"name\" : \"test\"}";

  // send message
  publisher->publish(message);
}
