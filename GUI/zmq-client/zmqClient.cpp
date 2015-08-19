//
//  Hello World client in C++
//  Connects REQ socket to tcp://localhost:5555
//  Sends "Hello" to server, expects "World" back
//
#include "zmq.hpp"
#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;

class ZMQClient {
 private:
        zmq::context_t contextSub;
        zmq::socket_t socketSub;
        
        zmq::context_t contextReq;
        zmq::socket_t socketReq;
    public:
        ///
        /// Binds two sockets: 
        ///  (1) PUB - publishes values to web socket server
        ///  (2) REP - receives values to be pushed by web socket server and zmq client
        ///
        ZMQClient();
        ///
        /// Response socket handler - sends a replay to requestor and pushes the sent value via PUB oskcet
        ///
        string sendRequest();

        ////
        /// Publishes the value to all connected subsribers (web-socket server for now)
        ///
        string reciveSubscribed();
};
ZMQClient::ZMQClient():
contextSub(1),
socketSub(contextSub, ZMQ_SUB),
contextReq(1),
socketReq(contextReq, ZMQ_REQ) {   
    /* binds server socket */
    socketSub.connect("tcp://pcaldvmh05.cern.ch:5444/");
    socketSub.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    cout << "Socket ZMQ_SUB connected to " << "tcp://pcaldvmh05.cern.ch:5444/" << endl;
    
    //socketReq.connect("tcp://pcaldvmh05.cern.ch:6444/");
    //cout << "Socket ZMQ_REQ connected to " << "tcp://pcaldvmh05.cern.ch:6444/" << endl;
}
string ZMQClient::reciveSubscribed() {
    zmq::message_t update;
    socketSub.recv(&update);
    string value = std::string(static_cast<char*>(update.data()), update.size());
    return value;
}
string ZMQClient::sendRequest() {
    //  Read line and send it via zmq socket
        string input;
        getline(cin, input);

        int size = input.size() + 1;
        zmq::message_t request (size);
        snprintf ((char *) request.data(), size, "%s", input.c_str());
        cout << "Sending request: " << input << endl;
        socketReq.send (request);

        //  Get the reply.
        zmq::message_t reply;
        socketReq.recv (&reply);
        string value = std::string(static_cast<char*>(reply.data()), reply.size());
        return value;
}
int main () {   
    ZMQClient client;
    while(1) {
        cout << "SUB: " << client.reciveSubscribed() << endl;
        //cout << "REQ: " << client.sendRequest() << endl;
        usleep(1000);
    }
}