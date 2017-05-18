/// \file AliceLowlevelFrontend.h
/// \brief Definition of ALICE Lowlevel Frontend (ALF) & related DIM items
///
/// \author Pascal Boeschoten (pascal.boeschoten@cern.ch)

#ifndef ALICEO2_READOUTCARD_UTILITIES_ALF_ALICELOWLEVELFRONTEND_H
#define ALICEO2_READOUTCARD_UTILITIES_ALF_ALICELOWLEVELFRONTEND_H

#include <string>
#include <functional>
#include <dim/dim.hxx>
#include <dim/dis.hxx>
#include <dim/dic.hxx>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/format.hpp>
#include "ExceptionInternal.h"

namespace Alf {

struct ServiceNames
{
    ServiceNames(int serialNumber, int channelNumber)
        : serial(serialNumber), channel(channelNumber)
    {
    }

    std::string registerReadRpc()
    {
      return boost::str(boost::format("ALF/SERIAL_%d/CHANNEL_%d/REGISTER_READ") % serial % channel);
    }

    std::string registerWriteRpc()
    {
      return boost::str(boost::format("ALF/SERIAL_%d/CHANNEL_%d/REGISTER_WRITE") % serial % channel);
    }

    std::string publishCommandRpc()
    {
      return boost::str(boost::format("ALF/SERIAL_%d/CHANNEL_%d/PUBLISH_SERVICE") % serial % channel);
    }

    std::string publishStopCommandRpc()
    {
      return boost::str(boost::format("ALF/SERIAL_%d/CHANNEL_%d/PUBLISH_SERVICE_STOP") % serial % channel);
    }

    std::string temperature()
    {
      return boost::str(boost::format("ALF/SERIAL_%d/CHANNEL_%d/TEMPERATURE") % serial % channel);
    }

    const int serial;
    const int channel;
};

/// We use this in a few places because DIM insists on non-const char*
std::vector<char> toCharBuffer(const std::string& string, bool addTerminator = true)
{
  std::vector<char> buffer(string.begin(), string.end());
  if (addTerminator) {
    buffer.push_back('\0');
  }
  return buffer;
}

template <typename DimObject>
void setDataString(const std::string& string, DimObject& dimObject, bool addTerminator = true)
{
  auto buffer = toCharBuffer(string, addTerminator);
  dimObject.setData(buffer.data(), buffer.size());
}

constexpr size_t PREFIX_LENGTH = 8;

inline std::string successPrefix()
{
  return "success:";
}

inline std::string failPrefix()
{
  return "failure:";
}

inline std::string makeSuccessString(const std::string& string)
{
  return successPrefix() + string;
}

inline std::string makeFailString(const std::string& string)
{
  return failPrefix() + string;
}

inline bool isSuccess(const std::string& string)
{
  return boost::starts_with(string, successPrefix());
}

inline bool isFail(const std::string& string)
{
  return boost::starts_with(string, failPrefix());
}

inline std::string stripPrefix(const std::string& string)
{
  return string.substr(PREFIX_LENGTH);
}

class PublishRpc: public DimRpcInfo
{
  public:
    PublishRpc(const std::string& serviceName)
        : DimRpcInfo(serviceName.c_str(), toCharBuffer("").data())
    {
    }

    void publish(std::string dnsName, double frequency, std::vector<size_t> addresses)
    {
      std::ostringstream stream;
      stream << dnsName << ';';
      for (size_t i = 0; i < addresses.size(); ++i) {
        stream << addresses[i];
        if ((i + 1) < addresses.size()) {
          stream << ',';
        }
      }
      stream << ';' << frequency;
      printf("Publish: %s\n", stream.str().c_str());
      setDataString(stream.str(), *this);

      auto returnValue = std::string(getString());
//      printf("Publish got return: %s\n", returnValue.c_str());
      if (isFail(returnValue)) {
        BOOST_THROW_EXCEPTION(AliceO2::roc::Exception() << AliceO2::roc::ErrorInfo::Message(returnValue));
      }
    }
};

class PublishStopRpc: public DimRpcInfo
{
  public:
    PublishStopRpc(const std::string &serviceName)
      : DimRpcInfo(serviceName.c_str(), toCharBuffer("").data())
    {
    }

