///
/// \file   DaqCheck.h
/// \author Barthelemy von Haller
///

#ifndef QUALITYCONTROL_LIBS_CHECKER_DaqCheck_H_
#define QUALITYCONTROL_LIBS_CHECKER_DaqCheck_H_

#include "QualityControl/MonitorObject.h"
#include "QualityControl/Quality.h"
#include "QualityControl/CheckInterface.h"

namespace AliceO2 {
namespace QualityControlModules {
namespace Daq {

/// \brief  Check whether a plot is empty or not.
///
/// \author Barthelemy von Haller
class DaqCheck : public AliceO2::QualityControl::Checker::CheckInterface
{
  public:
    /// Default constructor
    DaqCheck();
    /// Destructor
    ~DaqCheck() override;

    // Override interface
    void configure(std::string name) override;
    Quality check(const MonitorObject *mo) override;
    void beautify(MonitorObject *mo, Quality checkResult = Quality::Null) override;
    std::string getAcceptedType() override;

  ClassDefOverride(DaqCheck, 1);
};

} // namespace Example
} // namespace QualityControl
} // namespace AliceO2

#endif // QUALITYCONTROL_LIBS_CHECKER_DaqCheck_H_
