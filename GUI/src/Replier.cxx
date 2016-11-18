#include "GUI/Replier.h"

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
Replier::Replier(std::string &_url):
  context(1),
  socket(context, ZMQ_REP),
  url(_url)
{
  try {
    socket.bind(url.c_str());
  } catch (std::exception &e) {
    std::cout << "Could not bind socket " << url.c_str() << std::endl;
  }

  queueNotEmpty = false;
  zeromqThread = std::thread(&Replier::zeromqLoop, this);
}

Replier::~Replier()
{
  socket.unbind(url.c_str());
}

void Replier::bind(std::string name, std::function<void(std::unique_ptr<boost::property_tree::ptree>)> callback)
{
  callbacks.insert(std::make_pair(name, callback));
}

void Replier::processRequests()
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

void Replier::zeromqLoop()
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

boost::property_tree::ptree Replier::parseJson(std::string json) {
  boost::property_tree::ptree parsed;
  std::istringstream is(json);
  boost::property_tree::read_json(is, parsed);
  return parsed;
}

std::string Replier::receiveData() {
  zmq::message_t request;
  socket.recv(&request); /// loop blocks in here!!!
  return std::string(static_cast<char*>(request.data()), request.size());
}

void Replier::notifyReceived()
{
  std::string message = "{\"received\": true }";
  zmq::message_t reply (message.size());
  memcpy (reply.data(), message.data(), message.size());
  socket.send (reply);
}

}
}
