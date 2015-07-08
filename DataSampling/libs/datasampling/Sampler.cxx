///
/// \file   Sampler.cxx
/// \author Barthelemy von Haller
///

#include "Sampler.h"

using namespace std;

namespace AliceO2 {
namespace DataSampling {

Sampler::Sampler()
    : mLocation(""), mPolicy(Policy::Most), mFormat(DataFormat::Raw)
{
}

Sampler::Sampler(std::string Uri)
    : mPolicy(Policy::Most), mFormat(DataFormat::Raw)
{
  setLocation(Uri);
}

Sampler::~Sampler()
{
}

void Sampler::setLocation(std::string Uri)
{
  cout << "DataSampling::setLocation : " << Uri << endl;
  mLocation = Uri;
}

std::string Sampler::getLocation() const
{
  return mLocation;
}

void Sampler::setPolicy(Policy policy)
{
  cout << "DataSampling::setPolicy : " << policy << endl;
  mPolicy = policy;
}
void Sampler::setDataFormat(DataFormat format)
{
  cout << "DataSampling::setDataFormat : " << format << endl;
  mFormat = format;
}

void Sampler::setDataFormat(string format)
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

DataFormat Sampler::getDataFormat() const
{
 return mFormat;
}

Policy Sampler::getPolicy() const
{
 return mPolicy;
}

} /* namespace DataSampling */
} /* namespace AliceO2 */
