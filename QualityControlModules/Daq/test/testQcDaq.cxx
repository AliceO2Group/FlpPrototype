///
/// \file   testQcDaq.cxx
/// \author Barthelemy von Haller
///

#include "Daq/DaqTask.h"
#include "QualityControl/TaskFactory.h"
#include "QualityControl/ObjectsManager.h"
#include <boost/exception/diagnostic_information.hpp>
#include <TSystem.h>

#define BOOST_TEST_MODULE Publisher test
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <cassert>

#include <TH1.h>

using namespace std;

namespace AliceO2 {
namespace QualityControlModules {
namespace Daq {

BOOST_AUTO_TEST_CASE(instantiate_task)
{
  DaqTask task;
  TaskConfig config;
  auto manager = make_shared<ObjectsManager>(config);
  task.setObjectsManager(manager);
  task.initialize();

  BOOST_CHECK(manager->getMonitorObject("example")->getObject() != nullptr);

  Activity activity;
  task.startOfActivity(activity);
  task.startOfCycle();
  std::vector<std::shared_ptr<DataBlockContainer>> block;
  task.monitorDataBlock(block);

  TH1F *histo = (TH1F *) manager->getMonitorObject("example")->getObject();
  BOOST_CHECK(histo->GetEntries() > 0);

  task.endOfCycle();
  task.endOfActivity(activity);
}

} // namespace Daq
} // namespace QualityControlModules
} // namespace AliceO2
