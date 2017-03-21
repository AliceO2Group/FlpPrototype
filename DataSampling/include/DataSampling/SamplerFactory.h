///
/// \file   SamplerFactory.cxx
/// \author Barthelemy von Haller
///

#ifndef PROJECT_SAMPLERFACTORY_H
#define PROJECT_SAMPLERFACTORY_H

#include "DataSampling/SamplerInterface.h"

namespace AliceO2 {
namespace DataSampling {

class SamplerFactory
{
  private:
    SamplerFactory(){};
    virtual ~SamplerFactory(){};

  public:

    /**
    * \brief Create a new instance of a SamplerInterface.
    * The SamplerInterface actual class is decided based on the parameter passed.
    * @param name Either MockSampler, FileSampler or FairSampler
    * @todo add a version of this method to get a class based on the Configuration system
    * @return an instance of the class (the caller is the owner)
    */
    static SamplerInterface *create(std::string name);
};

}
}

#endif //PROJECT_SAMPLERFACTORY_H
