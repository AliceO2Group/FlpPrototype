#include <iostream>
#include "zhelpers.h"

/// \brief Represent message-part - mostly wraps ZeroMQ C-functions
/// \author Adam Wegrzynek
class Part {

	/// ZeroMQ message object
	zmq_msg_t part;

public:
	/// Default deallocate function
	/// \param data 	pointer to data that is about to be deallocated
	static void deallocate(void* data, void* hint);
	
	/// \return size of stored data
	int getSize();

	/// \return void pointer to data
	void *getData();

	/// Receives message-part from provided socket
	/// \param socket 	ZeroMQ socket
	void receive(void *socket);

	/// States if there's following message part
	bool isMore();

	/// Send message-parth via socket
	void send(void *socket, int flags = 0); 

	/// Initializes message-part (receiving)
	Part();

	/// Initializes message-part (sending)
	/// \param data 	pointer to data block
	/// \param size 	size of data
	/// \param ffn 		function that deallocates memory (free() as default)
	Part(void *data, size_t size, zmq_free_fn *ffn = &deallocate);

	//// Closes message-part - notifies ZeroMQ that memory can be deallocated
	~Part();
};

Part::Part() 
{
	zmq_msg_init (&part);
};

Part::Part(void *data, size_t size, zmq_free_fn *ffn)
{
	 zmq_msg_init_data (&part, data, size, ffn, NULL);
};

Part::~Part() 
{
	zmq_msg_close (&part);
};

bool Part::isMore() {
	return zmq_msg_more (&part);
};

void Part::receive(void *socket) 
{
	zmq_msg_recv (&part, socket, 0);
};

int Part::getSize () 
{
	return zmq_msg_size (&part);
};

void *Part::getData() 
{
	return zmq_msg_data (&part);
};

void Part::deallocate(void *data, void *hint) 
{
	free(data);
};

void Part::send(void *socket, int flags) 
{
	zmq_msg_send (&part, socket, flags);
};