#include <iostream>
#include <unistd.h>
#include "zhelpers.h"
 





class Pull {
  /// zeromq context
  void *context;

  /// zmq socket - ZMQ_PUSH
  void *socket;

  /// Receive event in multi-part message (event and payload)
  void receiveMultpart();
public:

  /// Initialize ZeroMQ context and connect to server socket
  Pull();

  void receive();
};


Pull::Pull() { 
    context = zmq_ctx_new ();
    socket = zmq_socket (context, ZMQ_PULL);
    zmq_connect (socket, "tcp://127.0.0.1:5558");
};

void Pull::receiveMultpart() {
	int i = 0;
	for (;;) {	
		  zmq_msg_t part;
    	int rc = zmq_msg_init (&part);
    	
    	/// receive message
    	rc = zmq_msg_recv (&part, socket, 0);

      int size = zmq_msg_size(&part);

      char* data = (char*) zmq_msg_data(&part);
      std::cout << data << std::endl;

    	//std::cout << "Part " << i << " - " << zmq_msg_size(&part) << " bytes" << std::endl;
      zmq_msg_close (&part);

      /// check if there are more parts
      if (!zmq_msg_more (&part)) break;
    	i++;
	}
};

void Pull::receive() {
  for (;;) {
    receiveMultpart();
  }
}
int main(int argc, char* argv[]) {
	Pull p;
	p.receive();
	
}