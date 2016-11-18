///
/// \file testGuiRemote.cxx
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#include "../include/GUI/zmq.hpp"
#include <string>
#include <iostream>

#define BOOST_TEST_MODULE testGuiRemote
#include <boost/test/included/unit_test.hpp>

namespace AliceO2 {
namespace GUI {
namespace Test {

/// Connects to the server and sends 10 sample message requests
BOOST_AUTO_TEST_CASE(remoteServerRequests)
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    socket.connect ("tcp://127.0.0.1:3001");

    //  Do 10 requests, waiting each time for a response
    for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
        std::string message = "{\"name\" : \"hello\", \"value\" : 123 }"; 
        zmq::message_t request (message.size());
        memcpy (request.data (), message.data(), message.size());
        std::cout << "Sending request to server... " << std::endl;
        socket.send (request);

        //  Get the reply.
        zmq::message_t reply;
        socket.recv (&reply);
    }
    BOOST_CHECK(true);
}

} // namespace Test
} // namespace GUI
} // namespace AliceO2
