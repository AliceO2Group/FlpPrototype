///
/// \file RequestNotifier.h
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#ifndef ALICEO2_GUI_REPLIER_H
#define ALICEO2_GUI_REPLIER_H

#include <iostream>
#include <atomic>
#include <thread>
#include <queue>
#include <mutex>
#include <map>
#include "zmq.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace AliceO2
{
/// ALICE O2 Monitoring system
namespace GUI 
{
/// Receives requests (in new thread) from client side and stores them in queue.
/// User binds callback functions to messages names
/// User decides when to obtain messages from the queue; callbacks are executed in his context
class RequestNotifier
{
  private:
    /// ZeroMQ context
    zmq::context_t context;

    /// ZeroMQ publisher socket
    zmq::socket_t socket;

    /// RequestNotifier server socket URL
    std::string url;

    /// Map that stores callbacks
    std::map<std::string, std::function<void(std::unique_ptr<boost::property_tree::ptree>)>> callbacks;

    /// queue of received messages waiting to be taken by user
    std::queue<std::pair<std::string, std::unique_ptr<boost::property_tree::ptree>>> queue;

    /// flag that indicates whether queue is empty or not
    std::atomic<bool> queueNotEmpty;

    /// mutex that protects queue
    std::mutex queueMutex;

    /// dedicated thread where ZeroMQ is runnig at
    std::thread mZeromqThread;

    /// States whether zeromq thread is running
    std::atomic<bool> mThreadRunning;

    /// ZeroMQ thread loop
    void zeromqLoop();

    /// Receives data from ZeroMQ socket
    /// \return received data
    std::string receiveData();

    /// Sends acknowledgement - data received
    void notifyReceived();

    /// parses JSON-string into boost ptree
    /// \param json 	JSON-string received from ZeroMQ socket
    /// \return 	JSON-string parsed into ptree
    boost::property_tree::ptree parseJson(std::string json);
  public:
    /// binds ZeroMQ server socket
    /// \param url 	url of socket that will be bound
    RequestNotifier(std::string &url);

    /// binds a request name to a callback function;
    /// if a message received with given name, bound callback will be invoked
    /// \param name 	request name, that arrives in JSON-encoded message
    /// \param callback callback function that receives as parameter pointer to ptree (user needs to be aware of message structure)
    void bind(std::string name, std::function<void(std::unique_ptr<boost::property_tree::ptree>)> callback);

    /// disconnects sockets
    ~RequestNotifier();

    /// checks whether there are new messages, if so invokes apropriate callbacks
    void processRequests();
};

} // namespace Monitoring
} // namespace AliceO2

#endif // ALICEO2_GUI_REPLIER_H
