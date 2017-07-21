///
/// \file   InjectorFactor.h
/// \author Barthelemy von Haller
///

#ifndef PROJECT_INJECTORFACTORY_H
#define PROJECT_INJECTORFACTORY_H

#include "DataSampling/InjectorInterface.h"

namespace AliceO2 {
namespace DataSampling {

class InjectorFactory
{
  private:
    InjectorFactory(){};
    virtual ~InjectorFactory(){};

  public:
    /**
    * \brief Create a new instance of a SamplerInterface.
    * The SamplerInterface actual class is decided based on the parameter passed.
    * @param name Either MockInjector or FairInjector
    * @todo add a version of this method to get a class based on the Configuration system
    * @return an instance of the class (the caller is the owner)
    */
    static std::unique_ptr<InjectorInterface> create(std::string name);
};

}
}

#endif //PROJECT_INJECTORFACTORY_H
