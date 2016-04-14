/// Boost Asio - EPN
/////
///// \author Christopher M. Kohlhoff (Boost)
///// \author Adam Wegrzynek (CERN)

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using boost::asio::ip::tcp;


typedef boost::shared_ptr<tcp::socket> socket_ptr;
int eventSize;
char *data;

void session(socket_ptr sock)
{
  try
  {
    for (;;)
    {
      

      boost::system::error_code error;
      size_t length = sock->read_some(boost::asio::buffer(data, eventSize), error);
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.
      ///boost::asio::write(*sock, boost::asio::buffer(data, length));
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_service& io_service, short port)
{
  tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    socket_ptr sock(new tcp::socket(io_service));
    a.accept(*sock);
    boost::thread t(boost::bind(session, sock));
  }
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: epn <eventSize>\n";
      return 1;
    }
    eventSize = std::atoi(argv[1]);
    data = new char[eventSize]();

    boost::asio::io_service io_service;

    server(io_service, 5555);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
