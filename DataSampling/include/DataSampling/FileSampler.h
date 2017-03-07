///
/// \file   FileSampler.h
/// \author Barthelemy von Haller
///

#ifndef DATASAMPLING_LIBS_DATASAMPLING_FILESAMPLER_H_
#define DATASAMPLING_LIBS_DATASAMPLING_FILESAMPLER_H_

#include "SamplerInterface.h"

namespace AliceO2 {
namespace DataSampling {

/// \brief  Sampler that gets data from a file.
///
/// This data sampler reads data blocks from a file.
///
/// \author Barthelemy von Haller
class FileSampler: public SamplerInterface
{
  public:
    /// Standard Constructor
    FileSampler();
    /// Destructor
    virtual ~FileSampler();

    virtual std::vector<std::shared_ptr<DataBlockContainer>>* getData(int timeout = 0);
    virtual void releaseData();

  private:
    std::string getFilePath();
};

} /* namespace Utilities */
} /* namespace AliceO2 */

#endif /* DATASAMPLING_LIBS_DATASAMPLING_FILESAMPLER_H_ */
