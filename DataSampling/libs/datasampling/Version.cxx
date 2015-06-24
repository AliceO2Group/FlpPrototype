///
/// @file    Version.cxx
/// @brief   Version Information
/// @author  Barthelemy von Haller
///

#include <datasampling/Version.h>
#include <sstream>

namespace AliceO2 {
namespace DataSampling {

int Version::getMajor()
{
  return HELLO_VERSION_MAJOR;
}

int Version::getMinor()
{
  return HELLO_VERSION_MINOR;
}

int Version::getPatch()
{
  return HELLO_VERSION_PATCH;
}

std::string Version::getString()
{
  std::ostringstream version;
  version << HELLO_VERSION_MAJOR << '.' << HELLO_VERSION_MINOR << '.' << HELLO_VERSION_PATCH;
  return version.str();
}

std::string Version::getRevision()
{
  return HELLO_VCS_REVISION;
}

std::string Version::getRevString()
{
  std::ostringstream version;
  version << getString() << '.' << HELLO_VCS_REVISION;
  return version.str();
}
}
}
