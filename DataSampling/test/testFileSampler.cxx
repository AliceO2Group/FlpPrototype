///
/// \file    TestHello.cxx
/// \author  Barthelemy von Haller
///

#include "../libs/datasampling/FileSampler.h"

#define BOOST_TEST_MODULE Test of DataSampling
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK // Define this flag to build/use dynamic library.
#include <boost/test/unit_test.hpp>
#include <assert.h>

#include <fstream>
#include <../../DataFormat/include/DataBlock.h> // TODO proper path once DataFormat can be installed

using namespace std;

string dataFile = "test.data";
string dataFileUri = "file:" + dataFile;
int payloadSize = 5;
string s = "barthelemy";

struct File
{
    File()
    {
      cout << "setup File" << endl;
      fstream file(dataFile, ios::out | ios::binary);
      if (file.is_open()) {
        DataBlockHeaderBase block;
        block.blockType = 0xba; // whatever
        block.headerSize = 0x60; // just the header base -> 96 bits
        block.dataSize = payloadSize * 8;
        file.write((char*) &block, sizeof(DataBlockHeaderBase));
        cout << "size of DataBlockHeaderBase : " << sizeof(DataBlockHeaderBase) << endl;
        char buffer[payloadSize];
        s.copy(buffer, payloadSize);
        cout << "size of buffer : " << sizeof(buffer) << endl;
        file.write(buffer, payloadSize);
        file.close();
      } else {
        std::cerr << "Couldn't open '" << dataFile << "'" << std::endl;
      }
    }

    ~File()
    {
      cout << "teardown File" << endl;
      if (remove(dataFile.c_str()) != 0)
        cerr << "Error deleting file" << endl;
      else
        cerr << "File successfully deleted" << endl;
    }
};

BOOST_FIXTURE_TEST_SUITE(FileSampler, File)

BOOST_AUTO_TEST_CASE(basic)
{
  AliceO2::DataSampling::FileSampler fileSampler;
  fileSampler.setLocation(dataFileUri);
  BOOST_CHECK_EQUAL(fileSampler.getLocation(), dataFileUri);
  fileSampler.setDataFormat(AliceO2::DataSampling::DataFormat::Raw);

  DataBlock *data = fileSampler.getData();
  if(!data) {
    BOOST_ERROR("pointer is null");
  }
  cout << "blockType : " << std::hex << data->header.blockType << endl;
  cout << "headerSize : " << std::hex << data->header.headerSize << endl;
  cout << "payload size : " << std::hex << data->header.dataSize << endl;
  int dataSizeBytes = data->header.dataSize / 8;
  BOOST_CHECK_EQUAL(dataSizeBytes, payloadSize);
  cout << "data from block from sampler : " << endl;
  for(int i = 0 ; i < dataSizeBytes ; i++) {
    cout << data->data[i];
  }
  cout << endl;
}

BOOST_AUTO_TEST_CASE(file_test)
{
  ifstream file(dataFile, ios::in | ios::binary);
  DataBlockHeaderBase block;
  cout << "blockType : " << block.blockType << endl;

  cout << "size of DataBlockHeaderBase : " << sizeof(DataBlockHeaderBase) << endl;
  file.read((char*) &block, sizeof(DataBlockHeaderBase));
  cout << "blockType : " << std::hex << block.blockType << endl;
  cout << "headerSize : " << std::hex << block.headerSize << endl;
  cout << "payload size : " << std::hex << block.dataSize << endl;

  int dataSizeBytes = block.dataSize / 8;
  BOOST_CHECK_EQUAL(dataSizeBytes, payloadSize);
  char buffer[dataSizeBytes + 1];
  file.read(buffer, dataSizeBytes);
  cout << "dataSizeBytes : " << dataSizeBytes << endl;
  buffer[dataSizeBytes] = '\0';
  cout << "buffer : " << buffer << endl;
  BOOST_CHECK_EQUAL(string(buffer), s.substr(0, dataSizeBytes));
  file.close();
}

BOOST_AUTO_TEST_SUITE_END()
