#include "Common/Iommu.h"

#define BOOST_TEST_MODULE Iommu
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestIsEnabled)
{
  BOOST_CHECK_NO_THROW(AliceO2::Common::Iommu::isEnabled());
}
