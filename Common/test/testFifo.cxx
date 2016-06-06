#include "../include/Common/Fifo.h"

#define BOOST_TEST_MODULE Fifo test
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <assert.h>



BOOST_AUTO_TEST_CASE(fifo_test)
{
  int fifoSz=100;

  AliceO2::Common::Fifo<int> f(fifoSz);
  int *v=new int[fifoSz];
  int *j=NULL;
  int sum1=0;
  int sum2=0;
  
  BOOST_CHECK_EQUAL(f.isEmpty(),1);
  BOOST_CHECK_EQUAL(f.isFull(),0);
  BOOST_CHECK_PREDICATE( std::not_equal_to<int>(), (f.pop(j))(0) );
  for (int i=0;i<fifoSz;i++){
    v[i]=i;
    sum1+=i;
    BOOST_CHECK_EQUAL(f.isFull(),0);
    BOOST_CHECK_EQUAL(f.push(&v[i]),0);
    BOOST_CHECK_EQUAL(f.isEmpty(),0);
  }
  BOOST_CHECK_EQUAL(f.isFull(),1);
  BOOST_CHECK_PREDICATE( std::not_equal_to<int>(), (f.push(NULL))(0) ); 

  for (int i=0;i<fifoSz;i++){
    j=NULL;
    BOOST_CHECK_EQUAL(f.isEmpty(),0);
    BOOST_CHECK_EQUAL(f.pop(j),0);
    BOOST_CHECK_EQUAL(f.isFull(),0);
    sum2+=*j;
  }
  BOOST_CHECK_EQUAL(f.isEmpty(),1);
  BOOST_CHECK_PREDICATE( std::not_equal_to<int>(), (f.pop(j))(0) );

  BOOST_CHECK_EQUAL(sum1,sum2);  
  delete[] v;
  
  printf("fifoSz=%d sum=%d\n",fifoSz,sum2);
}
