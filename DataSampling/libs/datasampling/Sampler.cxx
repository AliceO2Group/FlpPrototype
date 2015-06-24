///
/// \file   Sampler.cxx
/// \author Barthelemy von Haller
///

#include "Sampler.h"

#include <iostream>

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

void* Sampler::getData(int timeout)
{
  cout << "DataSampling::getData : " << endl;
  return nullptr;
}
void Sampler::releaseData()
{
  cout << "DataSampling::releaseData : " << endl;
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
