///
/// \file    TestFileSampler.cxx
/// \author  Barthelemy von Haller
///

#include "../include/DataSampling/FairSampler.h"

#define BOOST_TEST_MODULE Test of DataSampling
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK // Define this flag to build/use dynamic library.

#include <boost/test/unit_test.hpp>

#include <fstream>

using namespace std;

BOOST_AUTO_TEST_CASE(simple_fair_sampler)
{
  AliceO2::DataSampling::FairSampler sampler;
  this_thread::sleep_for(chrono::seconds(10));

  DataBlock *block;

//  for (int i = 0; i < 100; i++) {
//    block = sampler.getData();
//
//    if (!block) {
//      BOOST_ERROR("pointer is null");
//      return;
//    }
//    cout << "blockType : " << std::hex << block->header.blockType << endl;
//    cout << "headerSize : " << std::hex << block->header.headerSize << endl;
//    cout << "payload size : " << std::dec << block->header.dataSize << endl;
//    int dataSizeBytes = block->header.dataSize / 8;
//    BOOST_CHECK_GT(dataSizeBytes, 0);
//
//    sampler.releaseData();
//  }
}

