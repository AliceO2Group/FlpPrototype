///
/// \file   TemplateCheck.h
/// \author Barthelemy von Haller
///

#ifndef QUALITYCONTROL_LIBS_CHECKER_TemplateCheck_H_
#define QUALITYCONTROL_LIBS_CHECKER_TemplateCheck_H_

#include "QualityControl/MonitorObject.h"
#include "QualityControl/Quality.h"
#include "QualityControl/CheckInterface.h"

namespace AliceO2 {
namespace QualityControlModules {
namespace Template {

/// \brief  Check whether a plot is empty or not.
///
/// \author Barthelemy von Haller
class TemplateCheck : public AliceO2::QualityControl::Checker::CheckInterface
{
  public:
    /// Default constructor
    TemplateCheck();
    /// Destructor
    ~TemplateCheck() override;

    void configure(std::string name) override;
    Quality check(const MonitorObject *mo) override;
    void beautify(MonitorObject *mo, Quality checkResult = Quality::Null) override;
    std::string getAcceptedType() override;

    ClassDefOverride(TemplateCheck,1);
};

} // namespace Example
} // namespace QualityControl
} // namespace AliceO2

#endif // QUALITYCONTROL_LIBS_CHECKER_TemplateCheck_H_
