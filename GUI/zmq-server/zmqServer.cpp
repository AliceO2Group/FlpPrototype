#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include "zmq.hpp"

using namespace std;

class ZMQServer {
    private:
        zmq::context_t contextPub;
        zmq::context_t contextRsp;
        zmq::socket_t socketPub;
        zmq::socket_t socketRsp;

    public:
        ZMQServer();
        void response();
        void publishValue(string value);
};

ZMQServer::ZMQServer():
contextPub(1),
contextRsp(1),
socketPub(contextPub, ZMQ_PUB),
socketRsp(contextRsp, ZMQ_REP) {   
    /* binds server socket */
    socketPub.bind ("tcp://127.0.0.1:5444/");
    cout << "Socket bound " << "tcp://127.0.0.1:5444/" << endl;
    socketRsp.bind ("tcp://127.0.0.1:6444/");
    cout << "Socket bound " << "tcp://127.0.0.1:6444/" << endl;
}
void ZMQServer::response() {
    zmq::message_t request;
    if ((socketRsp.recv(&request, ZMQ_DONTWAIT) == true) &&  (request.size() > 0)) {
        string value = std::string(static_cast<char*>(request.data()), request.size());
        string tosend = "ZMQServer(" + value + ")";
        publishValue(tosend);
        tosend = "Value " + tosend + " has been pushed to Web-Clients";
        int size = tosend.size() + 1;
        zmq::message_t reply (size);
        memcpy ((void *) reply.data (), tosend.c_str(), size);
        /* sends messange */
        socketRsp.send (reply);
    }
}
void ZMQServer::publishValue(string tosend) {
    int size = tosend.size() + 1;
    zmq::message_t message(size);
    memcpy ((void *) message.data (), tosend.c_str(), size);
    socketPub.send(message);             
}
int main(int argc, char* argv[]) {
    ZMQServer server;
    while(1) {
        server.response();
        usleep(10000);
    }
}