#include <pthread.h>
#include <assert.h>
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>

class Client {
	/// nanomsg socket
	int sock;
public:
	/// connects to publisher
	Client(const char *url);
	~Client();

	/// receive data
	void receive();
};
Client::Client(const char *url)
{
	sock = nn_socket (AF_SP, NN_SUB);
	int rcvSize = -1;
	nn_setsockopt(sock, NN_SOL_SOCKET, NN_RCVMAXSIZE, &rcvSize, sizeof(rcvSize));
	assert (nn_setsockopt (sock, NN_SUB, NN_SUB_SUBSCRIBE, "", 0) >= 0);
	assert (nn_connect (sock, url) >= 0);
}
void Client::receive()
{
	while (1) {
		char *buf = NULL;
		nn_recv (sock, &buf, NN_MSG, 0);
		nn_freemsg (buf);
	}
}
Client::~Client()
{
	nn_shutdown (sock, 0);
}

int main (const int argc, const char **argv)
{
	//         URL
	Client c(argv[1]);
	c.receive();
	return 0;
}
