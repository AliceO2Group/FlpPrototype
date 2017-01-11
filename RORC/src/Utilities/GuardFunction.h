/// \file GuardFunction.h
/// \brief Definition of various useful utilities that don't really belong anywhere in particular
///
/// \author Pascal Boeschoten (pascal.boeschoten@cern.ch)

#pragma once

#include <functional>

namespace AliceO2 {
namespace ReadoutCard {
namespace Utilities {

class GuardFunction
{
  public:
    GuardFunction(std::function<void()> destruct)
        : mDestruct(destruct)
    {
    }

    template <class Callable>
    GuardFunction(Callable&& construct, std::function<void()> destruct)
        : mDestruct(destruct)
    {
      construct();
    }

    ~GuardFunction()
    {
      mDestruct();
    }

  private:
    std::function<void()> mDestruct;
};

} // namespace Util
} // namespace ReadoutCard
} // namespace AliceO2
