///
/// \file    TestFileSampler.cxx
/// \author  Barthelemy von Haller
///

#include "../include/DataSampling/DataBlockProducer.h"

#define BOOST_TEST_MODULE Test of DataSampling
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK // Define this flag to build/use dynamic library.

#include <boost/test/unit_test.hpp>

#include <fstream>
#include <DataFormat/DataBlock.h>
#include <boost/algorithm/string.hpp>

using namespace AliceO2::DataSampling;

using namespace std;

//string dataFile = "test.data";
//string dataFileUri = "file:" + dataFile;
//uint32_t payloadSize = 5;
//string s = "abcde";

//struct File
//{
//  File()
//  {
//    cout << "setup File" << endl;
//    AliceO2::DataSampling::DataBlockProducer producer(false, payloadSize /*payload size in bytes*/);
//    // Save 1 block to file
//    producer.saveToFile(dataFile, false);
//  }
//
//  ~File()
//  {
//    cout << "teardown File" << endl;
//    if (remove(dataFile.c_str()) != 0) {
//      cerr << "Error deleting file" << endl;
//    } else {
//      cerr << "File successfully deleted" << endl;
//    }
//  }
//};

BOOST_AUTO_TEST_SUITE(adf)

//BOOST_FIXTURE_TEST_SUITE(FileSampler, File)

//BOOST_AUTO_TEST_CASE(file_test)
//{
//  ifstream file(dataFile, ios::in | ios::binary);
//  DataBlockHeaderBase block;
//  cout << "blockType : " << block.blockType << endl;
//
//  cout << "size of DataBlockHeaderBase : " << sizeof(DataBlockHeaderBase) << endl;
//  file.read((char *) &block, sizeof(DataBlockHeaderBase));
//  cout << "blockType : " << std::hex << block.blockType << endl;
//  cout << "headerSize : " << std::hex << block.headerSize << endl;
//  cout << "payload size : " << std::hex << block.dataSize << endl;
//
//  int dataSizeBytes = block.dataSize / 8;
//  BOOST_CHECK_EQUAL(dataSizeBytes, payloadSize);
//  char buffer[dataSizeBytes + 1];
//  file.read(buffer, dataSizeBytes);
//  cout << "dataSizeBytes : " << dataSizeBytes << endl;
//  buffer[dataSizeBytes] = '\0';
//  cout << "buffer : " << buffer << endl;
//  BOOST_CHECK_EQUAL(string(buffer), s.substr(0, dataSizeBytes));
//  file.close();
//}

// TODO test file creation

BOOST_AUTO_TEST_CASE(test_blockproducer)
{
  uint32_t payloadSize = 30;
  DataBlockProducer producer(false/*not random*/, payloadSize);
  DataBlockContainerReference block = producer.getDataBlockContainer();
  cout << "block id : " << block->getData()->header.id << endl;
  BOOST_CHECK_EQUAL(block->getData()->header.id, 0);
  cout << "blockType : " << std::hex << block->getData()->header.blockType << endl;
  BOOST_CHECK_EQUAL(block->getData()->header.blockType, 0xba);
  cout << "headerSize : " << std::dec << block->getData()->header.headerSize << endl;
  BOOST_CHECK_EQUAL(block->getData()->header.headerSize, 0x60);
  cout << "payload size : " << std::dec << block->getData()->header.dataSize << endl;
  BOOST_CHECK_EQUAL(block->getData()->header.dataSize, payloadSize);

  DataBlockProducer producerRandom(true);
  DataBlockContainerReference block2 = producer.getDataBlockContainer();
  cout << "block2 id : " << block2->getData()->header.id << endl;
  BOOST_CHECK_EQUAL(block2->getData()->header.id, 1);
  cout << "block2Type : " << std::hex << block2->getData()->header.blockType << endl;
  BOOST_CHECK_EQUAL(block2->getData()->header.blockType, 0xba);
  cout << "headerSize : " << std::dec << block2->getData()->header.headerSize << endl;
  BOOST_CHECK_EQUAL(block2->getData()->header.headerSize, 0x60);

  BOOST_CHECK(boost::starts_with(block2->getData()->data, "abcdefghijklmnopqrstuvwxyz"));
}

BOOST_AUTO_TEST_CASE(test_setproducer)
{
  uint32_t payloadSize = 30;

  DataBlockProducer producer(false/*not random*/, payloadSize);
  auto set = producer.getDataSet();
  BOOST_CHECK_EQUAL(set->size(), 1);

  cout << "data : " << set->at(0)->getData()->data << endl;
  BOOST_CHECK(boost::starts_with(set->at(0)->getData()->data, "abcdefghijklmnopqrstuvwxyz"));

  DataBlockContainerReference block = set->at(0);
  cout << "block id : " << block->getData()->header.id << endl;
  BOOST_CHECK_EQUAL(block->getData()->header.id, 0);
  cout << "blockType : " << std::hex << block->getData()->header.blockType << endl;
  BOOST_CHECK_EQUAL(block->getData()->header.blockType, 0xba);
  cout << "headerSize : " << std::dec << block->getData()->header.headerSize << endl;
  BOOST_CHECK_EQUAL(block->getData()->header.headerSize, 0x60);
  cout << "payload size : " << std::dec << block->getData()->header.dataSize << endl;
  BOOST_CHECK_EQUAL(block->getData()->header.dataSize, payloadSize);

  set = producer.getDataSet(10);
  BOOST_CHECK_EQUAL(set->size(), 10);

  cout << "data : " << block->getData()->data << endl;
  BOOST_CHECK(boost::starts_with(block->getData()->data, "abcdefghijklmnopqrstuvwxyz"));
}

BOOST_AUTO_TEST_SUITE_END()
