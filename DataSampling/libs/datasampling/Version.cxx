///
/// \file    Version.cxx
/// \brief   Version Information
/// \author  Barthelemy von Haller
///

#include <datasampling/Version.h>
#include <sstream>

namespace AliceO2 {
namespace DataSampling {

int Version::getMajor()
{
  return DATASAMPLING_VERSION_MAJOR;
}

int Version::getMinor()
{
  return DATASAMPLING_VERSION_MINOR;
}

int Version::getPatch()
{
  return DATASAMPLING_VERSION_PATCH;
}

std::string Version::getString()
{
  std::ostringstream version;
  version << DATASAMPLING_VERSION_MAJOR << '.' << DATASAMPLING_VERSION_MINOR << '.' << DATASAMPLING_VERSION_PATCH;
  return version.str();
}

std::string Version::getRevision()
{
  return DATASAMPLING_VCS_REVISION;
}

std::string Version::getRevString()
{
  std::ostringstream version;
  version << getString() << '.' << DATASAMPLING_VCS_REVISION;
  return version.str();
}
}
}
