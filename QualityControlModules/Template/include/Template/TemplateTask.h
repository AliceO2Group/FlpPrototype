///
/// \file   TemplateTask.h
/// \author Barthelemy von Haller
///

#ifndef QC_MODULE_EXAMPLE_EXAMPLETASK_H
#define QC_MODULE_EXAMPLE_EXAMPLETASK_H

#include "QualityControl/TaskInterface.h"

class TH1F;

using namespace AliceO2::QualityControl::Core;

namespace AliceO2 {
namespace QualityControlModules {
namespace Template {

/// \brief Example Quality Control Task
/// It is final because there is no reason to derive from it. Just remove it if needed.
/// \author Barthelemy von Haller
class TemplateTask /*final*/: public TaskInterface // todo add back the "final" when doxygen is fixed
{
  public:
    /// \brief Constructor
    TemplateTask();
    /// Destructor
    ~TemplateTask() override;

    // Definition of the methods for the template method pattern
    void initialize() override;
    void startOfActivity(Activity &activity) override;
    void startOfCycle() override;
    void monitorDataBlock(std::vector<std::shared_ptr<DataBlockContainer>> &block) override;
    void endOfCycle() override;
    void endOfActivity(Activity &activity) override;
    void reset() override;

    // accessors (for the tests)
    TH1F *getHistogram(){return mHistogram;}

  private:

    TH1F* mHistogram;
};

}
}
}

#endif //QC_MODULE_EXAMPLE_EXAMPLETASK_H
