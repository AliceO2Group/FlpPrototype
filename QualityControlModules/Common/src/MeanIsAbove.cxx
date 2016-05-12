///
/// \file   MeanIsAbove.cxx
/// \author Barthelemy von Haller
///

#include "Common/MeanIsAbove.h"
#include <TClass.h>
#include <iostream>
#include <TH1.h>
#include "Configuration/Configuration.h"

ClassImp(AliceO2::QualityControlModules::Common::MeanIsAbove)

using namespace std;

namespace AliceO2 {
namespace QualityControlModules {
namespace Common {

MeanIsAbove::MeanIsAbove()
    : mThreshold(0.0)
{
}

MeanIsAbove::~MeanIsAbove()
{
}

void MeanIsAbove::configure(std::string name)
{
  // First call the parent !
  CheckInterface::configure(name);

  // TODO use the configuration system to set the params
//  ConfigFile configFile;
//  try {
//    configFile.load("file:../example.ini"); // not ok...
//  } catch (string &exception) {
//    cout << "error getting config file in MeanIsAbove : " << exception << endl;
//    mThreshold = 1.0f;
//    return;
//  }
  mThreshold = 1.0f;//std::stof(configFile.getValue<string>("Checks.checkMeanIsAbove/threshold"));
}

std::string MeanIsAbove::getAcceptedType()
{
  return "TH1";
}

Quality MeanIsAbove::check(const MonitorObject *mo)
{
  TH1 *th1 = dynamic_cast<TH1*>(mo->getObject());
  if (!th1) {
    // TODO
    return Quality::Null;
  }
  if (th1->GetMean() > mThreshold) {
    return Quality::Good;
  }
  return Quality::Bad;
}

void MeanIsAbove::beautify(MonitorObject *mo, Quality checkResult)
{
  // NOOP
}
} /* namespace Common */
} /* namespace QualityControlModules */
} /* namespace AliceO2 */

