///
/// @file    World.cxx
/// @author  Barthelemy von Haller
///

#include <ProjA/World.h>
#include <Common/SimpleLog.h>


#include <iostream>

namespace AliceO2 {
namespace ProjectTemplate {
namespace ProjA {

void World::greet()
{
  SimpleLog log;
  
  log.info("hello(info)");
  log.error("hello(error)");
  log.warning("hello(warning)");  
  
  std::cout << "Common world!!" << std::endl;
}

int World::returnsN(int n)
{

  /// \todo This is how you can markup a todo in your code that will show up in the documentation of your project.
  /// \bug This is how you annotate a bug in your code.
  return n;
}

} // namespace ProjA
} // namespace ProjectTemplate
} // namespace AliceO2
