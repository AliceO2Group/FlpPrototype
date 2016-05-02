///
/// @file    TestHello.cxx
/// @author  Barthelemy von Haller
///

#include "../include/Example/ExampleTask.h"

#define BOOST_TEST_MODULE hello test
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <assert.h>
#include <iostream>

using namespace std;

BOOST_AUTO_TEST_CASE(arithmetic_test)
{
  int a = 1;
  int b = 2;
  BOOST_CHECK_NE(a, b);
  b = a;
  BOOST_CHECK_EQUAL(a, b);
}

namespace AliceO2 {
namespace QualityControl {

using namespace Core;

namespace Test {
class TestTask : public TaskInterface
{
  public:
    TestTask(ObjectsManager *objectsManager) : TaskInterface(objectsManager)
    {
      test = 0;
    }

    virtual ~TestTask()
    {

    }

    // Definition of the methods for the template method pattern
    virtual void initialize()
    {
      cout << "initialize" << endl;
      test = 1;
    }

    virtual void startOfActivity(Activity &activity)
    {
      cout << "startOfActivity" << endl;
      test = 2;
    }

    virtual void startOfCycle()
    {
      cout << "startOfCycle" << endl;
    }

    virtual void monitorDataBlock(DataBlock &block)
    {
      cout << "monitorDataBlock" << endl;
    }

    virtual void endOfCycle()
    {
      cout << "endOfCycle" << endl;
    }

    virtual void endOfActivity(Activity &activity)
    {
      cout << "endOfActivity" << endl;
    }

    virtual void Reset()
    {
      cout << "Reset" << endl;
    }

    int test;
};

} /* namespace Test */
} /* namespace QualityControl */
} /* namespace AliceO2 */

//BOOST_AUTO_TEST_CASE(TestInstantiate)
//{
//  ObjectsManager objectsManager; // TODO we need a mock object
//  Test::TestTask tt("asdf", &objectsManager);
//  BOOST_CHECK_EQUAL(tt.test, 0);
//  tt.initialize();
//  BOOST_CHECK_EQUAL(tt.test, 1);
//  Activity act;
//  tt.startOfActivity(act);
//  BOOST_CHECK_EQUAL(tt.test, 2);
//}

