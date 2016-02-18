#include <iostream>
#include <algorithm>
#include <unistd.h>
#include "zhelpers.h"
	
/// memory to allocate for the buffer
int memorySize;

/// event size
int eventSize;

/// number of events that fits in memory
int blocksNumber;

/// pointer to begining of the buffer
char *data;
    
/// zeromq context
void *context;

/// zmq socket - ZMQ_PUSH
void *socket;
	
/// inits context and binds the PULL socket
void initZMQ() {
    context = zmq_ctx_new ();
    socket = zmq_socket (context, ZMQ_PUSH);
    zmq_bind (socket, "tcp://127.0.0.1:5558");
}

/// dealocates messages sent via zeromq
void deallocate(void *data, void *hint) {
	//free(data);
}

void allocate () {
    data = (char*)malloc(sizeof(char) * memorySize);
    char *current = data;
    for (int i = 0; i < blocksNumber; i++) {
      //std::generate_n(current, eventSize, std::rand);
      std::fill(current, current + eventSize, '*'); 
      current += eventSize;
    }
}
void sendChunk(char *buffer) {
	char _header[7] = "header";

	zmq_msg_t header;
	zmq_msg_t payload;
	int rc;
	
	/// Send header
	//rc = zmq_msg_init_data (&header, _header, 6, deallocate, NULL);
	//rc = zmq_msg_send (&header, socket, ZMQ_SNDMORE);
	//std::cout << "Sent header" << std::endl;
	/// Send payload
	rc = zmq_msg_init_data (&payload, buffer, eventSize, deallocate, NULL);
	rc = zmq_msg_send (&payload, socket, 0);
	
	std::cout << "Sent payload" << std::endl;
}

int main(int argc, char* argv[]) {
	eventSize =  1000000;
	memorySize = 10000000;
	blocksNumber = (int) memorySize / eventSize;

	allocate();
	initZMQ();
	for (;;) {
		char *current = data;
		for (int i = 0; i < blocksNumber; i++) {
			sendChunk(current);
			current += eventSize;
			usleep(1000000);
		}
	}
}