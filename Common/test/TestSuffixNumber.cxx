#include "Common/SuffixNumber.h"

#define BOOST_TEST_MODULE SuffixNumber
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using AliceO2::Common::SuffixNumber;

BOOST_AUTO_TEST_CASE(TestInteger)
{
  SuffixNumber<int32_t> sn;

  sn.setNumber("0");
  BOOST_CHECK_EQUAL(sn.getNumber(), 0);

  sn.setNumber("2147483647");
  BOOST_CHECK_EQUAL(sn.getNumber(), 2147483647);
  BOOST_CHECK_THROW(sn.setNumber("2147483648"), AliceO2::Common::Exception);

  sn.setNumber("-2147483648");
  BOOST_CHECK_EQUAL(sn.getNumber(), -2147483648);
  BOOST_CHECK_THROW(sn.setNumber("-2147483649"), AliceO2::Common::Exception);
}

BOOST_AUTO_TEST_CASE(TestFloatingPoint)
{
  SuffixNumber<double> sn;

  sn.setNumber("0");
  BOOST_CHECK_EQUAL(sn.getNumber(), 0);

  sn.setNumber("123");
  BOOST_CHECK_EQUAL(sn.getNumber(), 123);

  sn.setNumber("-123");
  BOOST_CHECK_EQUAL(sn.getNumber(), -123);
}

