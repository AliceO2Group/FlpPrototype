///
/// \file    TestFairSampler.cxx
/// \author  Barthelemy von Haller
///

#include "../include/DataSampling/InjectorInterface.h"
#include "../include/DataSampling/FairInjector.h"

#define BOOST_TEST_MODULE Test of DataSampling
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK // Define this flag to build/use dynamic library.

#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE(simple)
{
  AliceO2::DataSampling::FairInjector *dataSamplingInjector = new AliceO2::DataSampling::FairInjector();

  std::vector<std::shared_ptr<DataBlockContainer>> dataBlocks;

  for (int i = 0; i < 10; i++) {
    dataSamplingInjector->injectSamples(dataBlocks);
  }

}

