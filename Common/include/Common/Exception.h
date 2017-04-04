/// \file   Exception.h
/// \author Pascal Boeschoten
///
/// \brief  Define an exceptions based on boost::exception

#ifndef COMMON_EXCEPTION_H
#define COMMON_EXCEPTION_H

#include <exception>
#include <string>
#include <boost/exception/all.hpp>

namespace AliceO2 {
namespace Common {

namespace ErrorInfo
{
using Message = boost::error_info<struct _Message, std::string>;
using Input = boost::error_info<struct _Input, std::string>;
using Suffix = boost::error_info<struct _Suffix, std::string>;
using FileName = boost::error_info<struct _FileName, std::string>;
using FilesystemType = boost::error_info<struct _FileSystemType, std::string>;
}

struct Exception : virtual std::exception, virtual boost::exception
{
    /// The what() function is overridden to use the 'ErrorInfo::Message' when available
    virtual const char* what() const noexcept override;
};

struct ProgramOptionException : virtual Exception {};

}
}

#endif //COMMON_EXCEPTION_H
