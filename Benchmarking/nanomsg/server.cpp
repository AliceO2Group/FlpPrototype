#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <assert.h>
#include <pthread.h>
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>

class Server {
        /// memory to be allocated
	int memorySize;

        /// message size
	int messageSize;

        /// number of messages that fits in memory
        int messages;

	/// allocated data
	char *data;

	/// nanomsg socket
	int sock;
public:
	/// binds socket calculats params
	Server(const char *url, int _memorySize, int _messageSize);
	~Server();
	/// allocates memory
	void allocateMemory();

	/// sends chunks of data
	void send();

};

Server::Server(const char *url, int _memorySize, int _messageSize)
{
	messageSize =  _messageSize;
	memorySize = _memorySize;
	messages = (int) memorySize / messageSize;
	sock = nn_socket (AF_SP, NN_PUB);
	int rcvSize = -1;
	nn_setsockopt(sock, NN_SOL_SOCKET, NN_RCVMAXSIZE, &rcvSize, sizeof(rcvSize));
	assert (sock >= 0);
	assert (nn_bind (sock, url) >= 0);
}

Server::~Server()
{
	free(data);
	nn_shutdown (sock, 0);
}
void Server::allocateMemory()
{
	data = (char*)malloc(sizeof(char) * memorySize);
	char *current = data;
	for (int i = 0; i < messages; i++) {
		std::fill(current, current + messageSize, '*');
		current += messageSize;
	}
}

void Server::send()
{
	char *current = data;
	for (int i = 0; i < messages; i++) {
		nn_send (sock, current, messageSize, 0);
		current += messageSize;
	}
}

int main (const int argc, const char **argv)
{
	///      URL      memory size    block size
	Server s(argv[1], atoi(argv[2]), atoi(argv[3]));
	s.allocateMemory();
	for (;;) {
		s.send();
	}
}
