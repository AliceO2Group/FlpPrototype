///
/// \file   FileSampler.h
/// \author Barthelemy von Haller
///

#ifndef DATASAMPLING_LIBS_DATASAMPLING_FILESAMPLER_H_
#define DATASAMPLING_LIBS_DATASAMPLING_FILESAMPLER_H_

#include "Sampler.h"

namespace AliceO2 {
namespace DataSampling {

class FileSampler: public Sampler
{
  public:
    FileSampler();
    virtual ~FileSampler();

    virtual DataBlock* getData(int timeout = 0);
    virtual void releaseData();

  private:
    std::string getFilePath();

};

} /* namespace Utilities */
} /* namespace AliceO2 */

#endif /* DATASAMPLING_LIBS_DATASAMPLING_FILESAMPLER_H_ */
