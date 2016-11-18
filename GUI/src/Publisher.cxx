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

bool Publisher::publish(std::string &name, std::string &value)
{
  std::ostringstream ss;
  ss << "{\"name\":\"" << name << "\","
     << "\"value\":\"" << value << "\","
     << "\"timestamp\":" << getTimestampMilisecs() << ",";
  std::string jsonMessage = ss.str();
  zmq::message_t message(jsonMessage.size());
  memcpy(message.data(), jsonMessage.data(), jsonMessage.size());
  if (socket.send(message)) {
    return true;
  }
  return false;
}

unsigned long Publisher::getTimestampMilisecs()
{
  auto now = std::chrono::system_clock::now();
  return std::chrono::duration_cast <std::chrono::nanoseconds>(
    now.time_since_epoch()
  ).count();
}

} // namespace GUI
} // namespace AliceO2
