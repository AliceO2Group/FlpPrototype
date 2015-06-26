//
//  Hello World client in C++
//  Connects REQ socket to tcp://localhost:5555
//  Sends "Hello" to server, expects "World" back
//
#include "zmq.hpp"
#include <string>
#include <iostream>

using namespace std;

int main ()
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    std::cout << "Connecting to tcp://127.0.0.1:6444/" << std::endl;
    socket.connect ("tcp://127.0.0.1:6444/");

    //  Do 10 requests, waiting each time for a response
    for (;;) {
        string input;
        getline(cin, input);

        int size = input.size() + 1;
        zmq::message_t request (size);
        snprintf ((char *) request.data(), size, "%s", input.c_str());
        cout << "Sending request to publish value: " << input << endl;
        socket.send (request);

        //  Get the reply.
        zmq::message_t reply;
        socket.recv (&reply);
        string value = std::string(static_cast<char*>(reply.data()), reply.size());
        cout << "Confirmation: " << value << endl;
    }
    return 0;
}