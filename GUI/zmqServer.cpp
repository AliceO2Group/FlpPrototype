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
    socketRsp.recv(&request);

    /* !!!!!!!!!!!!!!!! DO SOMEHITNG !!!!!!!!!!!!!!!!!!! */
            
    /* prepare reply message (max 100b) */
    zmq::message_t reply (100);
    /* puts into json structure */
    snprintf ((char *) reply.data(), 100 ,"{\"source\":\"%s\"}", "subscriber");
    /* sends messange */
    socketRsp.send (reply);
}
void ZMQServer::publishValue() { 
    zmq::message_t message(100);
    snprintf ((char *) message.data(), 100 ,"{\"source\":\"%s\"}", "publish");
    socketPub.send(message);             
}
int main(int argc, char* argv[]) {
    ZMQServer server;
    while(1) {
        server.response();
        server.publishValue(); 
        sleep(1);
    }
;
}