///
/// \file    TestFairSampler.cxx
/// \author  Barthelemy von Haller
///

#include "../include/DataSampling/InjectorDevice.h"

#define BOOST_TEST_MODULE Test of DataSampling
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK // Define this flag to build/use dynamic library.

#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE(direct_injection)
{
  AliceO2::DataSampling::InjectorDevice device;

  for (int i = 0; i < 10; i++) {
    string s = "hello" + std::to_string(i);
    device.sendData(s);
  }
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

