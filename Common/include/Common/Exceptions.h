/// \file   Exceptions.h
/// \author Barthelemy von Haller
///
/// \brief  Define all the exceptions based on boost::exception that we use in the project.

#ifndef COMMON_EXCEPTIONS_H
#define COMMON_EXCEPTIONS_H

#include <boost/exception/all.hpp>
#include <exception>

namespace AliceO2 {
namespace Common {

/// Definitions of error_info structures to store extra info the boost way.
typedef boost::error_info<struct errinfo_object_name_, std::string> errinfo_object_name;
typedef boost::error_info<struct errinfo_details_, std::string> errinfo_details;

typedef boost::error_info<struct errinfo_db_errno_, int> errinfo_db_errno;
typedef boost::error_info<struct errinfo_db_message_, std::string> errinfo_db_message;

struct ExceptionBase : virtual std::exception, virtual boost::exception
{
};

struct ObjectNotFoundError : virtual ExceptionBase
{
  const char *what() const noexcept override
  {
    return "Object not found error";
  }
};

struct FatalException : virtual ExceptionBase {};

struct DatabaseException : virtual ExceptionBase {};
struct FatalDatabaseException : virtual FatalException{};

// we could define further exceptions this way :
//    struct io_error: virtual exception_base { };
//    struct file_error: virtual io_error { };
//    struct read_error: virtual io_error { };
//    struct file_read_error: virtual file_error, virtual read_error { };
// Note that we don't add members because boost exceptions allow adding arbitrary stuff any ways.
// See above for adding specific information but in a generic way.

// TODO add instructions on how to display diagnostic

}
}

#endif //COMMON_EXCEPTIONS_H
