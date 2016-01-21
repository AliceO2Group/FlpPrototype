//
// blocking_tcp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

/// memory to allocate for the buffer
int memorySize;

/// event size
int eventSize;

/// number of events that fits in memory
int blocksNumber;

/// pointer to begining of the buffer
char *data;


/// allocates event in the buffer - filling with random data
void allocate (int eS, int mS) {
    eventSize = eS;
    memorySize = mS;
    blocksNumber = (int) memorySize / eventSize;
    data = (char*)malloc(sizeof(char) * memorySize);
    char *current = data;
    for (int i = 0; i < blocksNumber; i++) {
      //std::fill(current, current + eventSize, '*');
      std::generate_n(current, eventSize, std::rand);   
      current += eventSize;
    }
}


int main(int argc, char* argv[])
{
  try
  {
    if (argc != 4)
    {
      std::cerr << "Usage: flp <host> <memorySize> <eventSize>\n";
      return 1;
    }
    allocate(std::atoi(argv[2]), std::atoi(argv[3]));
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), argv[1], "5555");
    tcp::resolver::iterator iterator = resolver.resolve(query);

    tcp::socket s(io_service);
    boost::asio::connect(s, iterator);
    for (;;) {
      char *current = data;
      for (int i = 0; i < blocksNumber; i++) {
        boost::asio::write(s, boost::asio::buffer(current, eventSize));
        current += eventSize;
      }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}