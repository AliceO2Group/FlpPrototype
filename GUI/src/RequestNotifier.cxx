#include "GUI/RequestNotifier.h"
#include "GuiInfoLogger.h"

#include <iostream>
#include <sstream>
#include <chrono>

//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/json_parser.hpp>

/// ALICE O2
namespace AliceO2
{
/// ALICE O2 GUI
namespace GUI 
{
RequestNotifier::RequestNotifier(std::string &_url):
  context(1),
  socket(context, ZMQ_REP),
  url(_url)
{
  try {
    socket.bind(url.c_str());
  } catch (std::exception &e) {
    GuinInfoLogger::GetInstance() << "GUI RequestNotifier : Cannot bind socket: " << url << AliceO2::InfoLogger::InfoLogger::endm;
  }
  GuinInfoLogger::GetInstance() << "GUI RequestNotifier : Socket bound: " << url << AliceO2::InfoLogger::InfoLogger::endm;

  queueNotEmpty = false;
  mThreadRunning = true;
  mZeromqThread = std::thread(&RequestNotifier::zeromqLoop, this);
}

RequestNotifier::~RequestNotifier()
{
  mThreadRunning = false;
  socket.unbind(url.c_str());
  if (mZeromqThread.joinable()) {
    mZeromqThread.join();
  }
}

void RequestNotifier::bind(std::string name, std::function<void(std::unique_ptr<boost::property_tree::ptree>)> callback)
{
  callbacks.insert(std::make_pair(name, callback));
}

void RequestNotifier::processRequests()
{
  if (queueNotEmpty) {
    std::lock_guard<std::mutex> lock(queueMutex);
    for (;;) {
       if (queue.empty()) {
         break;
       }
       std::string key = std::get<0>(queue.front());
       auto search = callbacks.find(key);
       if(search != callbacks.end()) {
         search->second(std::move(std::get<1>(queue.front())));
       }
       queue.pop();
    }
  }
  queueNotEmpty = false;
}

void RequestNotifier::zeromqLoop()
{
  for (;;) {
    std::string received = receiveData();
    std::unique_ptr<boost::property_tree::ptree> parsed = std::make_unique<boost::property_tree::ptree>(parseJson(received));
    notifyReceived();
    {
      std::lock_guard<std::mutex> lock(queueMutex);
      queue.push(std::make_pair(parsed->get<std::string>("name"), std::move(parsed)));
      queueNotEmpty = true;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}

boost::property_tree::ptree RequestNotifier::parseJson(std::string json) {
  boost::property_tree::ptree parsed;
  std::istringstream is(json);
  boost::property_tree::read_json(is, parsed);
  return parsed;
}

std::string RequestNotifier::receiveData() {
  zmq::message_t request;
  socket.recv(&request); /// loop blocks in here!!!
  return std::string(static_cast<char*>(request.data()), request.size());
}

void RequestNotifier::notifyReceived()
{
  std::string message = "{\"received\": true }";
  zmq::message_t reply (message.size());
  memcpy (reply.data(), message.data(), message.size());
  socket.send (reply);
}

}
}
