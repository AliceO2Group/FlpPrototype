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
        void publishValue();
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
    if (socketRsp.recv(&request, ZMQ_DONTWAIT) == true) {

        /* !!!!!!!!!!!!!!!! DO SOMEHITNG !!!!!!!!!!!!!!!!!!! */
        int random = rand() % 100;
        stringstream ss;
        ss << "Requested rand value: " << random;
        string tosend = ss.str();

        int size = tosend.size() + 1;
        zmq::message_t reply (size);
        snprintf ((char *) reply.data(), size, "%s", tosend.c_str());
        /* sends messange */
        socketRsp.send (reply);
    }
}
void ZMQServer::publishValue() {
    string tosend = "Published static value: 100";
    int size = tosend.size() + 1;
    zmq::message_t message(size);
    snprintf ((char *) message.data(), size, "%s", tosend.c_str());
    socketPub.send(message);             
}
int main(int argc, char* argv[]) {
    ZMQServer server;
    while(1) {
        server.publishValue(); 
        server.response();
        usleep(100000);
    }
}