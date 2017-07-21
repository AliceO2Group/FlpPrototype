///
/// \file   testQcTemplate.cxx
/// \author Barthelemy von Haller
///

#include "../include/Template/TemplateTask.h"
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
namespace Template {

BOOST_AUTO_TEST_CASE(instantiate_task)
{
  TemplateTask task;
  TaskConfig config;
  config.publisherClassName = "MockPublisher";
  auto manager = make_shared<ObjectsManager>(config);
  task.setObjectsManager(manager);
  task.initialize();

  BOOST_CHECK(task.getHistogram() != nullptr);

  Activity activity;
  task.startOfActivity(activity);
  task.startOfCycle();
  std::vector<std::shared_ptr<DataBlockContainer>> block;
  task.monitorDataBlock(block);

  BOOST_CHECK(task.getHistogram()->GetEntries() > 0);

  task.endOfCycle();
  task.endOfActivity(activity);
}

} // namespace Template
} // namespace QualityControlModules
} // namespace AliceO2
