/// \file DataBlockTransport.cxx
/// \brief Allows to contiguous memory allocation for DataBlock structure
/// \author Adam Wegrzynek, CERN

#include <iostream>
#include <cstring>
#include "DataBlock.h"

using namespace std;

class DataBlockTransport {
protected:
	DataBlock *frame;
public:
	void setPayload(char*, int);
};

void DataBlockTransport::setPayload(char *payload, int size) {
	//allocates memory for DataBlock
	int *buffer = (int*)malloc(sizeof(DataBlockHeaderBase) + sizeof(char) * size);
	*buffer = 1; //block type
	*(buffer + 1) = sizeof(DataBlockHeaderBase); //header size
	*(buffer + 2) = size; //data size

	frame = new DataBlock();
	frame->header = (DataBlockHeaderBase*) buffer;
	frame->data = (char*)(buffer + 3);
	memcpy(frame->data, payload, size);

	cout << "block type: " << frame->header->blockType << "; header size: "
		 << frame->header->headerSize << "; data size: " << frame->header->dataSize << "; data: "
		 << frame->data << endl;
}

int main() {
	//data received from detector
	char payload[20] = "Lorem ipsum dolor s";

	DataBlockTransport * transport = new DataBlockTransport();
	transport->setPayload(payload, sizeof(payload));

	delete(transport);
	return 0;
}
