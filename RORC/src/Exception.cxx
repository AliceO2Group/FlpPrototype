/// \file Exception.cxx
/// \brief Implementation of the RORC exceptions and related functions.
///
/// \author Pascal Boeschoten (pascal.boeschoten@cern.ch)

#include "RORC/Exception.h"
#include <stdexcept>
#include <boost/exception/all.hpp>
#include "ExceptionInternal.h"

using namespace AliceO2::ReadoutCard;
namespace b = boost;

namespace AliceO2 {
namespace ReadoutCard {

const char* Exception::what() const noexcept
{
  try {
    if (auto info = boost::get_error_info<ErrorInfo::Message>(*this)) {
      return info->data();
    } else {
      return "AliceO2::ReadoutCard::Exception";
    }
  }
  catch (const std::exception& e) {
    return "AliceO2::ReadoutCard::Exception";
  }
}

} // namespace ReadoutCard
} // namespace AliceO2
