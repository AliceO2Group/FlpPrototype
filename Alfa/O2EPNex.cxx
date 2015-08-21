/**
 * O2EPNex.cxx
 *
 * @since 2013-01-09
 * @author D. Klein, A. Rybalchenko, M.Al-Turany
 */

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "O2EPNex.h"
#include "FairMQLogger.h"

O2EPNex::O2EPNex()
{
}

void O2EPNex::Run()
{
	LOG(INFO) << "Using DataBlock.h";
	while (GetCurrentState() == RUNNING) {
    	FairMQMessage* msg = fTransportFactory->CreateMessage();
		fChannels["data-in"].at(0).Receive(msg);
		//int inputSize = msg->GetSize();
		DataBlock* input = new DataBlock();
		DataBlockHeaderBase header;
		//saving received data to char array
		char* received = reinterpret_cast<char*>(msg->GetData());
		//parsing the data
		std::sscanf(received, "%10d%10d%10d", &(header.blockType), &(header.headerSize), &(header.dataSize));
		//seting the data pointer
		input->data = received + header.headerSize;
		//LOG(INFO) << "blockType: " <<  header.blockType << "; headerSize: " << header.headerSize << "; dataSize: " << header.dataSize << "; data: "  << input->data;
		delete input;
		delete msg;
	}
}

O2EPNex::~O2EPNex()
{
}
