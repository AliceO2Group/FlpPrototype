#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include "zmq.hpp"
#include <functional> //for std::function
#include <algorithm>

using namespace std;

class ZMQServer {
    private:
        zmq::context_t contextPub;
        zmq::context_t contextRsp;
        zmq::socket_t socketPub;
        zmq::socket_t socketRsp;

    public:
        ///
        /// Binds two sockets: 
        ///  (1) PUB - publishes values to web socket server
        ///  (2) REP - receives values to be pushed by web socket server and zmq client
        ///
        ZMQServer();
        ///
        /// Response socket handler - sends a replay to requestor and pushes the sent value via PUB oskcet
        ///
        void response();
        string random_string(size_t length);
        ////
        /// Publishes the value to all connected subsribers (web-socket server for now)
        ///
        void publishValue(string value);
};
ZMQServer::ZMQServer():
contextPub(1),
contextRsp(1),
socketPub(contextPub, ZMQ_PUB),
socketRsp(contextRsp, ZMQ_REP) {   
    /* binds server socket */
    socketPub.bind ("tcp://*:5444/");
    cout << "Socket ZMQ_PUB bound " << "tcp://*:5444/" << endl;
    
    socketRsp.bind ("tcp://*:6444/");
    cout << "Socket ZMQ_REP bound " << "tcp://*:6444/" << endl;
}
void ZMQServer::response() {
    zmq::message_t request;
    if ((socketRsp.recv(&request, ZMQ_DONTWAIT) == true) &&  (request.size() > 0)) {
        string value = std::string(static_cast<char*>(request.data()), request.size());
        cout << "REQ Received: " << value << endl;
        string tosend = "ZMQServer(" + value + ")";
        //publishValue(tosend);
        tosend = "Value " + tosend + " has been pushed to Web-Clients";
        int size = tosend.size() + 1;
        zmq::message_t reply (size);
        memcpy ((void *) reply.data (), tosend.c_str(), size);
        /* sends messange */
        socketRsp.send (reply);
    }
}
string ZMQServer::random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}
void ZMQServer::publishValue(string tosend) {
    cout << "PUB Sent: " << tosend << endl;
    int size = tosend.size() + 1;
    zmq::message_t message(size);
    memcpy ((void *) message.data (), tosend.c_str(), size);
    socketPub.send(message);             
}
int main(int argc, char* argv[]) {
    ZMQServer server;
    while(1) {
        server.publishValue(server.random_string(20));
        usleep(1000000);
    }
}