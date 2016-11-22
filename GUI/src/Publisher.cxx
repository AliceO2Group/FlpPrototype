///
/// \file Publisher.cxx
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#include "GUI/Publisher.h"
#include "GuiInfoLogger.h"

#include <iostream>
#include <sstream>

#include <boost/property_tree/json_parser.hpp>

/// ALICE O2
namespace AliceO2
{
/// ALICE O2 GUI
namespace GUI
{

Publisher::Publisher(const std::string &_url):
  context(1),
  socket(context, ZMQ_PUB),
  url(_url)
{
  try {
    socket.bind(url.c_str());
  } catch (std::exception &e) {
    GuiInfoLogger::GetInstance() << "GUI Publisher : Cannot bind socket: " << url << AliceO2::InfoLogger::InfoLogger::endm;
  }
  GuiInfoLogger::GetInstance() << "GUI Publisher : Socket bound: " << url << AliceO2::InfoLogger::InfoLogger::endm;
}

Publisher::~Publisher()
{
  socket.unbind(url.c_str());
}

bool Publisher::publish(const std::string &message)
{
  zmq::message_t zmqMessage(message.size());
  memcpy(zmqMessage.data(), message.data(), message.size());
  if (socket.send(zmqMessage)) {
    return true;
  }
  return false;
}

bool Publisher::publish(const boost::property_tree::ptree &message)
{
  std::ostringstream buffer;
  boost::property_tree::write_json(buffer, message, false);
  return publish(buffer.str());
}

} // namespace GUI
} // namespace AliceO2