    void stop(std::string dnsName)
    {
      setDataString(dnsName, *this);
      auto returnValue = std::string(getString());
      if (isFail(returnValue)) {
        BOOST_THROW_EXCEPTION(AliceO2::roc::Exception() << AliceO2::roc::ErrorInfo::Message(returnValue));
      }
    }
};

class RegisterReadRpc: public DimRpcInfo
{
  public:
    RegisterReadRpc(const std::string& serviceName)
        : DimRpcInfo(serviceName.c_str(), toCharBuffer("").data())
    {
    }

    uint32_t readRegister(uint64_t registerAddress)
    {
      setDataString(std::to_string(registerAddress), *this);
      auto returnValue = std::string(getString());
//      printf("Read got return: %s\n", returnValue.c_str());
      if (isFail(returnValue)) {
        BOOST_THROW_EXCEPTION(AliceO2::roc::Exception() << AliceO2::roc::ErrorInfo::Message(returnValue));
      }
      return boost::lexical_cast<uint32_t>(stripPrefix(returnValue));
    }
};

class RegisterWriteRpc: public DimRpcInfo
{
  public:
    RegisterWriteRpc(const std::string& serviceName)
        : DimRpcInfo(serviceName.c_str(), toCharBuffer("").data())
    {
    }

    void writeRegister(uint64_t registerAddress, uint32_t registerValue)
    {
      auto string = std::to_string(registerAddress) + ',' + std::to_string(registerValue);
      setDataString(string, *this);
      auto returnValue = std::string(getString());
//      printf("Write got return: %s\n", returnValue.c_str());
      if (isFail(returnValue)) {
        BOOST_THROW_EXCEPTION(AliceO2::roc::Exception() << AliceO2::roc::ErrorInfo::Message(returnValue));
      }
    }
};

class RegisterWriteBlockRpc: public DimRpcInfo
{
  public:
    RegisterWriteBlockRpc(const std::string& serviceName)
        : DimRpcInfo(serviceName.c_str(), toCharBuffer("").data())
    {
    }

    void writeRegister(uint64_t registerAddress, uint32_t registerValue)
    {
      auto string = std::to_string(registerAddress) + ',' + std::to_string(registerValue);
      setDataString(string, *this);
      auto returnValue = std::string(getString());
      printf("Write got return: %s\n", returnValue.c_str());
      if (isFail(returnValue)) {
        BOOST_THROW_EXCEPTION(AliceO2::roc::Exception() << AliceO2::roc::ErrorInfo::Message(returnValue));
      }
    }
};

class BasicRpcServer: public DimRpc
{
  public:
    using Callback = std::function<void(BasicRpcServer&)>;

    BasicRpcServer(const std::string& serviceName, const char* formatIn, const char* formatOut, Callback callback)
        : DimRpc(serviceName.c_str(), formatIn, formatOut), callback(callback)
    {
    }

  private:
    void rpcHandler()
    {
      try {
        callback(*this);
      } catch (const std::exception& e) {
        printf("Error: %s\n", e.what());
      }
    }

    Callback callback;
};

class StringRpcServer: public DimRpc
{
  public:
    using Callback = std::function<std::string(const std::string&)>;

    StringRpcServer(const std::string& serviceName, Callback callback)
        : DimRpc(serviceName.c_str(), "C", "C"), callback(callback)
    {
    }

  private:
    void rpcHandler() override
    {
      try {
        std::string parameter = getString();
        std::string returnValue = callback(parameter);
        Alf::setDataString(Alf::makeSuccessString(returnValue), *this);
      } catch (const std::exception& e) {
        Alf::setDataString(Alf::makeFailString(e.what()), *this);
      }
    }

    Callback callback;
};

class CallbackCommand : public DimCommand
{
  public:
    CallbackCommand(const std::string& serviceName, const char* format, std::function<void()> callback)
        : DimCommand(serviceName.c_str(), format), callback(callback)
    {
    }

  private:
    void commandHandler() override
    {
      callback();
    }

    std::function<void()> callback;
};

}

#endif // ALICEO2_READOUTCARD_UTILITIES_ALF_ALICELOWLEVELFRONTEND_H
