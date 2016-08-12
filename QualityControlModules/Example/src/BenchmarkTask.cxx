///
/// \file   BenchmarkTask.cxx
/// \author Barthelemy von Haller
///

#include "Example/BenchmarkTask.h"
#include <TH1.h>
#include <TCanvas.h>
#include "QualityControl/QcInfoLogger.h"
#include <thread>

using namespace std;

namespace AliceO2 {
namespace QualityControlModules {
namespace Example {

BenchmarkTask::BenchmarkTask()
  : TaskInterface()
{

}

BenchmarkTask::~BenchmarkTask()
{
}

void BenchmarkTask::initialize()
{
  QcInfoLogger::GetInstance() << "initialize benchmarktask \"" << getName() << "\""
                              << AliceO2::InfoLogger::InfoLogger::endm;

  mConfigFile.load("file:./example.ini");
  string taskDefinitionName = mConfigFile.getValue<string>(getName() + ".taskDefinition");
  mNumberHistos = mConfigFile.getValue<size_t>(taskDefinitionName + ".numberHistos");
  mNumberChecks = mConfigFile.getValue<size_t>(taskDefinitionName + ".numberChecks");
  mTypeOfChecks = mConfigFile.getValue<string>(taskDefinitionName + ".typeOfChecks");
  mModuleOfChecks = mConfigFile.getValue<string>(taskDefinitionName + ".moduleOfChecks");

  mHistos.reserve(mNumberHistos);

  // Create and publish the histos
  for (size_t i = 0; i < mNumberHistos; i++) {
    stringstream name;
    name << "histogram_" << getName() << "_" << i;
    mHistos.push_back(new TH1F(name.str().c_str(), name.str().c_str(), 1000, 0, 999));
    getObjectsManager()->startPublishing(getName(), name.str(), mHistos[i]);

    // Add the checks
    for (size_t j = 0; j < mNumberChecks; j++) {
      getObjectsManager()->addCheck(name.str(), "fakeCheck_" + std::to_string(j),
                                    mTypeOfChecks, mModuleOfChecks);
    }
  }
}

void BenchmarkTask::startOfActivity(Activity &activity)
{
  QcInfoLogger::GetInstance() << "startOfActivity" << AliceO2::InfoLogger::InfoLogger::endm;
}

void BenchmarkTask::startOfCycle()
{
  QcInfoLogger::GetInstance() << "startOfCycle" << AliceO2::InfoLogger::InfoLogger::endm;
}

void BenchmarkTask::monitorDataBlock(DataBlock &block)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(100)/*1ms*/);
}

void BenchmarkTask::endOfCycle()
{
  // TODO insert data in plot
  QcInfoLogger::GetInstance() << "endOfCycle" << AliceO2::InfoLogger::InfoLogger::endm;
}

void BenchmarkTask::endOfActivity(Activity &activity)
{
  QcInfoLogger::GetInstance() << "endOfActivity" << AliceO2::InfoLogger::InfoLogger::endm;
}

void BenchmarkTask::Reset()
{
  QcInfoLogger::GetInstance() << "Reset" << AliceO2::InfoLogger::InfoLogger::endm;
}

}
}
}
