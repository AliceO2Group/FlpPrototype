#include <iostream>
#include <string>
#include "zhelpers.h"
#include "Part.cpp"

class Push {
	/// memory to allocate for the buffer
	int memorySize;

	/// message size
	int messageSize;

	/// number of messages that fits in memory
	int messages;

	/// pointer to begining of the buffer
	char *data;

	/// ZeroMQ context
	void *context;

	/// ZeroMQ socket (ZMQ_PUSH)
	void *socket;

	/// ZeroMQ URL
	std::string url;
public:
	static void deallocate(void* data, void* hint);
	/// Initializes context and socet; conntects to server socket
	/// \param _url URL to connect
	Push(std::string _url, int mS, int memS);

	/// Disconnects ZeroMQ socket and destroys the context
	~Push();

	/// Sends full message - header and payload - in two parts
	void send();

	/// Fills up memory (memorySize) with data chunks (messageSize)
	void allocateMemory();

	/// Sends single message (made od two parts)
	void sendMessage(char *buffer);

	/// Sends burst of messages (see variable messages)
	void sendBurst();
};

Push::Push(std::string _url, int mS, int memS) 
{
	messageSize =  mS;
	memorySize = memS;
	messages = (int) memorySize / messageSize;

	url = _url;
	context = zmq_ctx_new ();
    socket = zmq_socket (context, ZMQ_PUSH);
    zmq_bind (socket, url.c_str());

    allocateMemory();
};

Push::~Push() 
{
	zmq_unbind (socket, url.c_str());
	zmq_close (socket);

	zmq_ctx_shutdown (context);
}

void Push::allocateMemory () 
{
    data = (char*)malloc(sizeof(char) * memorySize);
    char *current = data;
    for (int i = 0; i < messages; i++) {
      //std::generate_n(current, messageSize, std::rand);
      std::fill(current, current + messageSize, '*'); 
      current += messageSize;
    }
};

void Push::sendMessage(char *buffer) 
{
	char _header[7] = "header";

	Part *header  = new Part(_header, 6, &deallocate);
	Part *payload = new Part(buffer, messageSize, &deallocate);

	header->send(socket, ZMQ_SNDMORE);
	payload->send(socket);

	delete header;
	delete payload;
};

void Push::sendBurst() 
{
	char *current = data;
	for (int i = 0; i < messages; i++) {
		sendMessage(current);
		current += messageSize;
	}
};
void Push::deallocate(void *data, void *hint) 
{
	//free(data);
};

int main() {
	Push p("tcp://127.0.0.1:5558", 100000, 1000000);
	for (;;) {
		p.sendBurst();
	}
}