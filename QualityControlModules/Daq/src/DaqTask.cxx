///
/// \file   DaqTask.cxx
/// \author Barthelemy von Haller
///

#include "Daq/DaqTask.h"
#include "QualityControl/QcInfoLogger.h"
#include <TH1.h>
#include <TCanvas.h>

using namespace std;

namespace AliceO2 {
namespace QualityControlModules {
namespace Daq {

DaqTask::DaqTask()
  : TaskInterface(), mHistogram(nullptr)
{
  mHistogram = nullptr;
}

DaqTask::~DaqTask()
{
}

void DaqTask::initialize()
{
  QcInfoLogger::GetInstance() << "initialize DaqTask" << AliceO2::InfoLogger::InfoLogger::endm;

  mHistogram = new TH1F("example", "Example", 100, 0, 99);
  getObjectsManager()->startPublishing(mHistogram);
  getObjectsManager()->addCheck(mHistogram, "checkFromDaq",
                                "AliceO2::QualityControlModules::Daq::DaqCheck",
                                "QcDaq");
}

void DaqTask::startOfActivity(Activity &activity)
{
  QcInfoLogger::GetInstance() << "startOfActivity" << AliceO2::InfoLogger::InfoLogger::endm;
}

void DaqTask::startOfCycle()
{
  QcInfoLogger::GetInstance() << "startOfCycle" << AliceO2::InfoLogger::InfoLogger::endm;
}

void DaqTask::monitorDataBlock(std::vector<std::shared_ptr<DataBlockContainer>> &block)
{
  mHistogram->FillRandom("gaus", 1);
}

void DaqTask::endOfCycle()
{
  QcInfoLogger::GetInstance() << "endOfCycle" << AliceO2::InfoLogger::InfoLogger::endm;
}

void DaqTask::endOfActivity(Activity &activity)
{
  QcInfoLogger::GetInstance() << "endOfActivity" << AliceO2::InfoLogger::InfoLogger::endm;
}

void DaqTask::reset()
{
  QcInfoLogger::GetInstance() << "Reset" << AliceO2::InfoLogger::InfoLogger::endm;
}

}
}
}
