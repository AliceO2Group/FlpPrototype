///
/// \file   testQcExample.cxx
/// \author Barthelemy von Haller
///

#define private public // hack to have access to everything

#include "../include/Example/ExampleTask.h"
#include "QualityControl/TaskFactory.h"
#include "QualityControl/ObjectsManager.h"
#include <boost/exception/diagnostic_information.hpp>
#include <TSystem.h>

#define BOOST_TEST_MODULE Publisher test
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <assert.h>

#include <TH1.h>

using namespace std;

namespace AliceO2 {
namespace QualityControlModules {
namespace Example {

BOOST_AUTO_TEST_CASE(insantiate_task)
{
  ExampleTask task;
  ObjectsManager manager("MockPublisher");
  task.setObjectsManager(&manager);
  task.initialize();

  BOOST_CHECK(task.mHisto1 != nullptr);
  BOOST_CHECK(task.mHisto2 != nullptr);

  Activity activity;
  task.startOfActivity(activity);
  task.startOfCycle();
  DataBlock block;
  task.monitorDataBlock(block);

  BOOST_CHECK(task.mHisto1->GetEntries() > 0);

  task.endOfCycle();
  task.endOfActivity(activity);
  task.startOfActivity(activity);

  BOOST_CHECK(task.mHisto1->GetEntries() == 0);

  task.endOfActivity(activity);

}

} // namespace Example
} // namespace QualityControlModules
} // namespace AliceO2
