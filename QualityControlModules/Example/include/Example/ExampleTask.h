///
/// \file   ExampleTask.h
/// \author Barthelemy von Haller
///

#ifndef QC_MODULE_EXAMPLE_EXAMPLETASK_H
#define QC_MODULE_EXAMPLE_EXAMPLETASK_H

#include "QualityControl/TaskInterface.h"

class TH1F;

using namespace AliceO2::QualityControl::Core;

namespace AliceO2 {
namespace QualityControlModules {
namespace Example {

/// \brief Example Quality Control Task
/// It is final because there is no reason to derive from it. Just remove it if needed.
/// \author Barthelemy von Haller
class ExampleTask /*final*/ : public TaskInterface // todo add back the "final" when doxygen is fixed
{
  public:
    /// \brief Constructor
    ExampleTask();
    /// Destructor
    virtual ~ExampleTask();

    // Definition of the methods for the template method pattern
    void initialize() override;
    void startOfActivity(Activity &activity) override;
    void startOfCycle() override;
    void monitorDataBlock(DataBlock &block) override;
    void endOfCycle() override;
    void endOfActivity(Activity &activity) override;
    void Reset() override;

  private:

    TH1F *mHisto1, *mHisto2;
};

}
}
}

#endif //QC_MODULE_EXAMPLE_EXAMPLETASK_H
