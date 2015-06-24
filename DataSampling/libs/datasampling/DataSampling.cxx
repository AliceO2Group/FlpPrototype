///
/// @file   DataSampling.cxx
/// @author Barthelemy von Haller
///

#include "DataSampling.h"
#include <iostream>

using namespace std;

namespace AliceO2 {
namespace DataSampling {

DataSampling::DataSampling()
    : m_location(""), m_policy(Policy::Most), m_format(DataFormat::Raw)
{
}

DataSampling::DataSampling(std::string Uri)
    : m_policy(Policy::Most), m_format(DataFormat::Raw)
{
  setLocation(Uri);
}

DataSampling::~DataSampling()
{
}

void DataSampling::setLocation(std::string Uri)
{
  cout << "DataSampling::setLocation : " << Uri << endl;
  m_location = Uri;
}

std::string DataSampling::getLocation() const
{
  return m_location;
}

void* DataSampling::getData(int timeout)
{
  cout << "DataSampling::getData : " << endl;
  return nullptr;
}
void DataSampling::releaseData()
{
  cout << "DataSampling::releaseData : " << endl;
}
void DataSampling::setPolicy(Policy policy)
{
  cout << "DataSampling::setPolicy : " << policy << endl;
  m_policy = policy;
}
void DataSampling::setDataFormat(DataFormat format)
{
  cout << "DataSampling::setDataFormat : " << format << endl;
  m_format = format;
}

DataFormat DataSampling::getDataFormat() const
{
  return m_format;
}

Policy DataSampling::getPolicy() const
{
  return m_policy;
}

} /* namespace DataSampling */
} /* namespace AliceO2 */
