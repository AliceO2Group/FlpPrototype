///
/// @file    TestHello.cxx
/// @author  Barthelemy von Haller
///

#include "../libs/datasampling/DataSampling.h"

#define BOOST_TEST_MODULE Test of DataSampling
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK // Define this flag to build/use dynamic library.
#include <boost/test/unit_test.hpp>
#include <assert.h>

BOOST_AUTO_TEST_CASE(arithmetic_test)
{
  int a = 1;
  int b = 2;
  BOOST_CHECK_NE(a, b);
  b = a;
  BOOST_CHECK_EQUAL(a, b);
}

BOOST_AUTO_TEST_CASE(basic)
{
  AliceO2::DataSampling::DataSampling sampling;
  sampling.setLocation("file:test.data");
  BOOST_CHECK_EQUAL(sampling.getLocation(), "file:test.data");
}
