///
/// \file Publisher.h
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#ifndef ALICEO2_GUI_PUBLISHER_H
#define ALICEO2_GUI_PUBLISHER_H

#include <iostream>
#include "zmq.hpp"

namespace AliceO2
{
/// ALICE O2 Monitoring system
namespace GUI
{

/// Allows to publish generic messages that are passed through WebSocket server and then delivered to given widget on client side
class Publisher
{
  private:
    /// ZeroMQ context
    zmq::context_t context;

    /// ZeroMQ publisher socket
    zmq::socket_t socket;

    /// Publishr server socket URL
    std::string url;
  public:
    /// Creates context and socket
    /// Binds ZeroMQ socket to provided URL
    /// \param url 	URL used to bind socket
    Publisher(std::string &url);

    /// Unbinds server sockets
    ~Publisher();

    /// Sends message to WebSocket server
    bool publish(std::string &message);
};
} // namespace Monitoring
} // namespace AliceO2

#endif // ALICEO2_GUI_PUBLISHER_H
