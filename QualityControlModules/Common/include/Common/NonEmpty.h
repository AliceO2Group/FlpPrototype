///
/// \file   NonEmpty.h
/// \author flpprotodev
///

#ifndef QUALITYCONTROL_LIBS_CHECKER_NONEMPTY_H_
#define QUALITYCONTROL_LIBS_CHECKER_NONEMPTY_H_

#include "QualityControl/CheckInterface.h"

namespace AliceO2 {
namespace QualityControlModules {
namespace Common {

class NonEmpty /*final*/: public AliceO2::QualityControl::Checker::CheckInterface
{
  public:
    NonEmpty();
    virtual ~NonEmpty();

    Quality check(const MonitorObject *mo) override;
    void beautify(MonitorObject *mo, Quality checkResult = Quality::Null) override;
    std::string getAcceptedType() override;

    ClassDefOverride(NonEmpty,1);
};

} // namespace Checker 
} // namespace QualityControl 
} // namespace AliceO2 

#endif // QUALITYCONTROL_LIBS_CHECKER_NONEMPTY_H_ 
