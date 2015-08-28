///
/// \file   SamplerInterface.cxx
/// \author Barthelemy von Haller
///

#include "DataSampling/SamplerInterface.h"

using namespace std;

namespace AliceO2 {
namespace DataSampling {

SamplerInterface::SamplerInterface()
    : mLocation(""), mPolicy(Policy::Most), mFormat(DataFormat::Raw)
{
}

SamplerInterface::SamplerInterface(std::string Uri)
    : mPolicy(Policy::Most), mFormat(DataFormat::Raw)
{
  setLocation(Uri);
}

SamplerInterface::~SamplerInterface()
{
}

void SamplerInterface::setLocation(std::string Uri)
{
  cout << "DataSampling::setLocation : " << Uri << endl;
  mLocation = Uri;
}

std::string SamplerInterface::getLocation() const
{
  return mLocation;
}

void SamplerInterface::setPolicy(Policy policy)
{
  cout << "DataSampling::setPolicy : " << policy << endl;
  mPolicy = policy;
}
void SamplerInterface::setDataFormat(DataFormat format)
{
  cout << "DataSampling::setDataFormat : " << format << endl;
  mFormat = format;
}

void SamplerInterface::setDataFormat(string format)
{
  if (format == "Raw") {
    setDataFormat(DataFormat::Raw);
  } else if (format == "STF") {
    setDataFormat(DataFormat::STF);
  } else if (format == "TF") {
    setDataFormat(DataFormat::TF);
  } else if (format == "CTF") {
    setDataFormat(DataFormat::CTF);
  } else if (format == "AOD") {
    setDataFormat(DataFormat::AOD);
  } else if (format == "ESD") {
    setDataFormat(DataFormat::ESD);
  }
}

DataFormat SamplerInterface::getDataFormat() const
{
 return mFormat;
}

Policy SamplerInterface::getPolicy() const
{
 return mPolicy;
}

} /* namespace DataSampling */
} /* namespace AliceO2 */
