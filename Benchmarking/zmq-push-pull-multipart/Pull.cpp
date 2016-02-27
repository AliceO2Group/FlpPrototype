#include <iostream>
#include <string>
#include "zhelpers.h"
#include "Part.cpp"

class Pull {
	/// ZeroMQ context
	void *context;

	/// ZeroMQ socket (ZMQ_PULL)
	void *socket;

	/// ZeroMQ URL
	std::string url;
public:

	/// Initializes context and socet; conntects to server socket
	/// \param _url URL to connect
	Pull(std::string _url);

	/// Disconnects ZeroMQ socket and destroys the context
	~Pull();

	/// Receives full message - header and payload - in two parts
	void receive();
};

Pull::Pull(std::string _url)
{
	url = _url;
	context = zmq_ctx_new ();
    socket = zmq_socket (context, ZMQ_PULL);
    zmq_connect (socket, url.c_str());
};

Pull::~Pull()
{
	zmq_disconnect (socket, url.c_str());
	zmq_close (socket);

	zmq_ctx_shutdown (context);
	zmq_ctx_destroy (context);

};

void Pull::receive()
{
	Part *header  = new Part();
	Part *payload = new Part();
	header->receive(socket);
	if (header->isMore()) payload->receive(socket);
	//printf("%.*s", header->getSize(), header->getData());
	delete header;
	delete payload;
};

int main() {
	Pull p("tcp://127.0.0.1:5558");
	for (;;) {
		p.receive();
	}
}