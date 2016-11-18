///
/// \file Publisher.cxx
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#include "GUI/Publisher.h"

#include <iostream>
#include <sstream>

/// ALICE O2
namespace AliceO2
{
/// ALICE O2 GUI
namespace GUI
{

Publisher::Publisher(std::string &_url):
  context(1),
  socket(context, ZMQ_PUB),
  url(_url)
{
  try {
    socket.bind(url.c_str());
  } catch (std::exception &e) {
    std::cout << "Could not bind socket " << url.c_str();
  }
  std::cout << "Socket bound";
}

Publisher::~Publisher()
{
  socket.unbind(url.c_str());
}

bool Publisher::publish(std::string &message)
{
  zmq::message_t zmqMessage(message.size());
  memcpy(zmqMessage.data(), message.data(), message.size());
  if (socket.send(zmqMessage)) {
    return true;
  }
  return false;
}

} // namespace GUI
} // namespace AliceO2
