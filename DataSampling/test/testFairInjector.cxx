///
/// \file    TestFairSampler.cxx
/// \author  Barthelemy von Haller
///

#include "../include/DataSampling/InjectorInterface.h"
#include "../include/DataSampling/FairInjector.h"
#include "../include/DataSampling/DataBlockProducer.h"

#define BOOST_TEST_MODULE Test of DataSampling
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK // Define this flag to build/use dynamic library.

#include <boost/test/unit_test.hpp>

using namespace AliceO2::DataSampling;
using namespace std;

BOOST_AUTO_TEST_CASE(simple)
{
  auto *dataSamplingInjector = new AliceO2::DataSampling::FairInjector();

  std::vector<std::shared_ptr<DataBlockContainer>> dataBlocks;

  DataBlockProducer producer(false, 1024);

  for (int i = 0; i < 10; i++) {
    std::vector<std::shared_ptr<DataBlockContainer>> blocks;
    DataBlock *block = producer.get();
    std::shared_ptr<DataBlockContainer> containerPtr = std::make_shared<DataBlockContainer>(block);
    blocks.push_back(containerPtr);
    dataSamplingInjector->injectSamples(blocks);
    producer.regenerate();
  }

  delete dataSamplingInjector;

}

