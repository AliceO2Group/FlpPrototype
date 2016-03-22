///
/// @file    World.cxx
/// @author  Barthelemy von Haller
///

#include "Common/World.h"

#include <iostream>

#include <InfoLogger/InfoLogger.hxx>

namespace AliceO2 {
namespace Common {

void World::greet()
{
  AliceO2::InfoLogger::InfoLogger theLog;

  theLog.log("infoLogger message test");

  std::cout << "Common world!!" << std::endl;
}

int World::returnsN(int n)
{

  /// \todo This is how you can markup a todo in your code that will show up in the documentation of your project.
  /// \bug This is how you annotate a bug in your code.
  return n;
}

} // namespace Common
} // namespace AliceO2
