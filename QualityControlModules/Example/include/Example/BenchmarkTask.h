///
/// \file   BenchmarkTask.h
/// \author Barthelemy von Haller
///

#ifndef QC_MODULE_EXAMPLE_BENCHMARKTASK_H
#define QC_MODULE_EXAMPLE_BENCHMARKTASK_H

#include "QualityControl/TaskInterface.h"

#include <vector>
#include "Configuration/Configuration.h"

class TH1F;

using namespace AliceO2::QualityControl::Core;

namespace AliceO2 {
namespace QualityControlModules {
namespace Example {

/// \brief Example Quality Control Task
/// It is final because there is no reason to derive from it. Just remove it if needed.
/// \author Barthelemy von Haller
class BenchmarkTask /*final*/: public TaskInterface // todo add back the "final" when doxygen is fixed
{
  public:
    /// \brief Constructor
    BenchmarkTask();
    /// Destructor
    virtual ~BenchmarkTask();

    // Definition of the methods for the template method pattern
    void initialize() override;
    void startOfActivity(Activity &activity) override;
    void startOfCycle() override;
    void monitorDataBlock(DataBlock &block) override;
    void endOfCycle() override;
    void endOfActivity(Activity &activity) override;
    void Reset() override;

  private:

    std::vector<TH1F*> mHistos;
    ConfigFile mConfigFile;
    size_t mNumberHistos;
    size_t mNumberChecks;
    std::string mTypeOfChecks;
    std::string mModuleOfChecks;

//    ClassDef(BenchmarkTask,1);
};

}
}
}

#endif //QC_MODULE_EXAMPLE_BENCHMARKTASK_H
