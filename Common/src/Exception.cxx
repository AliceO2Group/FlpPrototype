/// \file Exception.cxx
/// \brief Implementation of the RORC exceptions and related functions.
///
/// \author Pascal Boeschoten (pascal.boeschoten@cern.ch)

#include "Common/Exception.h"
#include <stdexcept>
#include <boost/exception/all.hpp>

namespace AliceO2 
{
namespace Common
{

const char* Exception::what() const noexcept
{
  try {
    if (auto info = boost::get_error_info<ErrorInfo::Message>(*this)) {
      return info->data();
    } else {
      return "AliceO2::Common::Exception";
    }
  }
  catch (const std::exception& e) {
    return "AliceO2::Common::Exception";
  }
}

} // namespace Common
} // namespace AliceO2
