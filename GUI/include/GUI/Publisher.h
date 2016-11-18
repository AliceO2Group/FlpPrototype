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

class Publisher
{
  private:
    /// ZeroMQ context
    zmq::context_t context;

    /// ZeroMQ publisher socket
    zmq::socket_t socket;

    /// Socket URL
    std::string url;

    unsigned long getTimestampMilisecs();
  public:
    /// Creates context and socket
    /// Binds ZeroMQ socket to provided URL
    /// \param url 	URL used to bind socket
    Publisher(std::string &url);

    /// Unbind
    ~Publisher();

    /// Publishes value
    bool publish(std::string &name, std::string &value);
};
} // namespace Monitoring
} // namespace AliceO2

#endif // ALICEO2_GUI_PUBLISHER_H
