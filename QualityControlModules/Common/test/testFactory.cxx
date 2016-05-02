///
/// \file   testNonEmpty.cxx
/// \author flpprotodev
///

#include "../include/Common/NonEmpty.h"
#include "QualityControl/TaskFactory.h"
#include "QualityControl/ObjectsManager.h"
#include <boost/exception/diagnostic_information.hpp>

#define BOOST_TEST_MODULE Publisher test
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <assert.h>

#include <TH1.h>

namespace AliceO2 {
namespace QualityControlModules {
namespace Common {

BOOST_AUTO_TEST_CASE(Factory)
{
  TaskFactory factory;
  ObjectsManager manager("MockPublisher");
  try {
    factory.create("QcCommon", "AliceO2::QualityControlModules::Common::NonEmpty", &manager);
  } catch (...) {
    BOOST_TEST_FAIL(boost::current_exception_diagnostic_information());
  }
}

} // namespace Checker 
} // namespace QualityControl 
} // namespace AliceO2 
