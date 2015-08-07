///
/// \file    TestFileSampler.cxx
/// \author  Barthelemy von Haller
///

#include "../libs/datasampling/DataBlockProducer.h"

#define BOOST_TEST_MODULE Test of DataSampling
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK // Define this flag to build/use dynamic library.

#include <boost/test/unit_test.hpp>

#include <fstream>

using namespace std;

string dataFile = "test.data";
string dataFileUri = "file:" + dataFile;
uint32_t payloadSize = 5;
string s = "abcde";

struct File
{
  File()
  {
    cout << "setup File" << endl;
    AliceO2::DataSampling::DataBlockProducer producer(false, payloadSize /*payload size in bytes*/);
    // Save 1 block to file
    producer.saveToFile(dataFile, false);
  }

  ~File()
  {
    cout << "teardown File" << endl;
    if (remove(dataFile.c_str()) != 0) {
      cerr << "Error deleting file" << endl;
    } else {
      cerr << "File successfully deleted" << endl;
    }
  }
};

BOOST_FIXTURE_TEST_SUITE(FileSampler, File)

BOOST_AUTO_TEST_CASE(file_test)
{
  ifstream file(dataFile, ios::in | ios::binary);
  DataBlockHeaderBase block;
  cout << "blockType : " << block.blockType << endl;

  cout << "size of DataBlockHeaderBase : " << sizeof(DataBlockHeaderBase) << endl;
  file.read((char *) &block, sizeof(DataBlockHeaderBase));
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
