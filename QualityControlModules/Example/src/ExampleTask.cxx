///
/// \file   ExampleTask.cxx
/// \author Barthelemy von Haller
///

#include "Example/ExampleTask.h"
#include "QualityControl/QcInfoLogger.h"
#include <TH1.h>
#include <TCanvas.h>

using namespace std;

namespace AliceO2 {
namespace QualityControlModules {
namespace Example {

ExampleTask::ExampleTask()
    : TaskInterface(), mPayloadSize(nullptr), mHisto2(nullptr)
{
  for(int i = 0 ; i < 25 ; i++) {
    mHistos[i] = nullptr;
  }
}

ExampleTask::~ExampleTask()
{
  // TODO use unique_ptr instead
  if (mPayloadSize) {
    delete mPayloadSize;
  }
  if (mHisto2) {
    delete mHisto2;
  }
  for(int i = 0 ; i < 25 ; i++) {
    if(mHistos[i]) {
      delete mHistos[i];
    }
  }
}

void ExampleTask::initialize()
{
  QcInfoLogger::GetInstance() << "initialize ExampleTask" << AliceO2::InfoLogger::InfoLogger::endm;
  mPayloadSize = new TH1F("payloadsize", "Payload size of blocks;bytes", 2048, 0, 2047);
  mPayloadSize->SetCanExtend(TH1::kXaxis);
  mHisto2 = new TH1F("second", "second", 100, -10, 10);
  getObjectsManager()->startPublishing(mPayloadSize);
  getObjectsManager()->addCheck(mPayloadSize, "checkNonEmpty", "AliceO2::QualityControlModules::Common::NonEmpty", "QcCommon");
  getObjectsManager()->addCheck(mPayloadSize, "checkMeanIsAbove", "AliceO2::QualityControlModules::Common::MeanIsAbove", "QcCommon");
  getObjectsManager()->startPublishing(mHisto2, "my second object"); // explicit name, different from the TObject's name
  getObjectsManager()->addCheck("my second object", "checkMeanIsAbove", "AliceO2::QualityControlModules::Common::MeanIsAbove", "QcCommon");
  mNumberSubblocks = new TH1F("number sub blocks", "Number of subblocks", 100, 1, 100);
  getObjectsManager()->startPublishing(mNumberSubblocks);
  mSubPayloadSize = new TH1F("payloadsizesubblocks", "Payload size of subblocks;bytes", 2048, 0, 2047);
  mSubPayloadSize->SetCanExtend(TH1::kXaxis);
  getObjectsManager()->startPublishing(mSubPayloadSize);

  for(int i = 0 ; i < 25 ; i++) {
    stringstream name;
    name << "array-" << i;
    mHistos[i]  = new TH1F(name.str().c_str(), name.str().c_str(), 100, 0, 99);
    getObjectsManager()->startPublishing(mHistos[i], name.str());
  }
}

void ExampleTask::startOfActivity(Activity &activity)
{
  QcInfoLogger::GetInstance() << "startOfActivity" << AliceO2::InfoLogger::InfoLogger::endm;
  mPayloadSize->Reset();
  mNumberSubblocks->Reset();
  mSubPayloadSize->Reset();
}

void ExampleTask::startOfCycle()
{
  QcInfoLogger::GetInstance() << "startOfCycle" << AliceO2::InfoLogger::InfoLogger::endm;
}

void ExampleTask::monitorDataBlock(std::vector<std::shared_ptr<DataBlockContainer>> &block)
{
  uint32_t payloadSizeBytes = 0;

//  cout << "--> block received : " << endl;
  if (block.size() > 0) {
    if (block.at(0) != nullptr) {
//      cout << "    id : " << block.at(0)->getData()->header.id << endl;
//      cout << "    blockType : " << std::hex << block.at(0)->getData()->header.blockType << endl;
//      cout << "    headerSize : " << std::dec << block.at(0)->getData()->header.headerSize << endl;
//      cout << "    payload size : " << std::dec << block.at(0)->getData()->header.dataSize << endl;
      payloadSizeBytes = block.at(0)->getData()->header.dataSize / 8;
    } else {
      cout << "     Container pointer invalid" << endl;
    }
  } else {
    cout << "    Empty vector!" << endl;
  }

  uint32_t totalPayloadSize = 0;
  for(auto b : block) {
    uint32_t size = b->getData()->header.dataSize / 8;
    mSubPayloadSize->Fill(size);
    totalPayloadSize += size;
  }

//  QcInfoLogger::GetInstance() << "Payload size " << payloadSizeBytes << AliceO2::InfoLogger::InfoLogger::endm;
  mPayloadSize->Fill(totalPayloadSize);
  mHisto2->FillRandom("gaus", 10);
  mNumberSubblocks->Fill(block.size());


//  QcInfoLogger::GetInstance() << "ExampleTask monitorDataBlock (5x1s)" << AliceO2::InfoLogger::InfoLogger::endm;
//  sleep(2);
//  QcInfoLogger::GetInstance() << "   ." << AliceO2::InfoLogger::InfoLogger::endm;
//  sleep(1);
//  QcInfoLogger::GetInstance() << "   ." << AliceO2::InfoLogger::InfoLogger::endm;
//  sleep(1);
//  QcInfoLogger::GetInstance() << "   ." << AliceO2::InfoLogger::InfoLogger::endm;
//  sleep(1);
//  QcInfoLogger::GetInstance() << "   ." << AliceO2::InfoLogger::InfoLogger::endm;
//  sleep(1);
//  QcInfoLogger::GetInstance() << "   ." << AliceO2::InfoLogger::InfoLogger::endm;
}

void ExampleTask::endOfCycle()
{
  QcInfoLogger::GetInstance() << "endOfCycle" << AliceO2::InfoLogger::InfoLogger::endm;
}

void ExampleTask::endOfActivity(Activity &activity)
{
  QcInfoLogger::GetInstance() << "endOfActivity" << AliceO2::InfoLogger::InfoLogger::endm;
}

void ExampleTask::reset()
{
  QcInfoLogger::GetInstance() << "Reset" << AliceO2::InfoLogger::InfoLogger::endm;
}

}
}
}
