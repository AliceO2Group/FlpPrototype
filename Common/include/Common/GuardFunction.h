/// \file GuardFunction.h
/// \brief Definition of various useful utilities that don't really belong anywhere in particular
///
/// \author Pascal Boeschoten (pascal.boeschoten@cern.ch)

#ifndef ALICEO2_FLPPROTOTYPE_COMMON_GUARDFUNCTION_H_
#define ALICEO2_FLPPROTOTYPE_COMMON_GUARDFUNCTION_H_

#include <functional>

namespace AliceO2
{
namespace Common
{

/// Class that executes a function on scope exit
class GuardFunction
{
  public:
    /// \param function Function to execute on scope exit. Must not throw.
    GuardFunction(std::function<void()> function)
        : mFunction(function)
    {
    }

    ~GuardFunction()
    {
      mFunction();
    }

  private:
    std::function<void()> mFunction;
};

} // namespace Common
} // namespace AliceO2

#endif // ALICEO2_FLPPROTOTYPE_COMMON_GUARDFUNCTION_H_