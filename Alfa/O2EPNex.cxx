/// \file O2EPNex.cxx
/// \brief EPN device using FairMQ and data format specified in DataBlock.h (instead of Content structure)
///
/// \author D. Klein, A. Rybalchenko, M.Al-Turany
/// \author modifed by Adam Wegrzynek (21.08.2015)

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "O2EPNex.h"
#include "FairMQLogger.h"

O2EPNex::O2EPNex()
{
}

/// This function was competly rewritten by Adam, it uses structures from DataBlock.h instead of standard Content structure from O2EPNex.h
/// It runs as infinite loop in a new thread
///  - copies the message from FairMQ buffer to char*
///  - parses message header usuing sscanf
///  - creates pointer to payload which straight after header
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
