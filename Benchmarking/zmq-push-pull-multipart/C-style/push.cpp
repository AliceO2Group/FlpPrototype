#include <iostream>
#include <algorithm>
#include "zhelpers.h"

class Push {
	/// memory to allocate for the buffer
	int memorySize;

	/// event size
	int eventSize;

	/// number of events that fits in memory
	int events;

	/// pointer to begining of the buffer
	char *data;
	    
	/// zeromq context
	void *context;

	/// zmq socket - ZMQ_PUSH
	void *socket;

	/// sends single event
	void sendEvent(char*);
	
public:
	/// initialaze variables, ZeroMQ context and socket
	Push(int, int);

	/// Sends single event
	void send();

	/// Fills up memory (memorySize) with data chunks (eventSize)
	void allocate();

	/// dealocates memory - passed by reference to ZeroMQ
	static void deallocate(void *, void*);
};

Push::Push(int eS, int mS) {
	// init vars
	eventSize =  eS;
	memorySize = mS;
	events = (int) memorySize / eventSize;

	// inits context and binds the PUSH socket
	context = zmq_ctx_new ();
    socket = zmq_socket (context, ZMQ_PUSH);
    zmq_bind (socket, "tcp://127.0.0.1:5558");
};
void Push::deallocate(void *data, void *hint) {
	//free(data);
};

void Push::allocate () {
    data = (char*)malloc(sizeof(char) * memorySize);
    char *current = data;
    for (int i = 0; i < events; i++) {
      //std::generate_n(current, eventSize, std::rand);
      std::fill(current, current + eventSize, '*'); 
      current += eventSize;
    }
};
void Push::sendEvent(char *buffer) {
	char _header[7] = "header";

	zmq_msg_t header;
	zmq_msg_t payload;
	int rc;
	
	/// Send header
	rc = zmq_msg_init_data (&header, _header, 6, deallocate, NULL);
	rc = zmq_msg_send (&header, socket, ZMQ_SNDMORE);
	//std::cout << "Sent header" << std::endl;
	/// Send payload
	rc = zmq_msg_init_data (&payload, buffer, eventSize, &deallocate, NULL);
	rc = zmq_msg_send (&payload, socket, 0);
};
void Push::send() {
	for (;;) {
		char *current = data;
		for (int i = 0; i < events; i++) {
			sendEvent(current);
			current += eventSize;
		}
	}
};

int main(int argc, char* argv[]) {
	Push p(100000, 1000000);
	p.allocate();
	p.send();
}