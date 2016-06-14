#include "../include/Common/Timer.h"

#define BOOST_TEST_MODULE Timer test
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <assert.h>

#include <time.h>

BOOST_AUTO_TEST_CASE(timer_test)
{
  AliceO2::Common::Timer t;
  time_t t0;
  t0=time(NULL);
  
  while (time(NULL)==t0) {}
  t.reset();
  while (time(NULL)<=t0+1) {}
  
  double elapsed=t.getTime();
  
  printf("Timer elapsed=%.4lf\n",elapsed);
  int success=0;
  if (fabs(elapsed-1.0)<0.01) {
    success=1;
  }

  BOOST_CHECK_EQUAL(success, 1);
}
