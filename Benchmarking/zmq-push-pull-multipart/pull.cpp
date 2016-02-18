#include <iostream>
#include <unistd.h>
#include "zhelpers.h"
 
/// zeromq context
void *context;

/// zmq socket - ZMQ_PUSH
void *socket;

void initZMQ() { 
    context = zmq_ctx_new ();
    socket = zmq_socket (context, ZMQ_PULL);
    zmq_connect (socket, "tcp://127.0.0.1:5558");
}

void receiveMultpart() {
	int i = 0;
	for (;;) {
		usleep(100000);
		
		zmq_msg_t part;
    	int rc = zmq_msg_init (&part);
    	
    	/// receive message
    	rc = zmq_msg_recv (&part, socket, 0);
    	std::cout << "Part " << i << " - " << zmq_msg_size(&part) << " bytes" << std::endl;
       	zmq_msg_close (&part);

       	/// check if there are more parts
       	if (!zmq_msg_more (&part)) break;
    	i++;
	}
}
int main(int argc, char* argv[]) {
	initZMQ();
	receiveMultpart();
	
}