///
/// \file   TemplateTask.cxx
/// \author Barthelemy von Haller
///

#include "Template/TemplateTask.h"
#include "QualityControl/QcInfoLogger.h"
#include <TH1.h>
#include <TCanvas.h>

using namespace std;

namespace AliceO2 {
namespace QualityControlModules {
namespace Template {

TemplateTask::TemplateTask()
  : TaskInterface(), mHistogram(nullptr)
{
  mHistogram = nullptr;
}

TemplateTask::~TemplateTask()
{
}

void TemplateTask::initialize()
{
  QcInfoLogger::GetInstance() << "initialize TemplateTask" << AliceO2::InfoLogger::InfoLogger::endm;

  mHistogram = new TH1F("example", "Example", 100, 0, 99);
  getObjectsManager()->startPublishing(mHistogram);
  getObjectsManager()->addCheck(mHistogram, "checkNonEmpty", "AliceO2::QualityControlModules::Common::NonEmpty",
                                "QcCommon");
  getObjectsManager()->addCheck(mHistogram, "checkFromTemplate", "AliceO2::QualityControlModules::Template::TemplateCheck",
                                "QcTemplate");
}

void TemplateTask::startOfActivity(Activity &activity)
{
  QcInfoLogger::GetInstance() << "startOfActivity" << AliceO2::InfoLogger::InfoLogger::endm;
}

void TemplateTask::startOfCycle()
{
  QcInfoLogger::GetInstance() << "startOfCycle" << AliceO2::InfoLogger::InfoLogger::endm;
}

void TemplateTask::monitorDataBlock(std::vector<std::shared_ptr<DataBlockContainer>> &block)
{
  mHistogram->FillRandom("gaus", 1);
}

void TemplateTask::endOfCycle()
{
  QcInfoLogger::GetInstance() << "endOfCycle" << AliceO2::InfoLogger::InfoLogger::endm;
}

void TemplateTask::endOfActivity(Activity &activity)
{
  QcInfoLogger::GetInstance() << "endOfActivity" << AliceO2::InfoLogger::InfoLogger::endm;
}

void TemplateTask::reset()
{
  QcInfoLogger::GetInstance() << "Reset" << AliceO2::InfoLogger::InfoLogger::endm;
}

}
}
}
